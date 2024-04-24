import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
import datetime

from helpers import apology, login_required, lookup, usd, is_positive_integer

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    userId = session["user_id"]
    stocks = db.execute("SELECT symbol, shares FROM user_stocks WHERE user_id = ?", userId)
    currPrice = []
    stocksTotalPrice = 0
    for stock in stocks:
        res = lookup(stock["symbol"])
        stockCurrPrice = {'symbol': res["symbol"].lower(), 'price': res["price"]}
        stocksTotalPrice += res["price"] * stock["shares"]
        currPrice.append(stockCurrPrice)

    cashBalance = db.execute("SELECT cash FROM users WHERE id = ?", userId)[0]["cash"]
    grandTotal = stocksTotalPrice + cashBalance
    return render_template("index.html", stocks=stocks, currPrice=currPrice, cashBalance=usd(cashBalance), grandTotal=usd(grandTotal))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "GET":
        return render_template("buy.html")

    else:
        formSymbol = request.form.get("symbol")
        if not formSymbol:
            return apology("Symbol cannot be blank")
        symbol = formSymbol.lower()
        quote = lookup(symbol)
        if not quote:
            return apology("Symbol not found")
        shares = request.form.get("shares")
        if not shares or not is_positive_integer(shares):
            return apology("invalid value for shares. It must be a positive integer")

        userId = session["user_id"]
        price = quote["price"]
        userCash = db.execute("SELECT cash FROM users WHERE id = ?", userId)[0]["cash"]
        bill = price * int(shares)
        userCanAfford = userCash >= bill

        if not userCanAfford:
            return apology("You don't have enough founds in your account")

        current_date = datetime.date.today()
        current_time = datetime.datetime.now().time()
        formatted_date = current_date.strftime('%Y-%m-%d')
        formatted_time = current_time.strftime('%H:%M:%S')

        db.execute("INSERT INTO transactions (user_id, symbol, shares, price,transaction_type,transaction_date,transaction_time) VALUES (?, ?, ?, ?, ?, ?, ?)",
                   userId, symbol, int(shares), price, "buy", formatted_date, formatted_time)
        db.execute("UPDATE users SET cash = cash - ? WHERE id = ?", bill, userId)
        alreadyOwnsThisSymbol = db.execute(
            "SELECT symbol FROM user_stocks WHERE user_id = ? AND symbol = ?", userId, symbol)
        if alreadyOwnsThisSymbol:
            db.execute("UPDATE user_stocks SET shares = shares + ? WHERE user_id = ? AND symbol = ?",
                       int(shares), userId, symbol)
        else:
            db.execute("INSERT INTO user_stocks (user_id,symbol,shares) VALUES (?,?,?)",
                       userId, symbol, int(shares))
        return redirect("/")


@app.route("/history")
@login_required
def history():
    userId = session["user_id"]
    transactions = db.execute(
        "SELECT symbol,price,shares,transaction_type,transaction_date,transaction_time FROM transactions WHERE user_id = ?", userId)
    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    if request.method == "GET":
        return render_template("quote.html")
    else:
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("Symbol cannot be blank")
        quote = lookup(symbol)
        if not quote:
            return apology("Invalid symbol")
        price = usd(quote["price"])
        stockName = quote['symbol']
        return render_template("quoted.html", price=price, symbol=stockName)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "GET":
        return render_template("register.html")

    else:
        username = request.form.get("username")

        if not username:
            return apology("Username can not be blank")

        usernameAlreadyExists = db.execute("SELECT * FROM users WHERE username = ?", username)

        if usernameAlreadyExists:
            return apology("Username already exists")

        password = request.form.get("password")
        if not password:
            return apology("Password can't be blank")

        confirmation = request.form.get("confirmation")

        if not confirmation:
            return apology("Confirmation can't be blank")

        passwordsMatch = password == confirmation

        if not passwordsMatch:
            return apology("Passwords do not match!")

        hashedPassword = generate_password_hash(password)
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, hashedPassword)

        return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    userId = session["user_id"]
    if request.method == "GET":
        stocksSymbols = db.execute("SELECT symbol FROM user_stocks WHERE user_id = ?", userId)
        return render_template("sell.html", symbols=stocksSymbols)
    else:
        formSymbol = request.form.get("symbol")
        if not formSymbol:
            return apology("Please insert a symbol")
        symbol = formSymbol.lower()
        userOwnsStock = False
        ownedSymbols = db.execute("SELECT symbol FROM user_stocks WHERE user_id = ?", userId)
        for obj in ownedSymbols:
            if obj.get('symbol') == symbol:
                userOwnsStock = True
                break

        if not userOwnsStock:
            return apology("You do not own any share of the specified symbol")

        shares = request.form.get("shares")
        if not shares or not is_positive_integer(shares):
            return apology("Please enter a positive integer for the value of shares")

        userShares = db.execute(
            "SELECT shares FROM user_stocks WHERE user_id = ? AND symbol = ?", userId, symbol)[0]["shares"]
        userOwnsEnoughShares = userShares >= int(shares)
        if not userOwnsEnoughShares:
            return apology("You do not own enough shares")
        db.execute("UPDATE user_stocks SET shares = shares - ? WHERE user_id = ? AND symbol = ?",
                   int(shares), userId, symbol)
        price = lookup(symbol)["price"]
        sharesValue = int(shares) * price
        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", sharesValue, userId)

        current_date = datetime.date.today()
        current_time = datetime.datetime.now().time()
        formatted_date = current_date.strftime('%Y-%m-%d')
        formatted_time = current_time.strftime('%H:%M:%S')

        db.execute("INSERT INTO transactions (user_id, symbol, shares, price,transaction_type,transaction_date,transaction_time) VALUES (?, ?, ?, ?, ?, ?, ?)",
                   userId, symbol, int(shares), sharesValue, "sell", formatted_date, formatted_time)
        return redirect("/")


@app.route("/addCash", methods=["GET", "POST"])
@login_required
def addCash():
    userId = session["user_id"]
    if request.method == "GET":
        return render_template("add-cash.html")
    else:
        cash = request.form.get("cash")
        if not cash:
            return apology("Amount of cash cannot be blank")
        if not is_positive_integer(cash):
            return apology("Amount of cash must be a positive integer")
        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", cash, userId)
        return redirect("/")
