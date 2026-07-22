#include <iostream>
#include <cstring>
#include <cstdlib>
#include "Book.h"
#include "Library.h"
using namespace std;

const char* DATA_FILE = "data/library.dat";
const char* CSV_FILE = "data/library.csv";
const char* JSON_FILE = "data/library.json";

void displayMainMenu() {
    cout << "+==========================================================+" << endl;
    cout << "|         Personal Library Management System - v1.0         |" << endl;
    cout << "+==========================================================+" << endl;
    cout << "|                                                          |" << endl;
    cout << "|   1. Add new book                                        |" << endl;
    cout << "|   2. Remove book by ISBN                                 |" << endl;
    cout << "|   3. Edit book by ISBN                                   |" << endl;
    cout << "|   4. Search books                                        |" << endl;
    cout << "|   5. List all books                                      |" << endl;
    cout << "|   6. Sort books                                          |" << endl;
    cout << "|   7. Save data (Binary/CSV/JSON)                         |" << endl;
    cout << "|   8. Load data (Binary/CSV/JSON)                         |" << endl;
    cout << "|   0. Exit                                                |" << endl;
    cout << "|                                                          |" << endl;
    cout << "+==========================================================+" << endl;
}

void readNonEmptyString(char* buffer, int size, const char* prompt, const char* fieldName) {
    while (true) {
        cout << prompt;
        cin.getline(buffer, size);
        if (strlen(buffer) == 0) {
            cout << "Error: " << fieldName << " cannot be empty. Try again." << endl;
        } else {
            break;
        }
    }
}

void readValidISBN(char* buffer, int size, const char* prompt) {
    while (true) {
        readNonEmptyString(buffer, size, prompt, "ISBN");

        int digitCount = 0;
        bool valid = true;
        for (int i = 0; buffer[i] != '\0'; i++) {
            if (buffer[i] >= '0' && buffer[i] <= '9') {
                digitCount++;
            } else if (buffer[i] != '-') {
                valid = false;
                break;
            }
        }

        if (valid && (digitCount == 10 || digitCount == 13)) {
            break;
        }
        cout << "Error: ISBN must have 10 or 13 digits. Try again." << endl;
    }
}

int readValidYear() {
    int year;
    while (true) {
        cout << "Publication year (1450-2026): ";
        cin >> year;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Error: Invalid input. Enter a number." << endl;
        } else if (year < 1450 || year > 2026) {
            cout << "Error: Year must be between 1450 and 2026." << endl;
            cin.ignore(10000, '\n');
        } else {
            cin.ignore(10000, '\n');
            return year;
        }
    }
}

int readIntInRange(const char* prompt, int minVal, int maxVal) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Error: Invalid input. Enter a number." << endl;
        } else if (value < minVal || value > maxVal) {
            cout << "Error: Value must be between " << minVal << " and " << maxVal << "." << endl;
            cin.ignore(10000, '\n');
        } else {
            cin.ignore(10000, '\n');
            return value;
        }
    }
}

bool readYesNo(const char* prompt) {
    char choice;
    while (true) {
        cout << prompt;
        cin >> choice;
        cin.ignore(10000, '\n');
        if (choice == 'y' || choice == 'Y') return true;
        if (choice == 'n' || choice == 'N') return false;
        cout << "Error: Please enter 'y' or 'n'." << endl;
    }
}

void handleAddBook(Library& library) {
    cout << endl << "=== Add New Book ===" << endl;

    char title[200];
    char author[200];
    char isbn[50];
    int year;

    readNonEmptyString(title, 200, "Book title: ", "title");
    readNonEmptyString(author, 200, "Author name: ", "author");
    readValidISBN(isbn, 50, "ISBN (10 or 13 digits): ");
    year = readValidYear();

    try {
        Book book(title, author, isbn, year);
        if (library.addBook(book)) {
            cout << endl << "Book added successfully:" << endl;
            book.print();
        }
    } catch (const char* msg) {
        cout << "Error: " << msg << endl;
    }
}

void handleRemoveBook(Library& library) {
    cout << endl << "=== Remove Book ===" << endl;

    if (library.empty()) {
        cout << "Library is empty." << endl;
        return;
    }

    char isbn[50];
    readValidISBN(isbn, 50, "Enter ISBN of the book to remove: ");

    Vector<Book> results = library.searchByISBN(isbn);
    if (results.isEmpty()) {
        cout << "No book found with this ISBN." << endl;
        return;
    }

    cout << endl << "Book found:" << endl;
    results.get(0).print();

    if (readYesNo("Are you sure you want to remove this book? (y/n): ")) {
        if (library.removeBook(isbn)) {
            cout << "Book removed successfully." << endl;
        }
    } else {
        cout << "Operation cancelled." << endl;
    }
}

void handleEditBook(Library& library) {
    cout << endl << "=== Edit Book ===" << endl;

    if (library.empty()) {
        cout << "Library is empty." << endl;
        return;
    }

    char isbn[50];
    readValidISBN(isbn, 50, "Enter ISBN of the book to edit: ");

    Vector<Book> results = library.searchByISBN(isbn);
    if (results.isEmpty()) {
        cout << "No book found with this ISBN." << endl;
        return;
    }

    cout << endl << "Current book:" << endl;
    results.get(0).print();
    cout << endl << "Press Enter to keep the current value." << endl << endl;

    char newTitle[200];
    char newAuthor[200];
    int newYear;

    cout << "New title (Enter to keep): ";
    cin.getline(newTitle, 200);

    cout << "New author (Enter to keep): ";
    cin.getline(newAuthor, 200);

    cout << "New publication year (0 to keep): ";
    cin >> newYear;
    cin.ignore(10000, '\n');

    try {
        if (library.editBook(isbn, newTitle, newAuthor, newYear)) {
            cout << endl << "Book updated successfully." << endl;
            Vector<Book> updated = library.searchByISBN(isbn);
            if (!updated.isEmpty()) {
                updated.get(0).print();
            }
        }
    } catch (const char* msg) {
        cout << "Error: " << msg << endl;
    }
}

void handleSearchBooks(Library& library) {
    cout << endl << "=== Search Books ===" << endl;
    cout << "1. Search by title" << endl;
    cout << "2. Search by author" << endl;
    cout << "3. Search by ISBN" << endl;

    int choice = readIntInRange("Choice: ", 1, 3);

    Vector<Book> results;

    if (choice == 1) {
        char title[200];
        cout << "Title (or part of it): ";
        cin.getline(title, 200);
        results = library.searchByTitle(title);
    } else if (choice == 2) {
        char author[200];
        cout << "Author (or part of name): ";
        cin.getline(author, 200);
        results = library.searchByAuthor(author);
    } else {
        char isbn[50];
        readValidISBN(isbn, 50, "ISBN: ");
        results = library.searchByISBN(isbn);
    }

    if (results.isEmpty()) {
        cout << "No books found." << endl;
    } else {
        cout << endl << "Found " << results.len() << " book(s):" << endl;
        for (int i = 0; i < results.len(); i++) {
            results.get(i).print();
        }
    }
}

void handleDisplayAll(Library& library) {
    library.displayAll();
}

void handleSortBooks(Library& library) {
    cout << endl << "=== Sort Books ===" << endl;
    cout << "1. By title" << endl;
    cout << "2. By author" << endl;
    cout << "3. By ISBN" << endl;
    cout << "4. By publication year" << endl;

    int fieldChoice = readIntInRange("Field choice: ", 1, 4);
    int field = fieldChoice - 1;

    cout << "1. Ascending (A -> Z)" << endl;
    cout << "2. Descending (Z -> A)" << endl;
    int orderChoice = readIntInRange("Order choice: ", 1, 2);
    int order = (orderChoice == 2) ? 1 : 0;

    library.sortBy(field, order);
    cout << "Books sorted successfully." << endl;
    library.displayAll();
}

void handleSaveMenu(Library& library) {
    cout << endl << "=== Save Data ===" << endl;

    if (library.empty()) {
        cout << "Library is empty. Nothing to save." << endl;
        return;
    }

    cout << "Select format:" << endl;
    cout << "  1. Binary (.dat) - compact, fast, native format" << endl;
    cout << "  2. CSV (.csv)    - opens in Excel, human-readable" << endl;
    cout << "  3. JSON (.json)  - web/API standard, structured" << endl;

    int formatChoice = readIntInRange("Choice: ", 1, 3);

    bool success = false;
    if (formatChoice == 1) {
        success = library.saveToBinaryFile(DATA_FILE);
        if (success) cout << library.size() << " book(s) saved to binary file." << endl;
    } else if (formatChoice == 2) {
        success = library.saveToCSVFile(CSV_FILE);
        if (success) cout << library.size() << " book(s) saved to CSV file: " << CSV_FILE << endl;
    } else {
        success = library.saveToJSONFile(JSON_FILE);
        if (success) cout << library.size() << " book(s) saved to JSON file: " << JSON_FILE << endl;
    }
}

void handleLoadMenu(Library& library) {
    cout << endl << "=== Load Data ===" << endl;

    if (!readYesNo("Warning: This will replace current books. Continue? (y/n): ")) {
        cout << "Operation cancelled." << endl;
        return;
    }

    cout << "Select format:" << endl;
    cout << "  1. Binary (.dat)" << endl;
    cout << "  2. CSV (.csv)" << endl;
    cout << "  3. JSON (.json)" << endl;

    int formatChoice = readIntInRange("Choice: ", 1, 3);

    bool success = false;
    if (formatChoice == 1) {
        success = library.loadFromBinaryFile(DATA_FILE);
        if (success) cout << library.size() << " book(s) loaded from binary file." << endl;
    } else if (formatChoice == 2) {
        success = library.loadFromCSVFile(CSV_FILE);
        if (success) cout << library.size() << " book(s) loaded from CSV file." << endl;
    } else {
        success = library.loadFromJSONFile(JSON_FILE);
        if (success) cout << library.size() << " book(s) loaded from JSON file." << endl;
    }

    if (!success) {
        cout << "Error: Could not load file. File may not exist." << endl;
    }
}

int main() {
    Library library;

    if (library.loadFromBinaryFile(DATA_FILE)) {
        if (library.size() > 0) {
            cout << "Loaded " << library.size() << " book(s) from file." << endl;
        }
    } else {
        cout << "No data file found. It will be created on first save." << endl;
    }

    while (true) {
        system("cls");
        displayMainMenu();
        cout << "Total books: " << library.size() << endl;
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Error: Invalid input." << endl;
            system("pause");
            continue;
        }
        cin.ignore(10000, '\n');

        if (choice == 0) {
            if (!library.empty()) {
                if (readYesNo("Do you want to save changes before exiting? (y/n): ")) {
                    cout << "Saving data..." << endl;
                    library.saveToBinaryFile(DATA_FILE);
                    cout << "Data saved successfully." << endl;
                }
            }
            cout << "Goodbye!" << endl;
            break;
        }

        switch (choice) {
            case 1: {
                handleAddBook(library);
                break;
            }
            case 2: {
                handleRemoveBook(library);
                break;
            }
            case 3: {
                handleEditBook(library);
                break;
            }
            case 4: {
                handleSearchBooks(library);
                break;
            }
            case 5: {
                handleDisplayAll(library);
                break;
            }
            case 6: {
                handleSortBooks(library);
                break;
            }
            case 7: {
                handleSaveMenu(library);
                break;
            }
            case 8: {
                handleLoadMenu(library);
                break;
            }
            default: {
                cout << "Error: Invalid option. Please enter 0-8." << endl;
                break;
            }
        }

        system("pause");
    }

    return 0;
}
