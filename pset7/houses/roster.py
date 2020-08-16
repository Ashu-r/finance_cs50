from cs50 import SQL
from sys import argv, exit

if len(argv) != 2:
    print("Usage: python roster.py name_of_the_house")
    exit()

db = SQL("sqlite:///students.db")

house = argv[1]

data = db.execute('SELECT * FROM students WHERE house = ? ORDER BY last ASC, first ASC', house)

for student in data:
    if student['middle'] == None:
        print(f"{student['first']} {student['last']}, born {student['birth']}")
    else:
        print(f"{student['first']} {student['middle']} {student['last']}, born {student['birth']}")
