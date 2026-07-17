#ifndef BOOK_EXCEPTIONS_H
#define BOOK_EXCEPTIONS_H

#include <stdexcept>
#include <string>

class BookException : public std::runtime_error {
public:
    explicit BookException(const std::string& message)
        : std::runtime_error(message) {}
};

class InvalidISBNException : public BookException {
public:
    explicit InvalidISBNException(const std::string& isbn)
        : BookException("Invalid ISBN: '" + isbn +
                        "'. ISBN must have 10 or 13 digits.") {}
};

class InvalidYearException : public BookException {
public:
    explicit InvalidYearException(int year)
        : BookException("Invalid publication year: " + std::to_string(year) +
                        ". Year must be between 1450 and 2026.") {}
};

class EmptyFieldException : public BookException {
public:
    explicit EmptyFieldException(const std::string& fieldName)
        : BookException("Field '" + fieldName + "' cannot be empty.") {}
};

class BookNotFoundException : public BookException {
public:
    explicit BookNotFoundException(const std::string& isbn)
        : BookException("Book with ISBN '" + isbn + "' not found.") {}
};

class DuplicateISBNException : public BookException {
public:
    explicit DuplicateISBNException(const std::string& isbn)
        : BookException("Book with ISBN '" + isbn +
                        "' already exists. ISBN must be unique.") {}
};

class FileOperationException : public BookException {
public:
    explicit FileOperationException(const std::string& operation,
                                     const std::string& filename)
        : BookException("Error during " + operation +
                        " operation on file '" + filename + "'.") {}
};

#endif
