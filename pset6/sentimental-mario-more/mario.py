def main():
    height = get_height()

    for i in range(height):
        grids = i + 1
        spaces = height - grids
        print(" " * spaces + "#" * grids + "  " + "#" * grids)


def get_height():
    while True:
        try:
            height = int(input("Height: "))
            if height >= 1 and height <= 8:
                break
        except ValueError:
            continue

    return height


main()