<div align="center">

# 📚 سیستم مدیریت کتابخانه شخصی

### یک برنامه کنسولی به زبان C++ برای مدیریت مجموعه کتاب‌های شخصی

[![C++](https://img.shields.io/badge/C%2B%2B-11-blue.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)](https://isocpp.org/)
[![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux-lightgrey.svg?style=for-the-badge)]()
[![License](https://img.shields.io/badge/License-Educational-green.svg?style=for-the-badge)]()

<p>
  <a href="#-امکانات">امکانات</a> •
  <a href="#-ساختار-پروژه">ساختار</a> •
  <a href="#-نصب-و-اجرا">نصب</a> •
  <a href="#-معماری-پروژه">معماری</a> •
  <a href="#-فرمت‌های-فایل">فرمت‌ها</a>
</p>

</div>

---

## 📝 معرفی

این پروژه یک **سیستم مدیریت کتابخانه شخصی** است که به صورت یک برنامه کنسولی به زبان C++ پیاده‌سازی شده است. این پروژه برای درس **برنامه‌نویسی پیشرفته** توسعه داده شده و در آن تمام مفاهیم تدریس‌شده در کلاس اعمال شده است.

پروژه از **کلاس Vector جنریک با Template** (به جای `std::vector`)، **آرایه‌های کاراکتری** (به جای `std::string` در کلاس‌های ذخیره‌شونده)، **الگوریتم‌های دستی** (Bubble Sort و جستجوی خطی) و **مدیریت خطا با try/catch** استفاده می‌کند.

---

## ✨ امکانات

| امکان | توضیح |
|------|------|
| ➕ **افزودن کتاب** | با اعتبارسنجی کامل فیلدها (عنوان، نویسنده، ISBN، سال) |
| ✏️ **ویرایش کتاب** | بر اساس ISBN با امکان نگه‌داشتن فیلدهای قبلی |
| 🗑️ **حذف کتاب** | بر اساس ISBN با تأیید کاربر قبل از حذف |
| 🔍 **جستجوی کتاب** | بر اساس عنوان، نویسنده یا ISBN (جستجوی جزئی و حساس به حروف) |
| 🔃 **مرتب‌سازی کتاب‌ها** | با الگوریتم Bubble Sort دستی بر اساس ۴ فیلد (صعودی یا نزولی) |
| 📋 **نمایش لیست کتاب‌ها** | در قالب جدول فرمت‌بندی‌شده |
| 💾 **ذخیره‌سازی در سه فرمت** | باینری، CSV و JSON |
| 📂 **بارگذاری از فایل** | با هر سه فرمت |
| ✅ **اعتبارسنجی ورودی‌ها** | ISBN معتبر، سال منطقی، فیلدهای غیرخالی |
| 🛡️ **مدیریت خطا** | با `try/catch` و `throw "message"` |
| 🔄 **بارگذاری خودکار** | در شروع برنامه از فایل باینری |
| 💬 **پرسش قبل از خروج** | برای ذخیره‌سازی تغییرات |

---

## 🛠️ ویژگی‌های فنی (مطابق تدریس استاد)

این پروژه دقیقاً مطابق با سبک کدنویسی تدریس‌شده در کلاس پیاده‌سازی شده است:

| ویژگی | توضیح |
|------|------|
| 🧩 **کلاس Vector جنریک** | پیاده‌سازی دستی با Template (به جای `std::vector`) |
| 🔤 **آرایه‌های کاراکتری** | فیلدهای کلاس Book به صورت `char[]` (برای ذخیره باینری) |
| 📚 **توابع cstring** | استفاده از `strcpy`، `strcmp`، `strstr`، `strlen` |
| 🔄 **Bubble Sort دستی** | پیاده‌سازی مرتب‌سازی با دو حلقه `for` تودرتو |
| 🔁 **جستجوی دستی** | پیمایش با حلقه `for` کلاسیک (نه `std::find`) |
| ⚠️ **مدیریت خطا** | `try/catch` با `throw "message"` |
| 📋 **منوی کنسولی** | `while(true)` + `switch` با `case`های دارای `{}` |
| 🌐 **`using namespace std`** | در تمام فایل‌های `.cpp` |
| 🖥️ **`system("pause")` و `system("cls")`** | برای رابط کنسولی |
| ⌨️ **`cin.ignore()`** | قبل از `cin.getline()` بعد از `cin >>` |

---

## 📁 ساختار پروژه

```
personal-library/
├── include/                        # فایل‌های هدر (.h)
│   ├── Book.h                      # اعلان کلاس Book
│   ├── Library.h                   # اعلان کلاس Library
│   └── Vector.h                    # کلاس Vector جنریک با Template
├── src/                            # فایل‌های پیاده‌سازی (.cpp)
│   ├── Book.cpp                    # پیاده‌سازی کلاس Book
│   ├── Library.cpp                 # پیاده‌سازی کلاس Library
│   └── main.cpp                    # تابع main و منوی کنسولی
├── data/                           # فایل‌های داده (ایجاد خودکار)
│   ├── library.dat                 # فایل باینری
│   ├── library.csv                 # فایل CSV
│   └── library.json                # فایل JSON
├── build/                          # خروجی build (ایجاد خودکار)
├── Makefile                        # اسکریپت ساخت
├── .gitignore                      # قوانین Git ignore
└── README.md                       # مستندات پروژه
```

---

## 🔧 نیازمندی‌ها

| نیازمندی | نسخه | توضیح |
|---------|------|------|
| **کامپایلر C++** | g++ 4.7+ یا MSVC 2012+ | پشتیبانی از C++11 |
| **ابزار Make** | GNU Make 4+ | برای اجرای Makefile |
| **سیستم‌عامل** | ویندوز / لینوکس / مک | پروژه Cross-Platform است |

### نصب پیش‌نیازها

**روی ویندوز (با MSYS2):**
```powershell
# نصب MSYS2 از https://www.msys2.org/
pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain
```

**روی لینوکس (Ubuntu/Debian):**
```bash
sudo apt update
sudo apt install build-essential
```

---

## 🚀 نصب و اجرا

### ساخت پروژه

```bash
# ساخت پروژه
make

# اجرای برنامه
make run

# یا اجرای مستقیم
./build/library_app         # لینوکس / مک
.\build\library_app.exe     # ویندوز (PowerShell)
```

### سایر دستورات

```bash
make clean      # پاک کردن فایل‌های build
make rebuild    # پاکسازی و ساخت مجدد
make help       # نمایش راهنما
```

---

## 📖 نحوه استفاده

### منوی اصلی

```
+==========================================================+
|         Personal Library Management System - v1.0         |
+==========================================================+
|                                                          |
|   1. Add new book                                        |
|   2. Remove book by ISBN                                 |
|   3. Edit book by ISBN                                   |
|   4. Search books                                        |
|   5. List all books                                      |
|   6. Sort books                                          |
|   7. Save data (Binary/CSV/JSON)                         |
|   8. Load data (Binary/CSV/JSON)                         |
|   0. Exit                                                |
|                                                          |
+==========================================================+
```

### مراحل استفاده

1. در اولین اجرا، برنامه به طور خودکار فایل داده می‌سازد.
2. از منوی اصلی، گزینه `1` را برای افزودن کتاب انتخاب کنید.
3. **ISBN** باید ۱۰ یا ۱۳ رقم داشته باشد (می‌توانید از خط تیره استفاده کنید).
4. **سال انتشار** باید بین ۱۴۵۰ و ۲۰۲۶ باشد.
5. برای ذخیره داده‌ها از گزینه `7` استفاده کنید و فرمت دلخواه را انتخاب کنید.
6. قبل از خروج، برنامه از شما می‌پرسد که آیا می‌خواهید تغییرات را ذخیره کنید.

---

## 🏗️ معماری پروژه

### کلاس Vector (جنریک با Template)

کلاس `Vector` به جای `std::vector` استفاده شده است و کاملاً مطابق با تدریس استاد پیاده‌سازی شده:

```cpp
template <typename T>
class Vector {
private:
    T* data;          // آرایه پویا
    int capacity;     // ظرفیت کل
    int count;        // تعداد عناصر فعلی

    void resize();    // افزایش ظرفیت (دو برابر)

public:
    Vector();                          // سازنده پیش‌فرض
    Vector(const Vector<T>& other);    // سازنده کپی (Deep Copy)
    ~Vector();                         // مخرب
    Vector<T>& operator=(const Vector<T>& other);  // عملگر تخصیص

    void add(const T& value);          // افزودن عنصر
    T& get(int index);                 // دسترسی به عنصر
    void set(int index, const T& v);   // تنظیم عنصر
    void removeAt(int index);          // حذف عنصر
    int len() const;                   // تعداد عناصر
    bool isEmpty() const;              // بررسی خالی بودن
    void clear();                      // پاک کردن

    T& operator[](int index);          // عملگر دسترسی
};
```

### کلاس Book

کلاس `Book` با فیلدهای `char[]` (برای ذخیره باینری) پیاده‌سازی شده:

```cpp
class Book {
private:
    char title[200];        // عنوان کتاب
    char author[100];       // نام نویسنده
    char isbn[20];          // شماره ISBN
    int publicationYear;    // سال انتشار

public:
    Book();
    Book(const char* title, const char* author, const char* isbn, int year);

    // Getterها
    const char* getTitle() const;
    const char* getAuthor() const;
    const char* getISBN() const;
    int getPublicationYear() const;

    // Setterها (با اعتبارسنجی)
    void setTitle(const char* title);
    void setAuthor(const char* author);
    void setISBN(const char* isbn);
    void setPublicationYear(int year);

    void print() const;

    // Operator Overloading (با friend)
    friend ostream& operator<<(ostream& out, const Book& book);
    friend istream& operator>>(istream& in, Book& book);
};
```

### کلاس Library

کلاس `Library` از `Vector<Book>` برای مدیریت کتاب‌ها استفاده می‌کند:

```cpp
class Library {
private:
    Vector<Book> books;     // بردار سفارشی کتاب‌ها
    int findByISBN(const char* isbn) const;  // جستجوی دستی

public:
    // عملیات CRUD
    bool addBook(const Book& book);
    bool editBook(const char* isbn, const char* newTitle,
                  const char* newAuthor, int newYear);
    bool removeBook(const char* isbn);

    // جستجو (با strstr برای case-insensitive)
    Vector<Book> searchByTitle(const char* title) const;
    Vector<Book> searchByAuthor(const char* author) const;
    Vector<Book> searchByISBN(const char* isbn) const;

    // مرتب‌سازی (Bubble Sort دستی)
    void sortBy(int field, int order);

    // مدیریت فایل (سه فرمت)
    bool saveToBinaryFile(const char* filename) const;
    bool loadFromBinaryFile(const char* filename);
    bool saveToCSVFile(const char* filename) const;
    bool loadFromCSVFile(const char* filename);
    bool saveToJSONFile(const char* filename) const;
    bool loadFromJSONFile(const char* filename);

    void displayAll() const;
    int size() const;
    bool empty() const;
};
```

---

## 📦 فرمت‌های فایل

### ۱. فرمت باینری (.dat)

ذخیره کل شیء `Book` با `write/read` (چون فقط `char[]` و `int` دارد):

```
[count: int]
برای هر کتاب:
  [Book object: sizeof(Book) bytes]
```

### ۲. فرمت CSV (.csv)

قابل باز شدن در Excel با escaping صحیح:

```csv
Title,Author,ISBN,Year
"The C Programming Language","Brian Kernighan","9780131103627",1988
"Clean Code","Robert Martin","9780132350884",2008
```

### ۳. فرمت JSON (.json)

استاندارد وب و API با escaping صحیح:

```json
{
  "books": [
    {
      "title": "The C Programming Language",
      "author": "Brian Kernighan",
      "isbn": "9780131103627",
      "year": 1988
    }
  ]
}
```

---

## 🧪 مثال استفاده

### افزودن کتاب

```
=== Add New Book ===
Book title: The C Programming Language
Author name: Brian Kernighan
ISBN (10 or 13 digits): 9780131103627
Publication year (1450-2026): 1988

Book added successfully:
+----------------------------------------------+
|  Title:    The C Programming Language
|  Author:   Brian Kernighan
|  ISBN:     9780131103627
|  Year:     1988
+----------------------------------------------+
```

### نمایش لیست کتاب‌ها

```
+--------------------------------------------------------------+
|                      Library Book List                       |
+--------------------------------------------------------------+
| #  | Title                    | Author              | ISBN            | Year  |
+--------------------------------------------------------------+
| 1  | Clean Code               | Robert Martin       | 9780132350884   | 2008  |
| 2  | Design Patterns          | Gang of Four        | 9780201633610   | 1994  |
| 3  | Effective C++            | Scott Meyers        | 9780321334879   | 2005  |
| 4  | The C Programming Lang.. | Brian Kernighan     | 9780131103627   | 1988  |
+--------------------------------------------------------------+
 Total: 4 book(s)
```

---

## 🛠️ عیب‌یابی

| مشکل | راه‌حل |
|------|-------|
| `make: command not found` | نصب GNU Make (از MSYS2 روی ویندوز) |
| `g++: command not found` | نصب کامپایلر g++ و اضافه کردن به PATH |
| `fatal error: Book.h: No such file` | اجرای `make` از پوشه اصلی پروژه |
| فایل `data/library.dat` ساخته نمی‌شود | اجرای برنامه از پوشه اصلی پروژه |
| `_mkdir` was not declared | هدر `<direct.h>` به صورت شرطی include شده |
| کاراکترهای عجیب در کنسول | تنظیم کدپیج کنسول روی UTF-8 |

---

## 📋 چک‌لیست تطبیق با سبک استاد

- [x] فقط از کتابخانه‌های مجاز استفاده شده (`iostream`, `cstring`, `fstream`, `iomanip`, `cstdlib`)
- [x] تمام فیلدهای کلاس‌ها `private` و دسترسی فقط از طریق Getter/Setter
- [x] کلاس Vector با `new/delete`، مخرب و سازنده کپی (Deep Copy)
- [x] رشته‌های باینری به صورت `char[]` (نه `string`)
- [x] مقایسه با `strcmp`/`strstr` (نه `==`)
- [x] فایل هدر Include Guard دارد
- [x] فایل با `is_open()` چک می‌شود
- [x] حلقه `for` کلاسیک (نه range-based for)
- [x] منو با `while(true)` + `switch`
- [x] `using namespace std` در فایل‌های `.cpp`
- [x] `system("pause")` و `system("cls")` برای رابط کنسولی
- [x] `cin.ignore()` قبل از `cin.getline()` بعد از `cin >>`

---

## 🔄 کنترل نسخه

این پروژه از Git برای کنترل نسخه استفاده می‌کند. تاریخچه commitها:

```bash
git log --oneline
```

<details>
<summary>📝 تاریخچه Commitها</summary>

```
9799a83 refactor: rewrite project to match professor's teaching style
61f192c docs: rewrite README in Persian with full project info
4d9173e chore: remove comments for final clean version
998533d feat: add CSV and JSON file format support
32b4d6e Initial release: Personal Library Management System v1.0
```

</details>

---

<div align="center">

## 👥 اطلاعات پروژه

### 👤 نویسنده
**ابوالفضل بنام**
دانشجوی مهندسی کامپیوتر

### 👨‍🏫 استاد درس
**احمد بادکوبه‌ای**

### 🏫 دانشگاه
**دانشگاه اراک**

### 📅 سال تحصیلی
**نیمه دوم ۱۴۰۴-۱۴۰۵**

### 📚 درس
**برنامه‌نویسی پیشرفته**

---

*این پروژه به عنوان پروژه نهایی درس برنامه‌نویسی پیشرفته توسعه یافته است*

*© ۱۴۰۵ - دانشگاه اراک*

</div>
