from cs50 import get_int
# mario from 1 to 8
while (True):
    height = get_int("Height: ")
    if height <= 8 and height >= 1:
        break

spaces = height - 1

for i in range(height):
    for j in range(spaces):
        print(" ", end="")
    spaces -= 1
    for j in range(i + 1):
        print("#", end="")
    print("  ", end="")
    for j in range(i+1):
        print("#", end="")

    print()
