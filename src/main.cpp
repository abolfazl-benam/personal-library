/**
 * @file main.cpp
 * @brief برنامه اصلی سیستم مدیریت کتابخانه شخصی
 *
 * این فایل شامل منوی کنسولی و تابع main است.
 * مسئولیت‌ها:
 * - نمایش منوی اصلی
 * - دریافت ورودی از کاربر
 * - فراخوانی متدهای مناسب روی شیء Library
 * - مدیریت استثناها و نمایش پیام‌های خطا
 *
 * نکته: تمام پیام‌های نمایش داده شده به کاربر به زبان انگلیسی هستند
 * تا در تمام کنسول‌ها (به خصوص CMD ویندوز) بدون مشکل encoding نمایش داده شوند.
 * کامنت‌ها و توضیحات کد به زبان فارسی هستند.
 */

#include <iostream>
#include <string>
#include <memory>
#include <filesystem>
#include "Book.h"
#include "Library.h"
#include "Utils.h"
#include "BookExceptions.h"

// ============================================================
// ثابت‌های برنامه
// ============================================================

constexpr const char* DATA_FILE = "data/library.dat";
constexpr const char* CSV_FILE = "data/library.csv";
constexpr const char* JSON_FILE = "data/library.json";
constexpr const char* APP_NAME = "Personal Library Management System";
constexpr const char* APP_VERSION = "v2.0";

// ============================================================
// اعلان توابع منو
// ============================================================

void displayMainMenu();
void handleAddBook(Library& library);
void handleRemoveBook(Library& library);
void handleEditBook(Library& library);
void handleSearchBooks(const Library& library);
void handleDisplayAll(const Library& library);
void handleSortBooks(Library& library);
void handleSaveMenu(const Library& library);
void handleLoadMenu(Library& library);

// ============================================================
// تابع اصلی
// ============================================================

int main() {
    // نمایش مسیر فعلی اجرای برنامه برای دیباگ
    std::cout << "[i] Working directory: "
              << std::filesystem::current_path().string() << "\n";
    std::cout << "[i] Data file location: "
              << std::filesystem::absolute(DATA_FILE).string() << "\n\n";

    // ایجاد شیء کتابخانه
    Library library;

    // بارگذاری خودکار داده‌ها از فایل در شروع برنامه
    try {
        library.loadFromBinaryFile(DATA_FILE);
        if (!library.empty()) {
            std::cout << "[OK] Loaded " << library.size()
                      << " book(s) from file.\n";
        }
    } catch (const FileOperationException& e) {
        // اگر فایل وجود ندارد، پیام خطا نمایش نمی‌دهیم (طبیعی است)
        std::cout << "[i] No data file found. It will be created on first save.\n";
    } catch (const std::exception& e) {
        std::cerr << "[!] Error loading initial data: " << e.what() << "\n";
    }

    bool running = true;
    while (running) {
        Utils::clearScreen();
        std::cout << "+==========================================================+\n";
        std::cout << "|         " << APP_NAME << " - " << APP_VERSION << "         |\n";
        std::cout << "|         Total books: " << library.size() << "\n";
        std::cout << "+==========================================================+\n";
        displayMainMenu();
        std::cout << "+==========================================================+\n";
        std::cout << "Enter your choice: ";

        int choice;
        if (!(std::cin >> choice)) {
            // اگر به EOF رسیدیم (مثلا ورودی از فایل تمام شده)
            if (std::cin.eof()) {
                std::cout << "\n[BYE] Input ended. Goodbye!\n";
                break;
            }
            Utils::clearInputStream();
            std::cerr << "[X] Invalid input!\n";
            Utils::pauseScreen();
            continue;
        }
        Utils::clearInputStream();

        try {
            switch (choice) {
                case 1:
                    handleAddBook(library);
                    break;
                case 2:
                    handleRemoveBook(library);
                    break;
                case 3:
                    handleEditBook(library);
                    break;
                case 4:
                    handleSearchBooks(library);
                    break;
                case 5:
                    handleDisplayAll(library);
                    break;
                case 6:
                    handleSortBooks(library);
                    break;
                case 7:
                    handleSaveMenu(library);
                    break;
                case 8:
                    handleLoadMenu(library);
                    break;
                case 0:
                    // قبل از خروج، از کاربر می‌پرسیم که آیا می‌خواهد ذخیره کند
                    if (!library.empty()) {
                        bool save = Utils::getYesNo(
                            "Do you want to save changes before exiting? (y/n): ");
                        if (save) {
                            std::cout << "[SAVE] Saving data...\n";
                            try {
                                library.saveToBinaryFile(DATA_FILE);
                                std::cout << "[OK] Data saved successfully.\n";
                            } catch (const std::exception& e) {
                                std::cerr << "[!] Error saving data: " << e.what() << "\n";
                            }
                        } else {
                            std::cout << "[i] Changes not saved.\n";
                        }
                    }
                    std::cout << "[BYE] Goodbye!\n";
                    running = false;
                    break;
                default:
                    std::cerr << "[X] Invalid option! Please enter 0-8.\n";
                    break;
            }
        } catch (const BookException& e) {
            std::cerr << "[X] Error: " << e.what() << "\n";
        } catch (const std::exception& e) {
            std::cerr << "[X] Unexpected error: " << e.what() << "\n";
        }

        if (running) {
            // اگر به EOF رسیدیم، pauseScreen نباید منتظر ورودی بماند
            if (!std::cin.eof()) {
                Utils::pauseScreen();
            } else {
                break;
            }
        }
    }

    return 0;
}

// ============================================================
// پیاده‌سازی توابع منو
// ============================================================

void displayMainMenu() {
    std::cout << "|                                                          |\n";
    std::cout << "|   1. [+] Add new book                                    |\n";
    std::cout << "|   2. [-] Remove book by ISBN                             |\n";
    std::cout << "|   3. [E] Edit book by ISBN                               |\n";
    std::cout << "|   4. [?] Search books                                    |\n";
    std::cout << "|   5. [L] List all books                                  |\n";
    std::cout << "|   6. [S] Sort books                                      |\n";
    std::cout << "|   7. [W] Save data (Binary/CSV/JSON)                     |\n";
    std::cout << "|   8. [R] Load data (Binary/CSV/JSON)                     |\n";
    std::cout << "|   0. [X] Exit                                            |\n";
    std::cout << "|                                                          |\n";
}

// ------------------------------------------------------------
// ۱. افزودن کتاب
// ------------------------------------------------------------
void handleAddBook(Library& library) {
    std::cout << "\n=== [+] Add New Book ===\n";

    std::string title = Utils::getNonEmptyString("Book title: ", "title");
    std::string author = Utils::getNonEmptyString("Author name: ", "author");
    std::string isbn = Utils::getValidISBN("ISBN (10 or 13 digits): ");
    int year = Utils::getValidYear();

    Book book(title, author, isbn, year);
    library.addBook(book);

    std::cout << "\n[OK] Book added successfully:\n";
    book.display();
}

// ------------------------------------------------------------
// ۲. حذف کتاب
// ------------------------------------------------------------
void handleRemoveBook(Library& library) {
    std::cout << "\n=== [-] Remove Book ===\n";

    if (library.empty()) {
        std::cout << "[i] Library is empty.\n";
        return;
    }

    std::string isbn = Utils::getValidISBN("Enter ISBN of the book to remove: ");

    // نمایش کتاب قبل از حذف برای تایید
    auto results = library.searchByISBN(isbn);
    if (results.empty()) {
        std::cout << "[X] No book found with this ISBN.\n";
        return;
    }

    std::cout << "\nBook found:\n";
    results[0].display();

    bool confirm = Utils::getYesNo("Are you sure you want to remove this book? (y/n): ");
    if (confirm) {
        library.removeBook(isbn);
        std::cout << "[OK] Book removed successfully.\n";
    } else {
        std::cout << "[i] Remove operation cancelled.\n";
    }
}

// ------------------------------------------------------------
// ۳. ویرایش کتاب
// ------------------------------------------------------------
void handleEditBook(Library& library) {
    std::cout << "\n=== [E] Edit Book ===\n";

    if (library.empty()) {
        std::cout << "[i] Library is empty.\n";
        return;
    }

    std::string isbn = Utils::getValidISBN("Enter ISBN of the book to edit: ");

    // نمایش کتاب فعلی
    auto results = library.searchByISBN(isbn);
    if (results.empty()) {
        std::cout << "[X] No book found with this ISBN.\n";
        return;
    }

    std::cout << "\nCurrent book:\n";
    results[0].display();
    std::cout << "\nPress Enter to keep the current value.\n\n";

    // دریافت مقادیر جدید (اختیاری)
    std::cout << "New title (Enter to keep): ";
    std::string newTitle;
    std::getline(std::cin, newTitle);

    std::cout << "New author (Enter to keep): ";
    std::string newAuthor;
    std::getline(std::cin, newAuthor);

    std::cout << "New publication year (0 to keep): ";
    int newYear;
    std::cin >> newYear;
    Utils::clearInputStream();

    library.editBook(isbn, newTitle, newAuthor, newYear);

    std::cout << "\n[OK] Book updated successfully.\n";
    auto updated = library.searchByISBN(isbn);
    if (!updated.empty()) {
        updated[0].display();
    }
}

// ------------------------------------------------------------
// ۴. جستجوی کتاب
// ------------------------------------------------------------
void handleSearchBooks(const Library& library) {
    std::cout << "\n=== [?] Search Books ===\n";
    std::cout << "1. Search by title\n";
    std::cout << "2. Search by author\n";
    std::cout << "3. Search by ISBN\n";
    std::cout << "Choice: ";

    int choice;
    std::cin >> choice;
    Utils::clearInputStream();

    std::vector<Book> results;

    if (choice == 1) {
        std::string title;
        std::cout << "Title (or part of it): ";
        std::getline(std::cin, title);
        results = library.searchByTitle(title);
    } else if (choice == 2) {
        std::string author;
        std::cout << "Author (or part of name): ";
        std::getline(std::cin, author);
        results = library.searchByAuthor(author);
    } else if (choice == 3) {
        std::string isbn = Utils::getValidISBN("ISBN: ");
        results = library.searchByISBN(isbn);
    } else {
        std::cout << "[X] Invalid choice.\n";
        return;
    }

    if (results.empty()) {
        std::cout << "[X] No books found.\n";
    } else {
        std::cout << "\n[OK] Found " << results.size() << " book(s):\n";
        for (const auto& book : results) {
            book.display();
        }
    }
}

// ------------------------------------------------------------
// ۵. نمایش تمام کتاب‌ها
// ------------------------------------------------------------
void handleDisplayAll(const Library& library) {
    std::cout << "\n";
    library.displayAll();
}

// ------------------------------------------------------------
// ۶. مرتب‌سازی کتاب‌ها
// ------------------------------------------------------------
void handleSortBooks(Library& library) {
    std::cout << "\n=== [S] Sort Books ===\n";
    std::cout << "1. By title\n";
    std::cout << "2. By author\n";
    std::cout << "3. By ISBN\n";
    std::cout << "4. By publication year\n";
    std::cout << "Field choice: ";

    int fieldChoice;
    std::cin >> fieldChoice;
    Utils::clearInputStream();

    Library::SortField field;
    switch (fieldChoice) {
        case 1: field = Library::SortField::TITLE;  break;
        case 2: field = Library::SortField::AUTHOR; break;
        case 3: field = Library::SortField::ISBN;   break;
        case 4: field = Library::SortField::YEAR;   break;
        default:
            std::cout << "[X] Invalid choice.\n";
            return;
    }

    std::cout << "1. Ascending (A -> Z)\n";
    std::cout << "2. Descending (Z -> A)\n";
    std::cout << "Order choice: ";
    int orderChoice;
    std::cin >> orderChoice;
    Utils::clearInputStream();

    Library::SortOrder order = (orderChoice == 2)
        ? Library::SortOrder::DESCENDING
        : Library::SortOrder::ASCENDING;

    library.sortBy(field, order);
    std::cout << "[OK] Books sorted successfully.\n";
    library.displayAll();
}

// ------------------------------------------------------------
// ۷. ذخیره در فایل (با انتخاب فرمت)
// ------------------------------------------------------------
void handleSaveMenu(const Library& library) {
    std::cout << "\n=== [W] Save Data ===\n";

    if (library.empty()) {
        std::cout << "[!] Library is empty. Nothing to save.\n";
        return;
    }

    std::cout << "Select format:\n";
    std::cout << "  1. Binary (.dat) - compact, fast, our native format\n";
    std::cout << "  2. CSV (.csv)    - opens in Excel, human-readable\n";
    std::cout << "  3. JSON (.json)  - web/API standard, structured\n";
    std::cout << "Choice: ";

    int formatChoice;
    std::cin >> formatChoice;
    Utils::clearInputStream();

    try {
        switch (formatChoice) {
            case 1:
                library.saveToBinaryFile(DATA_FILE);
                std::cout << "[OK] " << library.size()
                          << " book(s) saved to binary file.\n";
                break;
            case 2:
                library.saveToCSVFile(CSV_FILE);
                std::cout << "[OK] " << library.size()
                          << " book(s) saved to CSV file: " << CSV_FILE << "\n";
                std::cout << "[i] You can open this file in Excel.\n";
                break;
            case 3:
                library.saveToJSONFile(JSON_FILE);
                std::cout << "[OK] " << library.size()
                          << " book(s) saved to JSON file: " << JSON_FILE << "\n";
                std::cout << "[i] This file uses standard JSON format.\n";
                break;
            default:
                std::cout << "[X] Invalid choice.\n";
        }
    } catch (const FileOperationException& e) {
        std::cerr << "[X] Save failed: " << e.what() << "\n";
    }
}

// ------------------------------------------------------------
// ۸. بارگذاری از فایل (با انتخاب فرمت)
// ------------------------------------------------------------
void handleLoadMenu(Library& library) {
    std::cout << "\n=== [R] Load Data ===\n";

    bool confirm = Utils::getYesNo(
        "Warning: This will replace current books. Continue? (y/n): ");
    if (!confirm) {
        std::cout << "[i] Operation cancelled.\n";
        return;
    }

    std::cout << "Select format:\n";
    std::cout << "  1. Binary (.dat)\n";
    std::cout << "  2. CSV (.csv)\n";
    std::cout << "  3. JSON (.json)\n";
    std::cout << "Choice: ";

    int formatChoice;
    std::cin >> formatChoice;
    Utils::clearInputStream();

    try {
        switch (formatChoice) {
            case 1:
                library.loadFromBinaryFile(DATA_FILE);
                std::cout << "[OK] " << library.size()
                          << " book(s) loaded from binary file.\n";
                break;
            case 2:
                library.loadFromCSVFile(CSV_FILE);
                std::cout << "[OK] " << library.size()
                          << " book(s) loaded from CSV file.\n";
                break;
            case 3:
                library.loadFromJSONFile(JSON_FILE);
                std::cout << "[OK] " << library.size()
                          << " book(s) loaded from JSON file.\n";
                break;
            default:
                std::cout << "[X] Invalid choice.\n";
        }
    } catch (const FileOperationException& e) {
        std::cerr << "[X] Load failed: " << e.what() << "\n";
    }
}
