from cs50 import SQL
from sys import argv, exit
import csv

if len(argv) != 2:
    print("Usage: python import.py charaters.csv")
    exit()

db = SQL("sqlite:///students.db")


csvfile_name = argv[1]

with open(csvfile_name) as csvfile:
    csv_reader = csv.DictReader(csvfile)
    for row in csv_reader:
        name = row['name'].split(' ')
        if len(name) == 3:
            db.execute('INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)',
                       name[0], name[1], name[2], row['house'], row['birth'])
        elif len(name) == 2:
            db.execute('INSERT INTO students (first, middle, last, house, birth) VALUES(?, NULL, ?, ?, ?)',
                       name[0], name[1], row['house'], row['birth'])