import csv
import sys


def main():

    # TODO: Check for command-line usage
    databasePath = sys.argv[1]
    dnaSequencePath = sys.argv[2]
    # TODO: Read database file into a variable
    rows = []
    strs = []
    with open(databasePath) as file:
        readers = csv.DictReader(file)
        strs = readers.fieldnames[1:]
        for row in readers:
            rows.append(row)
    # TODO: Read DNA sequence file into a variable
    dnaSequence = ''
    with open(dnaSequencePath) as dnaFile:
        dnaSequence = dnaFile.read(50000)
        if dnaFile.read(1):
            print("Dna too long")
            return
    # TODO: Find longest match of each STR in DNA sequence
    strsMatches = {}
    for str in strs:
        strsMatches[str] = longest_match(dnaSequence, str)

    # TODO: Check database for matching profiles
    for row in rows:
        matches = True
        for str in strs:
            if int(row[str]) != int(strsMatches[str]):
                matches = False
                break
        if matches:
            print(row['name'])
            return

    print("No match")


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
