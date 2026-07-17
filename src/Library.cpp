#include "Library.h"
#include "BookExceptions.h"
#include "Utils.h"

#include <algorithm>
#include <fstream>
#include <cstdint>
#include <iomanip>
#include <filesystem>
#include <iostream>
 
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

void Library::addBook(const Book& book) {
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

std::vector<Book> Library::searchByTitle(const std::string& title) const {
    std::vector<Book> results;
    std::string lowerTitle = Utils::toLower(title);

    std::copy_if(books.begin(), books.end(), std::back_inserter(results),
                 [&lowerTitle](const Book& b) {
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

void Library::sortBy(SortField field, SortOrder order) {
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
        return (order == SortOrder::ASCENDING) ? result : !result;
    };

    std::sort(books.begin(), books.end(), comparator);
}

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

void Library::saveToBinaryFile(const std::string& filename) const {
    std::cout << "[i] Saving to: " << std::filesystem::absolute(filename).string() << "\n";

    auto parentPath = std::filesystem::path(filename).parent_path();
    if (!parentPath.empty() && !std::filesystem::exists(parentPath)) {
        std::cout << "[i] Creating directory: "
                  << std::filesystem::absolute(parentPath).string() << "\n";
        if (!std::filesystem::create_directories(parentPath)) {
            throw FileOperationException("create directory for",
                std::filesystem::absolute(parentPath).string());
        }
    }

    std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
    if (!outFile.is_open()) {
        throw FileOperationException("write", filename);
    }

    uint32_t count = static_cast<uint32_t>(books.size());
    outFile.write(reinterpret_cast<const char*>(&count), sizeof(count));

    for (const auto& book : books) {
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

    if (std::filesystem::exists(filename)) {
        auto fileSize = std::filesystem::file_size(filename);
        std::cout << "[OK] File created. Size: " << fileSize << " bytes.\n";
    }
}

void Library::loadFromBinaryFile(const std::string& filename) {
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile.is_open()) {
        throw FileOperationException("read", filename);
    }

    books.clear();

    uint32_t count = 0;
    inFile.read(reinterpret_cast<char*>(&count), sizeof(count));

    if (inFile.eof()) {
        inFile.close();
        return;
    }

    for (uint32_t i = 0; i < count; ++i) {
        auto readString = [&inFile]() -> std::string {
            uint32_t len = 0;
            inFile.read(reinterpret_cast<char*>(&len), sizeof(len));
            if (inFile.eof() || len > 10000) {
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
            try {
                Book book(title, author, isbn, year);
                books.push_back(book);
            } catch (const BookException& e) {
                std::cerr << "[!] Warning: Invalid book in file (row " << i + 1
                          << ") skipped: " << e.what() << "\n";
            }
        }
    }

    inFile.close();
}

namespace {
    std::string escapeCSV(const std::string& str) {
        bool needsQuoting = false;
        for (char c : str) {
            if (c == ',' || c == '"' || c == '\n' || c == '\r') {
                needsQuoting = true;
                break;
            }
        }
        if (!needsQuoting) {
            return str;
        }
        std::string result = "\"";
        for (char c : str) {
            if (c == '"') {
                result += "\"\"";
            } else {
                result += c;
            }
        }
        result += "\"";
        return result;
    }

    std::string unescapeCSV(const std::string& str) {
        if (str.size() < 2 || str.front() != '"' || str.back() != '"') {
            return str;
        }
        std::string inner = str.substr(1, str.size() - 2);
        std::string result;
        result.reserve(inner.size());
        for (size_t i = 0; i < inner.size(); ++i) {
            if (inner[i] == '"' && i + 1 < inner.size() && inner[i + 1] == '"') {
                result += '"';
                ++i;
            } else {
                result += inner[i];
            }
        }
        return result;
    }

    std::vector<std::string> parseCSVLine(const std::string& line) {
        std::vector<std::string> fields;
        std::string current;
        bool inQuotes = false;

        for (size_t i = 0; i < line.size(); ++i) {
            char c = line[i];
            if (inQuotes) {
                if (c == '"') {
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
    auto parentPath = std::filesystem::path(filename).parent_path();
    if (!parentPath.empty() && !std::filesystem::exists(parentPath)) {
        std::filesystem::create_directories(parentPath);
    }

    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        throw FileOperationException("write", filename);
    }

    outFile << "Title,Author,ISBN,Year\n";

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
        if (line.empty() || line == "\r") {
            continue;
        }

        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        if (firstLine) {
            firstLine = false;
            std::string lowerLine = Utils::toLower(line);
            if (lowerLine.find("title") != std::string::npos) {
                continue;
            }
        }

        std::vector<std::string> fields = parseCSVLine(line);
        if (fields.size() < 4) {
            std::cerr << "[!] Warning: Invalid CSV row skipped: " << line << "\n";
            continue;
        }

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

void Library::saveToJSONFile(const std::string& filename) const {
    auto parentPath = std::filesystem::path(filename).parent_path();
    if (!parentPath.empty() && !std::filesystem::exists(parentPath)) {
        std::filesystem::create_directories(parentPath);
    }

    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        throw FileOperationException("write", filename);
    }

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
            outFile << ",";
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

    std::string content((std::istreambuf_iterator<char>(inFile)),
                         std::istreambuf_iterator<char>());
    inFile.close();

    books.clear();

    size_t booksPos = content.find("\"books\"");
    if (booksPos == std::string::npos) {
        return;
    }

    size_t arrayStart = content.find('[', booksPos);
    if (arrayStart == std::string::npos) {
        return;
    }

    size_t arrayEnd = content.find(']', arrayStart);
    if (arrayEnd == std::string::npos) {
        return;
    }

    size_t pos = arrayStart + 1;
    while (pos < arrayEnd) {
        size_t objStart = content.find('{', pos);
        if (objStart == std::string::npos || objStart >= arrayEnd) {
            break;
        }

        size_t objEnd = objStart + 1;
        int braceCount = 1;
        while (objEnd < arrayEnd && braceCount > 0) {
            if (content[objEnd] == '{') braceCount++;
            else if (content[objEnd] == '}') braceCount--;
            objEnd++;
        }

        std::string objStr = content.substr(objStart, objEnd - objStart);

        auto extractField = [&objStr](const std::string& key) -> std::string {
            std::string pattern = "\"" + key + "\"";
            size_t keyPos = objStr.find(pattern);
            if (keyPos == std::string::npos) return "";

            size_t colonPos = objStr.find(':', keyPos + pattern.size());
            if (colonPos == std::string::npos) return "";

            size_t valueStart = colonPos + 1;
            while (valueStart < objStr.size() &&
                   (objStr[valueStart] == ' ' || objStr[valueStart] == '\t')) {
                valueStart++;
            }

            if (valueStart >= objStr.size()) return "";

            if (objStr[valueStart] == '"') {
                size_t valueEnd = valueStart + 1;
                std::string result;
                while (valueEnd < objStr.size() && objStr[valueEnd] != '"') {
                    if (objStr[valueEnd] == '\\' && valueEnd + 1 < objStr.size()) {
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

size_t Library::size() const { return books.size(); }

bool Library::empty() const { return books.empty(); }

const std::vector<Book>& Library::getBooks() const { return books; }
