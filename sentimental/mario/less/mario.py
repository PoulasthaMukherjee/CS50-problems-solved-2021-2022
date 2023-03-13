from cs50 import get_int
# Promp for height
while True:
    h = get_int("Height: ")
    if h > 0 and h < 9:
        break
# Iterate through height
for i in range(h):
    print((h - i - 1) * " ", end="")
    print((i + 1) * "#")