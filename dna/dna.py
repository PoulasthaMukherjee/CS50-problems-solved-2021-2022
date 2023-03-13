from csv import reader, DictReader
from sys import argv, exit
# check length
if len(argv) < 3:
    print("Usage: python dna.py data.csv sequence.txt")
    exit()
# read the dna sequence from the file
with open(argv[2]) as DNAfile:
    DNAreader = reader(DNAfile)
    for row in DNAreader:
        DNAlist = row
# store in string
DNA = DNAlist[0]
# create a dictionary
sequences = {}
with open(argv[1]) as peoplefile:
    people = reader(peoplefile)
    for row in people:
        DNASequences = row
        DNASequences.pop(0)
        break
    for item in DNASequences:
        sequences[item] = 1
# if repetitions of the values from sequence dictionary are found, count
for key in sequences:
    l = len(key)
    tempMax = 0
    temp = 0
    for i in range(len(DNA)):
        # after having counted a sequence
        # skip at the end of it to avoid counting again
        while temp > 0:
            temp -= 1
            continue
        # if the segment of dna corresponds to the key
        if DNA[i: i + l] == key:
            # there is a repetition of it
            while DNA[i - l: i] == DNA[i: i + l]:
                # increment counter
                temp += 1
                i += l
            # compare the value to the previous longest sequence
            if temp > tempMax:
                # if it is longer it becomes the new max
                tempMax = temp
    # store the longest sequences in the dictionary using the correspondent key
    sequences[key] += tempMax

with open(argv[1], newline='') as peoplefile:
    people = DictReader(peoplefile)
    for person in people:
        match = 0
        # compares the sequences to every person and prints name
        for DNA in sequences:
            if sequences[DNA] == int(person[DNA]):
                match += 1
        # leave the program if there is a match
        if match == len(sequences):
            print(person['name'])
            exit()
    # otherwise, no match
    print("No match")