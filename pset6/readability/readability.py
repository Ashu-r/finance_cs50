from cs50 import get_string


text = get_string("Text: ")

words = text.split(" ")

total_letters = 0
sentenses = 0

for letter in text:
    if letter.isalpha():
        total_letters += 1
    if letter in ['.', '!', '?']:
        sentenses += 1
total_words = len(words)

# print(f"total_letters{total_letters} sente{sentenses} total_word{total_words}")
L = 100 * total_letters / total_words
S = 100 * sentenses / total_words

index = 0.0588 * L - 0.296 * S - 15.8

if index <= 1:
    print("Before Grade 1")
elif index >= 16:
    print("Grade 16+")
else:
    print(f"Grade {round(index)}")