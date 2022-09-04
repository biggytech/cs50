import csv
from sys import argv


def main():

    # TODO: Check for command-line usage
    if not len(argv) == 3:
        print("Usage: python dna.py data.csv sequence.txt")
        exit(0)

    # TODO: Read DNA sequence file into a variable
    with open(argv[2]) as sequence_file:
        sequence = sequence_file.read()

    matched_name = False

    # TODO: Read database file into a variable
    with open(argv[1]) as database_file:
        reader = csv.DictReader(database_file)

        for row in reader:
            is_full_match = True

            for field in row:
                if not field == "name":
                    # TODO: Find longest match of each STR in DNA sequence
                    if not longest_match(sequence, field) == int(row[field]):
                        is_full_match = False
                        break

            # TODO: Check database for matching profiles
            if is_full_match:
                matched_name = row["name"]
                break

    if matched_name:
        print(matched_name)
    else:
        print("No match")

    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
