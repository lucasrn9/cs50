from cs50 import SQL
from flask import Flask, redirect, render_template, request, session, send_file
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
import datetime
import re
import io
import os

from helpers import apology, login_required, allowed_file

app = Flask(__name__)

app.config["TEMPLATES_AUTO_RELOAD"] = True
app.config["SEND_FILE_MAX_AGE_DEFAULT"] = 0

# max size of files to be uploaded to the server
app.config['MAX_CONTENT_LENGTH'] = 1 * 1024 * 1024  # 1 MB limit
max_content_length_string = "1MB"

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)
# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///blog.db")

@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

def handle_request_entity_too_large(e):
    return apology(f"File too big, the max size is {max_content_length_string}",413)

app.register_error_handler(413, handle_request_entity_too_large)

@app.route("/")
@login_required
def index():
    posts = db.execute("""SELECT articles.id,title,description,preview_img,publish_date,username,count(post_id) as likes_count FROM articles 
    JOIN users ON articles.author_id = users.id
    LEFT JOIN likes ON articles.id = likes.post_id
    GROUP BY articles.id
    ORDER BY publish_date DESC, likes_count DESC
    LIMIT 10;""")
    return render_template("index.html",posts=posts)

@app.route("/new-post", methods=["GET","POST"])
@login_required
def newPost():
    if request.method == "GET":
        return render_template("new-post.html")
    else:
        authorId = session["user_id"]
        previewImg = request.form.get("previewImg")
        articleImg = request.form.get("articleImg")
        title = request.form.get("title")
        content = request.form.get("content")
        description = request.form.get("description")
        validImgpattern = r'^(https?:\/\/)(?:www\.)?[a-zA-Z0-9-_.]+(?:\.[a-zA-Z]{2,})+(?:\/[^(){}\s]+(?:\/[^(){}\s]+)*)*$'
        previewImgValid = re.match(validImgpattern, previewImg)
        articleImgValid = re.match(validImgpattern,articleImg)
        current_datetime = datetime.datetime.now()
        date = current_datetime.strftime('%Y-%m-%d %H:%M:%S')
        if not title or not content:
            return apology("Fields title and content cannot be blank")
        if len(title) > 100:
            return apology("The post title should be at max 100 characteres")
        if not description or len(description) > 200:
            return apology("Description cannot be neither blank or have more than 200 characters")
        if not previewImg or not articleImg:
            return apology("Images are cool! Please add a preview and an article image to your post")
        if not previewImgValid or not articleImgValid:
            return apology("Ops! Looks like you have provided links that we do not support for one of your images")
        if not authorId or not date:
            return apology("Internal error: #1")
        db.execute("INSERT INTO articles (author_id, title,description, content, preview_img, article_img, publish_date) VALUES (?,?,?,?,?,?,?)",authorId,title,description,content,previewImg,articleImg,date)
        return redirect("/your-posts")

@app.route("/your-posts")
@login_required
def yourPosts():
    userId = session["user_id"]
    posts = db.execute("""SELECT articles.id,title,description,preview_img,publish_date,username FROM articles 
    JOIN users ON articles.author_id = users.id
    WHERE users.id = ?
    GROUP BY articles.id
    ORDER BY publish_date DESC""",userId)
    return render_template("your-posts.html",posts=posts)

@app.route('/profile/<username>',methods=["GET","POST"])
@login_required
def profile(username):
    if request.method == "GET":
        userId = session["user_id"]
        user = db.execute("SELECT id,username FROM users WHERE username = ?", username)[0]
        articles = {}
        quantity = db.execute("SELECT count(id) as quantity FROM articles WHERE author_id = ?",user["id"])[0]["quantity"]
        articles["quantity"] = quantity
        lastPostDate = db.execute("SELECT publish_date FROM articles WHERE author_id = ? ORDER BY publish_date DESC LIMIT 1",user["id"])
        if not lastPostDate:
            articles["lastPostDate"] = "no posts yet"
        else:
            articles["lastPostDate"] = lastPostDate[0]["publish_date"]
            likes = db.execute("SELECT COUNT(articles.author_id) as likes FROM likes JOIN articles ON likes.post_id = articles.id WHERE articles.author_id = ?;",userId)[0]["likes"]
            articles["likes"] = likes
        return render_template("profile.html",profile=user,articles=articles)
    else:
        if 'profilePicture' not in request.files:
            return "",400
        profilePicture = request.files['profilePicture']
        if profilePicture.filename == '':
            return "",400
        if not allowed_file(profilePicture.filename):
            return apology("Unsuported image file, please select an image in one of the following formarts: 'png', 'jpg', 'jpeg', 'webp'")
        if profilePicture:
            profile_picture_data = profilePicture.read()
            print(profile_picture_data)
            userId = session["user_id"]
            db.execute("UPDATE users SET profile_picture = ? WHERE id = ?",profile_picture_data,userId)
            return redirect(f"/profile/{username}")
        return "",400

@app.route('/profile-picture/<username>')
@login_required
def profilePicture(username):
    imageQuery = db.execute("SELECT profile_picture FROM users WHERE username = ?",username)
    imageRaw = imageQuery[0]["profile_picture"] if imageQuery else None
    if imageRaw:
        return send_file(io.BytesIO(imageRaw), mimetype='image/jpeg')
    else:
        return send_file(os.path.abspath("./static/default-pfp.jpg"))

@app.route('/post/<id>',methods=["GET","POST"])
@login_required
def post(id):
    userId = session["user_id"]
    post = db.execute("SELECT id,author_id,title,description,content,preview_img,article_img,publish_date FROM articles WHERE id = ?",id)
    liked = True if db.execute("SELECT * FROM likes WHERE post_id = ? AND user_id = ?",id,userId) else False
    if request.method == "GET":
        if not post:
            return apology("Post not found")
        author = db.execute("SELECT username FROM users WHERE id = ?",post[0]["author_id"])
        return render_template("post.html",post=post[0],author=author[0],liked=liked)
    else:
        if not liked:
            db.execute("INSERT INTO likes(post_id,user_id) VALUES(?,?)",id,userId)
            # liked = True
        else:
            db.execute("DELETE FROM likes WHERE post_id = ? AND user_id = ?",id,userId)
            # liked = False
        return redirect(f'/post/{id}')

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
        session["username"] = rows[0]["username"]

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
    
