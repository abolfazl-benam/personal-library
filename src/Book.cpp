#include "Book.h"
#include "Utils.h"
#include <iomanip>

Book::Book()
    : title(""), author(""), isbn(""), publicationYear(0) {}

Book::Book(const std::string& title,
           const std::string& author,
           const std::string& isbn,
           int publicationYear)
    : publicationYear(0) {
    setTitle(title);
    setAuthor(author);
    setISBN(isbn);
    setPublicationYear(publicationYear);
}

std::string Book::getTitle() const { return title; }
std::string Book::getAuthor() const { return author; }
std::string Book::getISBN() const { return isbn; }
int Book::getPublicationYear() const { return publicationYear; }

void Book::setTitle(const std::string& title) {
    if (Utils::isEmptyOrWhitespace(title)) {
        throw EmptyFieldException("title");
    }
    this->title = title;
}

void Book::setAuthor(const std::string& author) {
    if (Utils::isEmptyOrWhitespace(author)) {
        throw EmptyFieldException("author");
    }
    this->author = author;
}

void Book::setISBN(const std::string& isbn) {
    std::string normalized = Utils::normalizeISBN(isbn);
    if (!Utils::isValidISBN(normalized)) {
        throw InvalidISBNException(isbn);
    }
    this->isbn = normalized;
}

void Book::setPublicationYear(int year) {
    if (!Utils::isValidYear(year)) {
        throw InvalidYearException(year);
    }
    this->publicationYear = year;
}

void Book::display() const {
    std::cout << "+----------------------------------------------+\n";
    std::cout << "|  Title:    " << title << "\n";
    std::cout << "|  Author:   " << author << "\n";
    std::cout << "|  ISBN:     " << isbn << "\n";
    std::cout << "|  Year:     " << publicationYear << "\n";
    std::cout << "+----------------------------------------------+\n";
}

std::string Book::toString() const {
    return "Title: " + title +
           " | Author: " + author +
           " | ISBN: " + isbn +
           " | Year: " + std::to_string(publicationYear);
}

bool Book::operator==(const Book& other) const {
    return isbn == other.isbn;
}

std::ostream& operator<<(std::ostream& os, const Book& book) {
    os << "Title: " << book.title
       << " | Author: " << book.author
       << " | ISBN: " << book.isbn
       << " | Year: " << book.publicationYear;
    return os;
}

std::istream& operator>>(std::istream& is, Book& book) {
    std::string title, author, isbn;
    int year;

    std::cout << "Title: ";
    std::getline(is, title);
    std::cout << "Author: ";
    std::getline(is, author);
    std::cout << "ISBN: ";
    std::getline(is, isbn);
    std::cout << "Publication year: ";
    is >> year;
    is.ignore();

    book = Book(title, author, isbn, year);
    return is;
}
