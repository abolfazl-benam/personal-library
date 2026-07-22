#ifndef LIBRARY_H
#define LIBRARY_H

#include "Book.h"
#include "Vector.h"
using namespace std;

class Library {
private:
    Vector<Book> books;

    int findByISBN(const char* isbn) const;

public:
    Library();
    ~Library();

    bool addBook(const Book& book);
    bool editBook(const char* isbn, const char* newTitle, const char* newAuthor, int newYear);
    bool removeBook(const char* isbn);

    Vector<Book> searchByTitle(const char* title) const;
    Vector<Book> searchByAuthor(const char* author) const;
    Vector<Book> searchByISBN(const char* isbn) const;

    void sortBy(int field, int order);
    void displayAll() const;

    bool saveToBinaryFile(const char* filename) const;
    bool loadFromBinaryFile(const char* filename);

    bool saveToCSVFile(const char* filename) const;
    bool loadFromCSVFile(const char* filename);

    bool saveToJSONFile(const char* filename) const;
    bool loadFromJSONFile(const char* filename);

    int size() const;
    bool empty() const;
};

#endif
