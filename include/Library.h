/**
 * @file Library.h
 * @brief تعریف کلاس Library برای مدیریت مجموعه کتاب‌ها
 *
 * این کلاس مسئول مدیریت کامل مجموعه کتاب‌های شخصی است.
 * از std::vector برای ذخیره کتاب‌ها استفاده می‌کند و تمام عملیات
 * CRUD، جستجو، مرتب‌سازی و مدیریت فایل را فراهم می‌کند.
 *
 * الگوهای طراحی استفاده شده:
 * - Repository Pattern: کلاس Library به عنوان مخزن داده عمل می‌کند
 * - Iterator Pattern: از iteratorهای STL برای جستجو استفاده می‌شود
 */

#ifndef LIBRARY_H
#define LIBRARY_H

#include <vector>
#include <string>
#include "Book.h"

/**
 * @class Library
 * @brief کلاس مدیریت مجموعه کتاب‌ها
 */
class Library {
public:
    /**
     * @enum SortField
     * @brief فیلدهای ممکن برای مرتب‌سازی
     */
    enum class SortField {
        TITLE,    ///< مرتب‌سازی بر اساس عنوان
        AUTHOR,   ///< مرتب‌سازی بر اساس نویسنده
        ISBN,     ///< مرتب‌سازی بر اساس ISBN
        YEAR      ///< مرتب‌سازی بر اساس سال انتشار
    };

    /**
     * @enum SortOrder
     * @brief ترتیب مرتب‌سازی
     */
    enum class SortOrder {
        ASCENDING,   ///< صعودی
        DESCENDING   ///< نزولی
    };

private:
    std::vector<Book> books;  ///< بردار نگهدارنده کتاب‌ها

    /**
     * @brief یافتن iterator به کتاب با ISBN مشخص
     * @param isbn شماره ISBN برای جستجو
     * @return iterator به کتاب یا books.end() اگر یافت نشد
     */
    std::vector<Book>::iterator findByISBN(const std::string& isbn);

    /**
     * @brief نسخه const از findByISBN (برای استفاده در متدهای const)
     */
    std::vector<Book>::const_iterator findByISBN(const std::string& isbn) const;

public:
    // ============================================================
    // متدهای CRUD (Create, Read, Update, Delete)
    // ============================================================

    /**
     * @brief افزودن کتاب جدید به کتابخانه
     *
     * @param book کتابی که باید اضافه شود
     * @throw DuplicateISBNException اگر ISBN از قبل وجود داشته باشد
     */
    void addBook(const Book& book);

    /**
     * @brief ویرایش کتاب با ISBN مشخص
     *
     * @param isbn ISBN کتابی که باید ویرایش شود
     * @param newTitle عنوان جدید (اگر خالی باشد، تغییر نمی‌کند)
     * @param newAuthor نویسنده جدید (اگر خالی باشد، تغییر نمی‌کند)
     * @param newYear سال جدید (اگر ۰ باشد، تغییر نمی‌کند)
     * @throw BookNotFoundException اگر کتاب یافت نشد
     */
    void editBook(const std::string& isbn,
                  const std::string& newTitle = "",
                  const std::string& newAuthor = "",
                  int newYear = 0);

    /**
     * @brief حذف کتاب با ISBN مشخص
     *
     * @param isbn ISBN کتابی که باید حذف شود
     * @throw BookNotFoundException اگر کتاب یافت نشد
     */
    void removeBook(const std::string& isbn);

    // ============================================================
    // متدهای جستجو (Search)
    // ============================================================

    /**
     * @brief جستجوی کتاب بر اساس عنوان (جستجوی جزئی، case-insensitive)
     * @param title عنوان یا بخشی از عنوان برای جستجو
     * @return برداری از کتاب‌های یافت شده
     */
    std::vector<Book> searchByTitle(const std::string& title) const;

    /**
     * @brief جستجوی کتاب بر اساس نویسنده (جستجوی جزئی، case-insensitive)
     */
    std::vector<Book> searchByAuthor(const std::string& author) const;

    /**
     * @brief جستجوی کتاب بر اساس ISBN (تطبیق دقیق)
     */
    std::vector<Book> searchByISBN(const std::string& isbn) const;

    // ============================================================
    // متدهای مرتب‌سازی (Sort)
    // ============================================================

    /**
     * @brief مرتب‌سازی کتاب‌ها بر اساس فیلد مشخص شده
     *
     * @param field فیلد مرتب‌سازی
     * @param order ترتیب مرتب‌سازی (صعودی یا نزولی)
     */
    void sortBy(SortField field, SortOrder order = SortOrder::ASCENDING);

    // ============================================================
    // متدهای نمایش (Display)
    // ============================================================

    /**
     * @brief نمایش تمام کتاب‌های کتابخانه در قالب جدول
     */
    void displayAll() const;

    // ============================================================
    // متدهای مدیریت فایل (File Management)
    // ============================================================

    /**
     * @brief ذخیره‌سازی تمام کتاب‌ها در فایل باینری
     *
     * فرمت فایل:
     * - ابتدا تعداد کتاب‌ها به صورت uint32_t
     * - سپس برای هر کتاب:
     *   - طول title (uint32_t) + کاراکترهای title
     *   - طول author (uint32_t) + کاراکترهای author
     *   - طول isbn (uint32_t) + کاراکترهای isbn
     *   - publicationYear (int32_t)
     *
     * @param filename نام فایل
     * @throw FileOperationException اگر فایل قابل باز کردن نباشد
     */
    void saveToBinaryFile(const std::string& filename) const;

    /**
     * @brief بارگذاری کتاب‌ها از فایل باینری
     *
     * @param filename نام فایل
     * @throw FileOperationException اگر فایل قابل باز کردن نباشد
     */
    void loadFromBinaryFile(const std::string& filename);

    /**
     * @brief ذخیره‌سازی کتاب‌ها در فایل CSV (Comma-Separated Values)
     *
     * فرمت CSV با کاما جدا می‌شود و قابل باز شدن در Excel است.
     * خط اول: هدر (Title,Author,ISBN,Year)
     * خطوط بعدی: یک کتاب در هر خط
     *
     * اگر فیلدها شامل کاما یا علامت نقل قول باشند، به طور خودکار
     * درون علامت نقل قول (") قرار می‌گیرند (RFC 4180).
     *
     * @param filename نام فایل (معمولاً با پسوند .csv)
     * @throw FileOperationException اگر فایل قابل باز کردن نباشد
     */
    void saveToCSVFile(const std::string& filename) const;

    /**
     * @brief بارگذاری کتاب‌ها از فایل CSV
     *
     * @param filename نام فایل CSV
     * @throw FileOperationException اگر فایل قابل باز کردن نباشد
     */
    void loadFromCSVFile(const std::string& filename);

    /**
     * @brief ذخیره‌سازی کتاب‌ها در فایل JSON (JavaScript Object Notation)
     *
     * فرمت JSON استاندارد و قابل تبادل بین سیستم‌های مختلف است.
     * ساختار خروجی:
     * {
     *   "books": [
     *     {"title":"...","author":"...","isbn":"...","year":2020},
     *     ...
     *   ]
     * }
     *
     * @param filename نام فایل (معمولاً با پسوند .json)
     * @throw FileOperationException اگر فایل قابل باز کردن نباشد
     */
    void saveToJSONFile(const std::string& filename) const;

    /**
     * @brief بارگذاری کتاب‌ها از فایل JSON
     *
     * @param filename نام فایل JSON
     * @throw FileOperationException اگر فایل قابل باز کردن نباشد
     */
    void loadFromJSONFile(const std::string& filename);

    // ============================================================
    // متدهای دسترسی (Accessors)
    // ============================================================

    /**
     * @brief تعداد کتاب‌های کتابخانه
     */
    size_t size() const;

    /**
     * @brief بررسی خالی بودن کتابخانه
     */
    bool empty() const;

    /**
     * @brief دسترسی فقط خواندنی به بردار کتاب‌ها
     */
    const std::vector<Book>& getBooks() const;
};

#endif  // LIBRARY_H
