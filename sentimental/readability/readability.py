from cs50 import get_string
# Prompt for Text
txt = get_string("Text: ")
# Define total length of Text, total letters, spaces and sentences
l = len(txt)
letters = 0
spaces = 0
sentences = 0
# Calculate total letters, spaces and sentences in Text
for i in range(l):
    # Take all upper-case and lower-case chars as letters
    if txt[i].islower():
        letters += 1
    elif txt[i].isupper():
        letters += 1
    # Count spaces
    elif txt[i] == " ":
        spaces += 1
    # Search for different periods to calculate total sentences
    elif txt[i] == "." or txt[i] == "!" or txt[i] == "?":
        sentences += 1
# Calculate total words in Text
words = spaces + 1
# Calculate the Average number of letters per 100 words in Text
L = (letters * 100) / words
# Calculate the Average number of sentences per 100 words in Text
S = (sentences * 100) / words
# Calculate the The Coleman-Liau Index of the Text and ROUND IT TO THE NEAREST WHOLE NUMBER
index = round(0.0588 * L - 0.296 * S - 15.8)
# Different conditions of index / Grade
if index < 1:
    print("Before Grade 1")
elif index >= 16:
    print("Grade 16+")
else:
    print("Grade " + str(index))