#include "Book.h"
#include <iostream>
#include <cstring>
using namespace std;

Book::Book() {
    title[0] = '\0';
    author[0] = '\0';
    isbn[0] = '\0';
    publicationYear = 0;
}

Book::Book(const char* t, const char* a, const char* i, int year) {
    setTitle(t);
    setAuthor(a);
    setISBN(i);
    setPublicationYear(year);
}

const char* Book::getTitle() const {
    return title;
}

const char* Book::getAuthor() const {
    return author;
}

const char* Book::getISBN() const {
    return isbn;
}

int Book::getPublicationYear() const {
    return publicationYear;
}

void Book::setTitle(const char* t) {
    if (t == NULL || strlen(t) == 0) {
        throw "Title cannot be empty";
    }
    if (strlen(t) >= MAX_TITLE_LEN) {
        throw "Title is too long";
    }
    strcpy(title, t);
}

void Book::setAuthor(const char* a) {
    if (a == NULL || strlen(a) == 0) {
        throw "Author cannot be empty";
    }
    if (strlen(a) >= MAX_AUTHOR_LEN) {
        throw "Author name is too long";
    }
    strcpy(author, a);
}

void Book::setISBN(const char* i) {
    if (i == NULL || strlen(i) == 0) {
        throw "ISBN cannot be empty";
    }

    int digitCount = 0;
    for (int k = 0; i[k] != '\0'; k++) {
        if (i[k] >= '0' && i[k] <= '9') {
            digitCount++;
        } else if (i[k] != '-') {
            throw "ISBN contains invalid characters";
        }
    }

    if (digitCount != 10 && digitCount != 13) {
        throw "ISBN must have 10 or 13 digits";
    }

    int j = 0;
    for (int k = 0; i[k] != '\0'; k++) {
        if (i[k] != '-') {
            isbn[j] = i[k];
            j++;
        }
    }
    isbn[j] = '\0';
}

void Book::setPublicationYear(int year) {
    if (year < 1450 || year > 2026) {
        throw "Year must be between 1450 and 2026";
    }
    publicationYear = year;
}

void Book::print() const {
    cout << "+----------------------------------------------+" << endl;
    cout << "|  Title:    " << title << endl;
    cout << "|  Author:   " << author << endl;
    cout << "|  ISBN:     " << isbn << endl;
    cout << "|  Year:     " << publicationYear << endl;
    cout << "+----------------------------------------------+" << endl;
}

ostream& operator<<(ostream& out, const Book& book) {
    out << "Title: " << book.title
        << " | Author: " << book.author
        << " | ISBN: " << book.isbn
        << " | Year: " << book.publicationYear;
    return out;
}

istream& operator>>(istream& in, Book& book) {
    char title[MAX_TITLE_LEN];
    char author[MAX_AUTHOR_LEN];
    char isbn[MAX_ISBN_LEN];
    int year;

    cout << "Title: ";
    in.getline(title, MAX_TITLE_LEN);
    cout << "Author: ";
    in.getline(author, MAX_AUTHOR_LEN);
    cout << "ISBN: ";
    in.getline(isbn, MAX_ISBN_LEN);
    cout << "Publication year: ";
    in >> year;
    in.ignore();

    book = Book(title, author, isbn, year);
    return in;
}
