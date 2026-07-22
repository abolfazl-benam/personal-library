#ifndef BOOK_H
#define BOOK_H

#include <iostream>
#include <cstring>
using namespace std;

const int MAX_TITLE_LEN = 200;
const int MAX_AUTHOR_LEN = 100;
const int MAX_ISBN_LEN = 20;

class Book {
private:
    char title[MAX_TITLE_LEN];
    char author[MAX_AUTHOR_LEN];
    char isbn[MAX_ISBN_LEN];
    int publicationYear;

public:
    Book();
    Book(const char* title, const char* author, const char* isbn, int year);

    const char* getTitle() const;
    const char* getAuthor() const;
    const char* getISBN() const;
    int getPublicationYear() const;

    void setTitle(const char* title);
    void setAuthor(const char* author);
    void setISBN(const char* isbn);
    void setPublicationYear(int year);

    void print() const;

    friend ostream& operator<<(ostream& out, const Book& book);
    friend istream& operator>>(istream& in, Book& book);
};

#endif
