#ifndef LIBRARY_H
#define LIBRARY_H

#include <vector>
#include <string>
#include "Book.h"

class Library {
public:
    enum class SortField {
        TITLE,
        AUTHOR,
        ISBN,
        YEAR
    };

    enum class SortOrder {
        ASCENDING,
        DESCENDING
    };

private:
    std::vector<Book> books;

    std::vector<Book>::iterator findByISBN(const std::string& isbn);
    std::vector<Book>::const_iterator findByISBN(const std::string& isbn) const;

public:
    void addBook(const Book& book);
    void editBook(const std::string& isbn,
                  const std::string& newTitle = "",
                  const std::string& newAuthor = "",
                  int newYear = 0);
    void removeBook(const std::string& isbn);

    std::vector<Book> searchByTitle(const std::string& title) const;
    std::vector<Book> searchByAuthor(const std::string& author) const;
    std::vector<Book> searchByISBN(const std::string& isbn) const;

    void sortBy(SortField field, SortOrder order = SortOrder::ASCENDING);

    void displayAll() const;

    void saveToBinaryFile(const std::string& filename) const;
    void loadFromBinaryFile(const std::string& filename);

    void saveToCSVFile(const std::string& filename) const;
    void loadFromCSVFile(const std::string& filename);

    void saveToJSONFile(const std::string& filename) const;
    void loadFromJSONFile(const std::string& filename);

    size_t size() const;
    bool empty() const;
    const std::vector<Book>& getBooks() const;
};

#endif
