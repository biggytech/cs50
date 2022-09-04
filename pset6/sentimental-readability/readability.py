import re


def main():
    text = get_text()
    grade = get_grade(text)

    if grade >= 16:
        print("Grade 16+")
    elif grade < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {grade}")


def get_text():
    text = input("Text: ")
    return text


def get_grade(text):
    stats = get_text_stats(text)
    return calculate_grade(stats["av_letters"], stats["av_sentences"])


def get_text_stats(text):
    text = text.lower()
    stats = {
        "av_letters": 0,
        "av_sentences": 0
    }
    letters = 0
    words = 1 if len(text) > 0 else 0
    sentences = 0

    for char in text:
        if re.compile("[a-z]").match(char):
            letters += 1
        elif re.compile(" ").match(char):
            words += 1
        elif re.compile("[.!?]").match(char):
            sentences += 1

    # print(f"Letters: {letters}, Words: {words}, Sentences: {sentences}")

    stats["av_letters"] = (letters / words) * 100
    stats["av_sentences"] = (sentences / words) * 100

    return stats


def calculate_grade(l, s):
    return round(0.0588 * l - 0.296 * s - 15.8)


main()