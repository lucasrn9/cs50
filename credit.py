credit_card_number = int(input("Number: "))
credit_card_string = str(credit_card_number)
credit_card_number_length = len(credit_card_string)

if credit_card_number_length < 13 or credit_card_number_length > 16 or credit_card_number_length == 14:
    print("INVALID")
    exit()

counter = credit_card_number_length
first_sum = 0
second_sum = 0
add_to_first = False
for c in range(credit_card_number_length - 1, -1, -1):
    if add_to_first:
        multiplication = int(credit_card_string[c]) * 2
        multiplication_string = str(multiplication)
        for digit in multiplication_string:
            first_sum += int(digit)
        add_to_first = False
    else:
        second_sum += int(credit_card_string[c])
        add_to_first = True
    counter -= 1

result = first_sum + second_sum
result_string = str(result)
result_last_digit = int(result_string[-1])
is_result_valid = result_last_digit == 0

if is_result_valid:
    first_digit = int(credit_card_string[0])
    second_digit = int(credit_card_string[1])
    first_two_digits = first_digit * 10 + second_digit

    if credit_card_number_length == 13 or credit_card_number_length == 16:
        if first_digit == 4:
            print("Number:", credit_card_number)
            print("VISA")
        elif 50 < first_two_digits < 56:
            print("Number:", credit_card_number)
            print("MASTERCARD")
        else:
            print("INVALID")
    elif credit_card_number_length == 15:
        if first_two_digits == 34 or first_two_digits == 37:
            print("Number:", credit_card_number)
            print("AMEX")
        else:
            print("INVALID")
else:
    print("INVALID")
