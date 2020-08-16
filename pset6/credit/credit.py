from cs50 import get_int, get_string


def main():
    number = get_int("Number: ")
    res = [int(x) for x in str(number)]

    if res[0] == 3 and (res[1] == 7 or res[1] == 4):
        card = "AMEX"
    elif res[0] == 5 and (res[1] in list(range(1, 6))):
        card = "MASTERCARD"
    elif res[0] == 4:
        card = "VISA"

    else:
        print("INVALID")
        return 1

    formula1 = []
    formula2 = []

    while (len(res) != 0):
        last = res.pop()
        formula2.append(last)
        if len(res) == 0:
            break
        second_last = res.pop()
        formula1.append(second_last)

    result = 0

    for item in formula1:
        if (item * 2) < 10:
            result += item * 2
        else:
            new = (item * 2) % 10
            result += new
            new = (item * 2)//10
            result += new

    for item in formula2:
        result += item

    last_digit = result % 10
    if last_digit == 0:
        print(card)
    else:
        print("INVALID")


main()