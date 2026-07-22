#include "Library.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <sys/stat.h>
#if defined(_WIN32) || defined(_WIN64)
    #include <direct.h>
#endif
using namespace std;

const int FIELD_TITLE = 0;
const int FIELD_AUTHOR = 1;
const int FIELD_ISBN = 2;
const int FIELD_YEAR = 3;

const int ORDER_ASC = 0;
const int ORDER_DESC = 1;

// ساخت پوشه data/ در صورت عدم وجود
void ensureDataDirExists(const char* filename) {
    char dir[100];
    int i = 0;
    while (filename[i] != '\0' && filename[i] != '/' && i < 99) {
        dir[i] = filename[i];
        i++;
    }
    if (filename[i] == '/') {
        dir[i] = '\0';
#if defined(_WIN32) || defined(_WIN64)
        _mkdir(dir);
#else
        mkdir(dir, 0755);
#endif
    }
}

Library::Library() {
}

Library::~Library() {
}

int Library::findByISBN(const char* isbn) const {
    char normalized[MAX_ISBN_LEN];
    int j = 0;
    for (int k = 0; isbn[k] != '\0' && j < MAX_ISBN_LEN - 1; k++) {
        if (isbn[k] != '-') {
            normalized[j] = isbn[k];
            j++;
        }
    }
    normalized[j] = '\0';

    for (int i = 0; i < books.len(); i++) {
        if (strcmp(books.get(i).getISBN(), normalized) == 0) {
            return i;
        }
    }
    return -1;
}

bool Library::addBook(const Book& book) {
    if (findByISBN(book.getISBN()) != -1) {
        cout << "Error: Book with this ISBN already exists." << endl;
        return false;
    }
    books.add(book);
    return true;
}

bool Library::editBook(const char* isbn, const char* newTitle, const char* newAuthor, int newYear) {
    int index = findByISBN(isbn);
    if (index == -1) {
        cout << "Error: Book not found." << endl;
        return false;
    }

    Book b = books.get(index);

    if (newTitle != NULL && strlen(newTitle) > 0) {
        b.setTitle(newTitle);
    }
    if (newAuthor != NULL && strlen(newAuthor) > 0) {
        b.setAuthor(newAuthor);
    }
    if (newYear != 0) {
        b.setPublicationYear(newYear);
    }

    books.set(index, b);
    return true;
}

bool Library::removeBook(const char* isbn) {
    int index = findByISBN(isbn);
    if (index == -1) {
        cout << "Error: Book not found." << endl;
        return false;
    }
    books.removeAt(index);
    return true;
}

Vector<Book> Library::searchByTitle(const char* title) const {
    Vector<Book> results;

    char lowerTitle[200];
    int i = 0;
    for (; title[i] != '\0' && i < 199; i++) {
        char c = title[i];
        if (c >= 'A' && c <= 'Z') {
            c = c + ('a' - 'A');
        }
        lowerTitle[i] = c;
    }
    lowerTitle[i] = '\0';

    for (int k = 0; k < books.len(); k++) {
        char bookTitle[200];
        strcpy(bookTitle, books.get(k).getTitle());

        for (int j = 0; bookTitle[j] != '\0'; j++) {
            char c = bookTitle[j];
            if (c >= 'A' && c <= 'Z') {
                bookTitle[j] = c + ('a' - 'A');
            }
        }

        if (strstr(bookTitle, lowerTitle) != NULL) {
            results.add(books.get(k));
        }
    }
    return results;
}

Vector<Book> Library::searchByAuthor(const char* author) const {
    Vector<Book> results;

    char lowerAuthor[200];
    int i = 0;
    for (; author[i] != '\0' && i < 199; i++) {
        char c = author[i];
        if (c >= 'A' && c <= 'Z') {
            c = c + ('a' - 'A');
        }
        lowerAuthor[i] = c;
    }
    lowerAuthor[i] = '\0';

    for (int k = 0; k < books.len(); k++) {
        char bookAuthor[200];
        strcpy(bookAuthor, books.get(k).getAuthor());

        for (int j = 0; bookAuthor[j] != '\0'; j++) {
            char c = bookAuthor[j];
            if (c >= 'A' && c <= 'Z') {
                bookAuthor[j] = c + ('a' - 'A');
            }
        }

        if (strstr(bookAuthor, lowerAuthor) != NULL) {
            results.add(books.get(k));
        }
    }
    return results;
}

Vector<Book> Library::searchByISBN(const char* isbn) const {
    Vector<Book> results;
    int index = findByISBN(isbn);
    if (index != -1) {
        results.add(books.get(index));
    }
    return results;
}

void Library::sortBy(int field, int order) {
    int n = books.len();

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            Book a = books.get(j);
            Book b = books.get(j + 1);

            bool shouldSwap = false;

            if (field == FIELD_TITLE) {
                int cmp = strcmp(a.getTitle(), b.getTitle());
                if (order == ORDER_ASC) {
                    shouldSwap = (cmp > 0);
                } else {
                    shouldSwap = (cmp < 0);
                }
            } else if (field == FIELD_AUTHOR) {
                int cmp = strcmp(a.getAuthor(), b.getAuthor());
                if (order == ORDER_ASC) {
                    shouldSwap = (cmp > 0);
                } else {
                    shouldSwap = (cmp < 0);
                }
            } else if (field == FIELD_ISBN) {
                int cmp = strcmp(a.getISBN(), b.getISBN());
                if (order == ORDER_ASC) {
                    shouldSwap = (cmp > 0);
                } else {
                    shouldSwap = (cmp < 0);
                }
            } else if (field == FIELD_YEAR) {
                if (order == ORDER_ASC) {
                    shouldSwap = (a.getPublicationYear() > b.getPublicationYear());
                } else {
                    shouldSwap = (a.getPublicationYear() < b.getPublicationYear());
                }
            }

            if (shouldSwap) {
                Book temp = books.get(j);
                books.set(j, books.get(j + 1));
                books.set(j + 1, temp);
            }
        }
    }
}

void Library::displayAll() const {
    if (books.isEmpty()) {
        cout << "Library is empty. No books to display." << endl;
        return;
    }

    cout << endl;
    cout << "+--------------------------------------------------------------+" << endl;
    cout << "|                      Library Book List                       |" << endl;
    cout << "+--------------------------------------------------------------+" << endl;
    cout << "| #  | Title                    | Author              | ISBN            | Year  |" << endl;
    cout << "+--------------------------------------------------------------+" << endl;

    for (int i = 0; i < books.len(); i++) {
        Book b = books.get(i);

        char title[30];
        char author[25];
        char isbn[20];

        strncpy(title, b.getTitle(), 28);
        title[28] = '\0';
        if (strlen(b.getTitle()) > 28) {
            title[27] = '.';
            title[28] = '\0';
        }

        strncpy(author, b.getAuthor(), 22);
        author[22] = '\0';
        if (strlen(b.getAuthor()) > 22) {
            author[21] = '.';
            author[22] = '\0';
        }

        strncpy(isbn, b.getISBN(), 17);
        isbn[17] = '\0';

        cout << "| " << (i + 1) << "  | " << title << " | " << author << " | " << isbn << " | " << b.getPublicationYear() << " |" << endl;
    }
    cout << "+--------------------------------------------------------------+" << endl;
    cout << " Total: " << books.len() << " book(s)" << endl;
}

bool Library::saveToBinaryFile(const char* filename) const {
    ensureDataDirExists(filename);

    ofstream out(filename, ios::binary);
    if (!out.is_open()) {
        cout << "Error: Cannot open file for writing: " << filename << endl;
        return false;
    }

    int count = books.len();
    out.write((char*)&count, sizeof(int));

    for (int i = 0; i < count; i++) {
        Book b = books.get(i);
        out.write((char*)&b, sizeof(Book));
    }

    out.close();
    return true;
}

bool Library::loadFromBinaryFile(const char* filename) {
    ifstream in(filename, ios::binary);
    if (!in.is_open()) {
        return false;
    }

    books.clear();

    int count = 0;
    in.read((char*)&count, sizeof(int));

    for (int i = 0; i < count; i++) {
        Book b;
        in.read((char*)&b, sizeof(Book));
        books.add(b);
    }

    in.close();
    return true;
}

bool Library::saveToCSVFile(const char* filename) const {
    ensureDataDirExists(filename);

    ofstream out(filename);
    if (!out.is_open()) {
        cout << "Error: Cannot open file for writing: " << filename << endl;
        return false;
    }

    out << "Title,Author,ISBN,Year" << endl;

    for (int i = 0; i < books.len(); i++) {
        Book b = books.get(i);

        out << "\"" << b.getTitle() << "\",";
        out << "\"" << b.getAuthor() << "\",";
        out << "\"" << b.getISBN() << "\",";
        out << b.getPublicationYear() << endl;
    }

    out.close();
    return true;
}

bool Library::loadFromCSVFile(const char* filename) {
    ifstream in(filename);
    if (!in.is_open()) {
        return false;
    }

    books.clear();

    char line[1000];
    bool firstLine = true;

    while (in.getline(line, 1000)) {
        if (strlen(line) == 0) {
            continue;
        }

        if (firstLine) {
            firstLine = false;
            char lowerLine[100];
            for (int i = 0; line[i] != '\0' && i < 99; i++) {
                char c = line[i];
                if (c >= 'A' && c <= 'Z') {
                    c = c + ('a' - 'A');
                }
                lowerLine[i] = c;
            }
            lowerLine[strlen(line) < 99 ? strlen(line) : 99] = '\0';

            if (strstr(lowerLine, "title") != NULL) {
                continue;
            }
        }

        char title[200] = "";
        char author[200] = "";
        char isbn[50] = "";
        char yearStr[20] = "";

        int fieldIndex = 0;
        int pos = 0;

        while (line[pos] != '\0' && fieldIndex < 4) {
            char field[200] = "";
            int fpos = 0;

            if (line[pos] == '"') {
                pos++;
                while (line[pos] != '\0' && line[pos] != '"') {
                    if (line[pos] == '"' && line[pos + 1] == '"') {
                        field[fpos] = '"';
                        fpos++;
                        pos += 2;
                    } else {
                        field[fpos] = line[pos];
                        fpos++;
                        pos++;
                    }
                }
                if (line[pos] == '"') {
                    pos++;
                }
                if (line[pos] == ',') {
                    pos++;
                }
            } else {
                while (line[pos] != '\0' && line[pos] != ',') {
                    field[fpos] = line[pos];
                    fpos++;
                    pos++;
                }
                if (line[pos] == ',') {
                    pos++;
                }
            }
            field[fpos] = '\0';

            if (fieldIndex == 0) strcpy(title, field);
            else if (fieldIndex == 1) strcpy(author, field);
            else if (fieldIndex == 2) strcpy(isbn, field);
            else if (fieldIndex == 3) strcpy(yearStr, field);

            fieldIndex++;
        }

        if (fieldIndex == 4) {
            try {
                int year = atoi(yearStr);
                Book b(title, author, isbn, year);
                books.add(b);
            } catch (const char* msg) {
                cout << "Warning: Invalid book skipped: " << msg << endl;
            }
        }
    }

    in.close();
    return true;
}

bool Library::saveToJSONFile(const char* filename) const {
    ensureDataDirExists(filename);

    ofstream out(filename);
    if (!out.is_open()) {
        cout << "Error: Cannot open file for writing: " << filename << endl;
        return false;
    }

    out << "{" << endl;
    out << "  \"books\": [" << endl;

    for (int i = 0; i < books.len(); i++) {
        Book b = books.get(i);

        out << "    {" << endl;

        out << "      \"title\": \"";
        for (int k = 0; b.getTitle()[k] != '\0'; k++) {
            char c = b.getTitle()[k];
            if (c == '"') out << "\\\"";
            else if (c == '\\') out << "\\\\";
            else if (c == '\n') out << "\\n";
            else if (c == '\r') out << "\\r";
            else if (c == '\t') out << "\\t";
            else out << c;
        }
        out << "\"," << endl;

        out << "      \"author\": \"";
        for (int k = 0; b.getAuthor()[k] != '\0'; k++) {
            char c = b.getAuthor()[k];
            if (c == '"') out << "\\\"";
            else if (c == '\\') out << "\\\\";
            else if (c == '\n') out << "\\n";
            else if (c == '\r') out << "\\r";
            else if (c == '\t') out << "\\t";
            else out << c;
        }
        out << "\"," << endl;

        out << "      \"isbn\": \"";
        for (int k = 0; b.getISBN()[k] != '\0'; k++) {
            char c = b.getISBN()[k];
            if (c == '"') out << "\\\"";
            else if (c == '\\') out << "\\\\";
            else out << c;
        }
        out << "\"," << endl;

        out << "      \"year\": " << b.getPublicationYear() << endl;

        out << "    }";
        if (i < books.len() - 1) {
            out << ",";
        }
        out << endl;
    }

    out << "  ]" << endl;
    out << "}" << endl;

    out.close();
    return true;
}

bool Library::loadFromJSONFile(const char* filename) {
    ifstream in(filename);
    if (!in.is_open()) {
        return false;
    }

    books.clear();

    char content[50000];
    content[0] = '\0';
    char line[1000];

    while (in.getline(line, 1000)) {
        strcat(content, line);
        strcat(content, "\n");
    }
    in.close();

    char* booksPos = strstr(content, "\"books\"");
    if (booksPos == NULL) {
        return true;
    }

    char* arrayStart = strchr(booksPos, '[');
    if (arrayStart == NULL) {
        return true;
    }

    char* arrayEnd = strchr(arrayStart, ']');
    if (arrayEnd == NULL) {
        return true;
    }

    char* pos = arrayStart + 1;

    while (pos < arrayEnd) {
        char* objStart = strchr(pos, '{');
        if (objStart == NULL || objStart >= arrayEnd) {
            break;
        }

        int braceCount = 1;
        char* objEnd = objStart + 1;
        while (objEnd < arrayEnd && braceCount > 0) {
            if (*objEnd == '{') braceCount++;
            else if (*objEnd == '}') braceCount--;
            objEnd++;
        }

        char objStr[2000];
        int objLen = objEnd - objStart;
        if (objLen >= 2000) objLen = 1999;
        strncpy(objStr, objStart, objLen);
        objStr[objLen] = '\0';

        char title[200] = "";
        char author[200] = "";
        char isbn[50] = "";
        char yearStr[20] = "";

        char* keyPos = strstr(objStr, "\"title\"");
        if (keyPos != NULL) {
            char* colonPos = strchr(keyPos, ':');
            if (colonPos != NULL) {
                char* valueStart = colonPos + 1;
                while (*valueStart == ' ' || *valueStart == '\t') valueStart++;
                if (*valueStart == '"') {
                    valueStart++;
                    int j = 0;
                    while (*valueStart != '\0' && *valueStart != '"' && j < 199) {
                        if (*valueStart == '\\' && *(valueStart + 1) != '\0') {
                            char next = *(valueStart + 1);
                            if (next == '"') title[j] = '"';
                            else if (next == '\\') title[j] = '\\';
                            else if (next == 'n') title[j] = '\n';
                            else if (next == 't') title[j] = '\t';
                            else title[j] = next;
                            j++;
                            valueStart += 2;
                        } else {
                            title[j] = *valueStart;
                            j++;
                            valueStart++;
                        }
                    }
                    title[j] = '\0';
                }
            }
        }

        keyPos = strstr(objStr, "\"author\"");
        if (keyPos != NULL) {
            char* colonPos = strchr(keyPos, ':');
            if (colonPos != NULL) {
                char* valueStart = colonPos + 1;
                while (*valueStart == ' ' || *valueStart == '\t') valueStart++;
                if (*valueStart == '"') {
                    valueStart++;
                    int j = 0;
                    while (*valueStart != '\0' && *valueStart != '"' && j < 199) {
                        if (*valueStart == '\\' && *(valueStart + 1) != '\0') {
                            char next = *(valueStart + 1);
                            if (next == '"') author[j] = '"';
                            else if (next == '\\') author[j] = '\\';
                            else if (next == 'n') author[j] = '\n';
                            else if (next == 't') author[j] = '\t';
                            else author[j] = next;
                            j++;
                            valueStart += 2;
                        } else {
                            author[j] = *valueStart;
                            j++;
                            valueStart++;
                        }
                    }
                    author[j] = '\0';
                }
            }
        }

        keyPos = strstr(objStr, "\"isbn\"");
        if (keyPos != NULL) {
            char* colonPos = strchr(keyPos, ':');
            if (colonPos != NULL) {
                char* valueStart = colonPos + 1;
                while (*valueStart == ' ' || *valueStart == '\t') valueStart++;
                if (*valueStart == '"') {
                    valueStart++;
                    int j = 0;
                    while (*valueStart != '\0' && *valueStart != '"' && j < 49) {
                        if (*valueStart == '\\' && *(valueStart + 1) != '\0') {
                            char next = *(valueStart + 1);
                            if (next == '"') isbn[j] = '"';
                            else if (next == '\\') isbn[j] = '\\';
                            else isbn[j] = next;
                            j++;
                            valueStart += 2;
                        } else {
                            isbn[j] = *valueStart;
                            j++;
                            valueStart++;
                        }
                    }
                    isbn[j] = '\0';
                }
            }
        }

        keyPos = strstr(objStr, "\"year\"");
        if (keyPos != NULL) {
            char* colonPos = strchr(keyPos, ':');
            if (colonPos != NULL) {
                char* valueStart = colonPos + 1;
                while (*valueStart == ' ' || *valueStart == '\t') valueStart++;
                int j = 0;
                while ((*valueStart >= '0' && *valueStart <= '9') || *valueStart == '-') {
                    yearStr[j] = *valueStart;
                    j++;
                    valueStart++;
                }
                yearStr[j] = '\0';
            }
        }

        if (strlen(title) > 0 && strlen(author) > 0 && strlen(isbn) > 0) {
            try {
                int year = atoi(yearStr);
                Book b(title, author, isbn, year);
                books.add(b);
            } catch (const char* msg) {
                cout << "Warning: Invalid book in JSON skipped: " << msg << endl;
            }
        }

        pos = objEnd;
    }

    return true;
}

int Library::size() const {
    return books.len();
}

bool Library::empty() const {
    return books.isEmpty();
}
