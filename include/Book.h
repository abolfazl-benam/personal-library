#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <iostream>
#include "BookExceptions.h"

class Book {
private:
    std::string title;
    std::string author;
    std::string isbn;
    int publicationYear;

public:
    Book();
    Book(const std::string& title,
         const std::string& author,
         const std::string& isbn,
         int publicationYear);

    std::string getTitle() const;
    std::string getAuthor() const;
    std::string getISBN() const;
    int getPublicationYear() const;

    void setTitle(const std::string& title);
    void setAuthor(const std::string& author);
    void setISBN(const std::string& isbn);
    void setPublicationYear(int year);

    void display() const;
    std::string toString() const;

    bool operator==(const Book& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Book& book);
    friend std::istream& operator>>(std::istream& is, Book& book);
};

#endif
