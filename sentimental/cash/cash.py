from cs50 import get_float
# Prompt for non-negative change in dollars
while True:
    dollars = get_float("Change owed: ")
    if dollars >= 0:
        break
# Convert the dollars into cents and round them to the nearest penny
cents = round(dollars * 100)
# Define number of coins
coins = 0
# Check for max possible values of coins
while cents >= 25:
    cents -= 25
    coins += 1
while cents >= 10:
    cents -= 10
    coins += 1
while cents >= 5:
    cents -= 5
    coins += 1
while cents >= 1:
    cents -= 1
    coins += 1
# Print the optimal number of coins to be given as change
print(coins)