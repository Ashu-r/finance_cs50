import csv
import sys
import re

if len(sys.argv) != 3:
    print("Usage: python dna.py data.csv sequence.txt")
    sys.exit()
else:
    fields = []
    rows = []
    with open(sys.argv[1], "r") as data:
        csv_reader = csv.reader(data)
        fields = next(csv_reader)
        for row in csv_reader:
            rows.append(row)

    # print(rows,fields)
    max_occurences = []
    with open(sys.argv[2], "r") as dna:
        for line in dna:
            for seq in fields[1:]:
                # using re code inspired from https://www.geeksforgeeks.org/python-maximum-consecutive-substring-occurrence/
                res = max(re.findall('((?:' + re.escape(seq) + ')*)', line), key=len)
                max_occurences.append(str(int(len(res)/len(seq))))

    dna = "No match"
    for item in rows:
        if item[1:] == max_occurences:
            dna = item[0]

    print(dna)
