/**
 * @file Library.cpp
 * @brief پیاده‌سازی متدهای کلاس Library
 */

#include "Library.h"
#include "BookExceptions.h"
#include "Utils.h"

#include <algorithm>    // برای std::find_if، std::sort، std::remove
#include <fstream>      // برای std::ifstream و std::ofstream
#include <cstdint>      // برای uint32_t
#include <iomanip>      // برای std::setw و std::left
#include <filesystem>   // برای std::filesystem (C++17) - ساخت پوشه خودکار
#include <iostream>     // برای پیام‌های دیباگ

// ============================================================
// متدهای کمکی خصوصی
// ============================================================

std::vector<Book>::iterator Library::findByISBN(const std::string& isbn) {
    std::string normalized = Utils::normalizeISBN(isbn);
    return std::find_if(books.begin(), books.end(),
                        [&normalized](const Book& b) {
                            return b.getISBN() == normalized;
                        });
}

std::vector<Book>::const_iterator Library::findByISBN(const std::string& isbn) const {
    std::string normalized = Utils::normalizeISBN(isbn);
    return std::find_if(books.begin(), books.end(),
                        [&normalized](const Book& b) {
                            return b.getISBN() == normalized;
                        });
}

// ============================================================
// متدهای CRUD
// ============================================================

void Library::addBook(const Book& book) {
    // بررسی تکراری نبودن ISBN
    if (findByISBN(book.getISBN()) != books.end()) {
        throw DuplicateISBNException(book.getISBN());
    }
    books.push_back(book);
}

void Library::editBook(const std::string& isbn,
                       const std::string& newTitle,
                       const std::string& newAuthor,
                       int newYear) {
    auto it = findByISBN(isbn);
    if (it == books.end()) {
        throw BookNotFoundException(isbn);
    }

    // فقط فیلدهایی که کاربر مقدار داده را تغییر می‌دهیم
    if (!Utils::isEmptyOrWhitespace(newTitle)) {
        it->setTitle(newTitle);
    }
    if (!Utils::isEmptyOrWhitespace(newAuthor)) {
        it->setAuthor(newAuthor);
    }
    if (newYear != 0) {
        it->setPublicationYear(newYear);
    }
}

void Library::removeBook(const std::string& isbn) {
    auto it = findByISBN(isbn);
    if (it == books.end()) {
        throw BookNotFoundException(isbn);
    }
    books.erase(it);
}

// ============================================================
// متدهای جستجو
// ============================================================

std::vector<Book> Library::searchByTitle(const std::string& title) const {
    std::vector<Book> results;
    std::string lowerTitle = Utils::toLower(title);

    // استفاده از std::copy_if برای کپی کردن کتاب‌های منطبق
    std::copy_if(books.begin(), books.end(), std::back_inserter(results),
                 [&lowerTitle](const Book& b) {
                     // جستجوی جزئی و case-insensitive
                     std::string bookTitle = Utils::toLower(b.getTitle());
                     return bookTitle.find(lowerTitle) != std::string::npos;
                 });
    return results;
}

std::vector<Book> Library::searchByAuthor(const std::string& author) const {
    std::vector<Book> results;
    std::string lowerAuthor = Utils::toLower(author);

    std::copy_if(books.begin(), books.end(), std::back_inserter(results),
                 [&lowerAuthor](const Book& b) {
                     std::string bookAuthor = Utils::toLower(b.getAuthor());
                     return bookAuthor.find(lowerAuthor) != std::string::npos;
                 });
    return results;
}

std::vector<Book> Library::searchByISBN(const std::string& isbn) const {
    std::vector<Book> results;
    std::string normalized = Utils::normalizeISBN(isbn);

    std::copy_if(books.begin(), books.end(), std::back_inserter(results),
                 [&normalized](const Book& b) {
                     return b.getISBN() == normalized;
                 });
    return results;
}

// ============================================================
// متدهای مرتب‌سازی
// ============================================================

void Library::sortBy(SortField field, SortOrder order) {
    // تعریف comparator بر اساس فیلد انتخاب شده
    auto comparator = [field, order](const Book& a, const Book& b) {
        bool result = false;
        switch (field) {
            case SortField::TITLE:
                result = a.getTitle() < b.getTitle();
                break;
            case SortField::AUTHOR:
                result = a.getAuthor() < b.getAuthor();
                break;
            case SortField::ISBN:
                result = a.getISBN() < b.getISBN();
                break;
            case SortField::YEAR:
                result = a.getPublicationYear() < b.getPublicationYear();
                break;
        }
        // اگر نزولی است، نتیجه را معکوس می‌کنیم
        return (order == SortOrder::ASCENDING) ? result : !result;
    };

    std::sort(books.begin(), books.end(), comparator);
}

// ============================================================
// متدهای نمایش
// ============================================================

void Library::displayAll() const {
    if (books.empty()) {
        std::cout << "Library is empty. No books to display.\n";
        return;
    }

    std::cout << "\n+--------------------------------------------------------------+\n";
    std::cout << "|                      Library Book List                       |\n";
    std::cout << "+--------------------------------------------------------------+\n";
    std::cout << "| " << std::left << std::setw(4)  << "#"
              << "| " << std::setw(25) << "Title"
              << "| " << std::setw(20) << "Author"
              << "| " << std::setw(15) << "ISBN"
              << "| " << std::setw(6)  << "Year" << "|\n";
    std::cout << "+--------------------------------------------------------------+\n";

    int index = 1;
    for (const auto& book : books) {
        // برش رشته‌ها اگر خیلی بلند باشند
        std::string title = book.getTitle();
        std::string author = book.getAuthor();
        std::string isbn = book.getISBN();

        if (title.length() > 24) title = title.substr(0, 23) + ".";
        if (author.length() > 19) author = author.substr(0, 18) + ".";
        if (isbn.length() > 14) isbn = isbn.substr(0, 13) + ".";

        std::cout << "| " << std::left << std::setw(4)  << index++
                  << "| " << std::setw(25) << title
                  << "| " << std::setw(20) << author
                  << "| " << std::setw(15) << isbn
                  << "| " << std::setw(6)  << book.getPublicationYear() << "|\n";
    }
    std::cout << "+--------------------------------------------------------------+\n";
    std::cout << " Total: " << books.size() << " book(s)\n";
}

// ============================================================
// متدهای مدیریت فایل باینری
// ============================================================

void Library::saveToBinaryFile(const std::string& filename) const {
    // نمایش مسیر کامل فایل برای دیباگ
    std::cout << "[i] Saving to: " << std::filesystem::absolute(filename).string() << "\n";

    // ساخت پوشه والد اگر وجود نداشته باشد (مثلا پوشه data/)
    auto parentPath = std::filesystem::path(filename).parent_path();
    if (!parentPath.empty() && !std::filesystem::exists(parentPath)) {
        std::cout << "[i] Creating directory: "
                  << std::filesystem::absolute(parentPath).string() << "\n";
        // ساخت پوشه (و همه پوشه‌های والد اگر لازم باشد)
        if (!std::filesystem::create_directories(parentPath)) {
            throw FileOperationException("create directory for",
                std::filesystem::absolute(parentPath).string());
        }
    }

    // باز کردن فایل در حالت باینری برای نوشتن
    std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
    if (!outFile.is_open()) {
        throw FileOperationException("write", filename);
    }

    // نوشتن تعداد کتاب‌ها (به صورت uint32_t برای سازگاری بین پلتفرم‌ها)
    uint32_t count = static_cast<uint32_t>(books.size());
    outFile.write(reinterpret_cast<const char*>(&count), sizeof(count));

    // نوشتن هر کتاب
    for (const auto& book : books) {
        // تابع کمکی برای نوشتن رشته (طول + محتوا)
        auto writeString = [&outFile](const std::string& str) {
            uint32_t len = static_cast<uint32_t>(str.size());
            outFile.write(reinterpret_cast<const char*>(&len), sizeof(len));
            outFile.write(str.data(), len);
        };

        writeString(book.getTitle());
        writeString(book.getAuthor());
        writeString(book.getISBN());

        int year = book.getPublicationYear();
        outFile.write(reinterpret_cast<const char*>(&year), sizeof(year));
    }

    outFile.close();

    if (!outFile) {
        throw FileOperationException("write", filename);
    }

    // تایید موفقیت عملیات
    if (std::filesystem::exists(filename)) {
        auto fileSize = std::filesystem::file_size(filename);
        std::cout << "[OK] File created. Size: " << fileSize << " bytes.\n";
    }
}

void Library::loadFromBinaryFile(const std::string& filename) {
    // باز کردن فایل در حالت باینری برای خواندن
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile.is_open()) {
        throw FileOperationException("read", filename);
    }

    // پاک کردن کتاب‌های موجود قبل از بارگذاری
    books.clear();

    // خواندن تعداد کتاب‌ها
    uint32_t count = 0;
    inFile.read(reinterpret_cast<char*>(&count), sizeof(count));

    // اگر فایل خالی است، هیچ کاری نمی‌کنیم
    if (inFile.eof()) {
        inFile.close();
        return;
    }

    // خواندن هر کتاب
    for (uint32_t i = 0; i < count; ++i) {
        // تابع کمکی برای خواندن رشته
        auto readString = [&inFile]() -> std::string {
            uint32_t len = 0;
            inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
            if (inFile.eof() || len > 10000) {  // محدودیت امنیتی
                return "";
            }
            std::string str(len, '\0');
            inFile.read(&str[0], len);
            return str;
        };

        std::string title = readString();
        std::string author = readString();
        std::string isbn = readString();

        int year = 0;
        inFile.read(reinterpret_cast<char*>(&year), sizeof(year));

        if (inFile) {
            // اضافه کردن کتاب به بردار (بدون بررسی تکراری بودن، چون از فایل می‌خوانیم)
            try {
                Book book(title, author, isbn, year);
                books.push_back(book);
            } catch (const BookException& e) {
                // اگر یک کتاب نامعتبر در فایل بود، از قلم می‌اندازیم و ادامه می‌دهیم
                std::cerr << "[!] Warning: Invalid book in file (row " << i + 1
                          << ") skipped: " << e.what() << "\n";
            }
        }
    }

    inFile.close();
}

// ============================================================
// متدهای مدیریت فایل CSV
// ============================================================

// تابع کمکی برای escaping رشته در CSV (مطابق RFC 4180)
// اگر رشته شامل کاما، علامت نقل قول یا خط جدید باشد، باید درون
// علامت نقل قول قرار گیرد و تمام علامت‌های نقل قول داخلی دو倍 شوند.
namespace {
    std::string escapeCSV(const std::string& str) {
        // بررسی نیاز به escaping
        bool needsQuoting = false;
        for (char c : str) {
            if (c == ',' || c == '"' || c == '\n' || c == '\r') {
                needsQuoting = true;
                break;
            }
        }
        if (!needsQuoting) {
            return str;  // بدون تغییر
        }
        // قرار دادن درون علامت نقل قول و دو برابر کردن علامت‌های نقل قول داخلی
        std::string result = "\"";
        for (char c : str) {
            if (c == '"') {
                result += "\"\"";  // دو تا علامت نقل قول
            } else {
                result += c;
            }
        }
        result += "\"";
        return result;
    }

    // تابع کمکی برای حذف escaping از رشته CSV
    std::string unescapeCSV(const std::string& str) {
        // اگر رشته با علامت نقل قول شروع و تمام نمی‌شود، بدون تغییر برمی‌گردد
        if (str.size() < 2 || str.front() != '"' || str.back() != '"') {
            return str;
        }
        // حذف علامت‌های نقل قول ابتدا و انتها
        std::string inner = str.substr(1, str.size() - 2);
        // جایگزینی دو علامت نقل قول پشت سر هم با یک علامت
        std::string result;
        result.reserve(inner.size());
        for (size_t i = 0; i < inner.size(); ++i) {
            if (inner[i] == '"' && i + 1 < inner.size() && inner[i + 1] == '"') {
                result += '"';
                ++i;  // رد کردن علامت دوم
            } else {
                result += inner[i];
            }
        }
        return result;
    }

    // تابع کمکی برای parsing یک خط CSV به فیلدها
    std::vector<std::string> parseCSVLine(const std::string& line) {
        std::vector<std::string> fields;
        std::string current;
        bool inQuotes = false;

        for (size_t i = 0; i < line.size(); ++i) {
            char c = line[i];
            if (inQuotes) {
                if (c == '"') {
                    // بررسی علامت نقل قول فراری (دو تا پشت سر هم)
                    if (i + 1 < line.size() && line[i + 1] == '"') {
                        current += '"';
                        ++i;
                    } else {
                        inQuotes = false;
                    }
                } else {
                    current += c;
                }
            } else {
                if (c == '"') {
                    inQuotes = true;
                } else if (c == ',') {
                    fields.push_back(current);
                    current.clear();
                } else {
                    current += c;
                }
            }
        }
        fields.push_back(current);
        return fields;
    }

    // تابع کمکی برای escaping رشته در JSON
    std::string escapeJSON(const std::string& str) {
        std::string result;
        result.reserve(str.size() + 10);
        for (char c : str) {
            switch (c) {
                case '"':  result += "\\\""; break;
                case '\\': result += "\\\\"; break;
                case '\n': result += "\\n";  break;
                case '\r': result += "\\r";  break;
                case '\t': result += "\\t";  break;
                default:
                    if (static_cast<unsigned char>(c) < 0x20) {
                        // کاراکترهای کنترلی به صورت \uXXXX
                        char buf[8];
                        snprintf(buf, sizeof(buf), "\\u%04x",
                                 static_cast<unsigned char>(c));
                        result += buf;
                    } else {
                        result += c;
                    }
            }
        }
        return result;
    }
}

void Library::saveToCSVFile(const std::string& filename) const {
    // ساخت پوشه والد اگر وجود نداشته باشد
    auto parentPath = std::filesystem::path(filename).parent_path();
    if (!parentPath.empty() && !std::filesystem::exists(parentPath)) {
        std::filesystem::create_directories(parentPath);
    }

    // باز کردن فایل متنی برای نوشتن
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        throw FileOperationException("write", filename);
    }

    // نوشتن خط هدر (نام ستون‌ها)
    outFile << "Title,Author,ISBN,Year\n";

    // نوشتن هر کتاب در یک خط
    for (const auto& book : books) {
        outFile << escapeCSV(book.getTitle()) << ","
                << escapeCSV(book.getAuthor()) << ","
                << escapeCSV(book.getISBN()) << ","
                << book.getPublicationYear() << "\n";
    }

    outFile.close();
    if (!outFile) {
        throw FileOperationException("write", filename);
    }
}

void Library::loadFromCSVFile(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        throw FileOperationException("read", filename);
    }

    books.clear();

    std::string line;
    bool firstLine = true;

    while (std::getline(inFile, line)) {
        // نادیده گرفتن خط خالی
        if (line.empty() || line == "\r") {
            continue;
        }

        // حذف \r در انتهای خط (برای فایل‌های ویندوزی CRLF)
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        // خط اول هدر است، نادیده می‌گیریم
        if (firstLine) {
            firstLine = false;
            // تشخیص خودکار هدر: اگر شامل کلمه Title است، خط را رد می‌کنیم
            std::string lowerLine = Utils::toLower(line);
            if (lowerLine.find("title") != std::string::npos) {
                continue;
            }
        }

        // parsing خط به فیلدها
        std::vector<std::string> fields = parseCSVLine(line);
        if (fields.size() < 4) {
            std::cerr << "[!] Warning: Invalid CSV row skipped: " << line << "\n";
            continue;
        }

        // فیلدها: title, author, isbn, year
        std::string title   = unescapeCSV(fields[0]);
        std::string author  = unescapeCSV(fields[1]);
        std::string isbn    = unescapeCSV(fields[2]);
        std::string yearStr = unescapeCSV(fields[3]);

        try {
            int year = std::stoi(yearStr);
            Book book(title, author, isbn, year);
            books.push_back(book);
        } catch (const std::exception& e) {
            std::cerr << "[!] Warning: Invalid book in CSV row skipped: "
                      << line << " (" << e.what() << ")\n";
        }
    }

    inFile.close();
}

// ============================================================
// متدهای مدیریت فایل JSON
// ============================================================

void Library::saveToJSONFile(const std::string& filename) const {
    // ساخت پوشه والد اگر وجود نداشته باشد
    auto parentPath = std::filesystem::path(filename).parent_path();
    if (!parentPath.empty() && !std::filesystem::exists(parentPath)) {
        std::filesystem::create_directories(parentPath);
    }

    // باز کردن فایل متنی برای نوشتن
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        throw FileOperationException("write", filename);
    }

    // نوشتن JSON به صورت دستی (بدون کتابخانه خارجی)
    outFile << "{\n";
    outFile << "  \"books\": [\n";

    for (size_t i = 0; i < books.size(); ++i) {
        const auto& book = books[i];
        outFile << "    {\n";
        outFile << "      \"title\": \"" << escapeJSON(book.getTitle()) << "\",\n";
        outFile << "      \"author\": \"" << escapeJSON(book.getAuthor()) << "\",\n";
        outFile << "      \"isbn\": \"" << escapeJSON(book.getISBN()) << "\",\n";
        outFile << "      \"year\": " << book.getPublicationYear() << "\n";
        outFile << "    }";
        if (i + 1 < books.size()) {
            outFile << ",";  // کاما بین آیتم‌های آرایه
        }
        outFile << "\n";
    }

    outFile << "  ]\n";
    outFile << "}\n";

    outFile.close();
    if (!outFile) {
        throw FileOperationException("write", filename);
    }
}

void Library::loadFromJSONFile(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        throw FileOperationException("read", filename);
    }

    // خواندن کل فایل در یک رشته
    std::string content((std::istreambuf_iterator<char>(inFile)),
                         std::istreambuf_iterator<char>());
    inFile.close();

    books.clear();

    // ============================================================
    // JSON Parser ساده (بدون کتابخانه خارجی)
    // این parser فقط برای فرمت ساده‌ای که ما تولید می‌کنیم طراحی شده
    // ============================================================

    // پیدا کردن شروع آرایه books
    size_t booksPos = content.find("\"books\"");
    if (booksPos == std::string::npos) {
        return;  // فایل خالی یا نامعتبر
    }

    size_t arrayStart = content.find('[', booksPos);
    if (arrayStart == std::string::npos) {
        return;
    }

    size_t arrayEnd = content.find(']', arrayStart);
    if (arrayEnd == std::string::npos) {
        return;
    }

    // پیدا کردن تمام اشیاء کتاب در آرایه
    size_t pos = arrayStart + 1;
    while (pos < arrayEnd) {
        // پیدا کردن شروع شیء کتاب
        size_t objStart = content.find('{', pos);
        if (objStart == std::string::npos || objStart >= arrayEnd) {
            break;
        }

        // پیدا کردن پایان شیء کتاب (با در نظر گرفتن اشیاء تو در تو)
        size_t objEnd = objStart + 1;
        int braceCount = 1;
        while (objEnd < arrayEnd && braceCount > 0) {
            if (content[objEnd] == '{') braceCount++;
            else if (content[objEnd] == '}') braceCount--;
            objEnd++;
        }

        std::string objStr = content.substr(objStart, objEnd - objStart);

        // استخراج فیلدها با الگوهای ساده
        auto extractField = [&objStr](const std::string& key) -> std::string {
            // الگو: "key": "value" یا "key": number
            std::string pattern = "\"" + key + "\"";
            size_t keyPos = objStr.find(pattern);
            if (keyPos == std::string::npos) return "";

            size_t colonPos = objStr.find(':', keyPos + pattern.size());
            if (colonPos == std::string::npos) return "";

            size_t valueStart = colonPos + 1;
            // رد کردن فضای خالی
            while (valueStart < objStr.size() &&
                   (objStr[valueStart] == ' ' || objStr[valueStart] == '\t')) {
                valueStart++;
            }

            if (valueStart >= objStr.size()) return "";

            // اگر با " شروع می‌شود، رشته است
            if (objStr[valueStart] == '"') {
                size_t valueEnd = valueStart + 1;
                std::string result;
                while (valueEnd < objStr.size() && objStr[valueEnd] != '"') {
                    if (objStr[valueEnd] == '\\' && valueEnd + 1 < objStr.size()) {
                        // کاراکتر فراری
                        char next = objStr[valueEnd + 1];
                        switch (next) {
                            case '"':  result += '"';  break;
                            case '\\': result += '\\'; break;
                            case 'n':  result += '\n'; break;
                            case 'r':  result += '\r'; break;
                            case 't':  result += '\t'; break;
                            default:   result += next; break;
                        }
                        valueEnd += 2;
                    } else {
                        result += objStr[valueEnd];
                        valueEnd++;
                    }
                }
                return result;
            } else {
                // عدد است
                size_t valueEnd = valueStart;
                while (valueEnd < objStr.size() &&
                       (std::isdigit(static_cast<unsigned char>(objStr[valueEnd])) ||
                        objStr[valueEnd] == '-')) {
                    valueEnd++;
                }
                return objStr.substr(valueStart, valueEnd - valueStart);
            }
        };

        std::string title  = extractField("title");
        std::string author = extractField("author");
        std::string isbn   = extractField("isbn");
        std::string yearStr = extractField("year");

        try {
            int year = yearStr.empty() ? 0 : std::stoi(yearStr);
            Book book(title, author, isbn, year);
            books.push_back(book);
        } catch (const std::exception& e) {
            std::cerr << "[!] Warning: Invalid book in JSON skipped: "
                      << e.what() << "\n";
        }

        pos = objEnd;
    }
}

// ============================================================
// متدهای دسترسی
// ============================================================

size_t Library::size() const { return books.size(); }

bool Library::empty() const { return books.empty(); }

const std::vector<Book>& Library::getBooks() const { return books; }
