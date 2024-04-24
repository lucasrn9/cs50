from cs50 import get_int

height = get_int("Height: ")

while height < 1 or height > 8:
    height = get_int("Height: ")

for i in range(height):
    # left spaces
    for _ in range(height-i-1):
        print(" ", end="")
    # left hashes
    for _ in range(i+1):
        print("#", end="")
    # mid space
    print("  ", end="")
    # right hashes
    for _ in range(i+1):
        print("#", end="")
    print("")
