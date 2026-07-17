/**
 * @file Book.h
 * @brief تعریف کلاس Book برای مدیریت اطلاعات یک کتاب
 *
 * این کلاس نشان‌دهنده یک کتاب در سیستم کتابخانه شخصی است.
 * هر کتاب دارای چهار ویژگی اصلی است: عنوان، نویسنده، ISBN و سال انتشار.
 *
 * اصول OOP استفاده شده:
 * - Encapsulation: ویژگی‌ها private و فقط از طریق getter/setter قابل دسترسی‌اند
 * - Validation: setterها ورودی را اعتبارسنجی می‌کنند
 * - Operator Overloading: عملگرهای == و << و >> سربارگذاری شده‌اند
 */

#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <iostream>
#include "BookExceptions.h"

/**
 * @class Book
 * @brief کلاس نشان‌دهنده یک کتاب در کتابخانه
 */
class Book {
private:
    std::string title;            ///< عنوان کتاب
    std::string author;           ///< نام نویسنده
    std::string isbn;             ///< شماره ISBN (یکتا)
    int publicationYear;          ///< سال انتشار

public:
    // ============================================================
    // سازنده‌ها (Constructors)
    // ============================================================

    /**
     * @brief سازنده پیش‌فرض
     *
     * یک کتاب با مقادیر پیش‌فرض (رشته‌های خالی و سال ۰) می‌سازد.
     * برای ساخت آرایه‌ها و بردارها از کتاب‌ها ضروری است.
     */
    Book();

    /**
     * @brief سازنده با پارامتر
     *
     * یک کتاب با تمام ویژگی‌ها می‌سازد. اعتبارسنجی در setterها انجام می‌شود.
     *
     * @param title عنوان کتاب
     * @param author نام نویسنده
     * @param isbn شماره ISBN
     * @param publicationYear سال انتشار
     * @throw EmptyFieldException اگر عنوان یا نویسنده خالی باشد
     * @throw InvalidISBNException اگر ISBN نامعتبر باشد
     * @throw InvalidYearException اگر سال نامعتبر باشد
     */
    Book(const std::string& title,
         const std::string& author,
         const std::string& isbn,
         int publicationYear);

    // ============================================================
    // Getter ها (دسترسی به ویژگی‌ها)
    // ============================================================

    std::string getTitle() const;
    std::string getAuthor() const;
    std::string getISBN() const;
    int getPublicationYear() const;

    // ============================================================
    // Setter ها (تغییر ویژگی‌ها با اعتبارسنجی)
    // ============================================================

    /**
     * @brief تنظیم عنوان کتاب
     * @throw EmptyFieldException اگر عنوان خالی باشد
     */
    void setTitle(const std::string& title);

    /**
     * @brief تنظیم نام نویسنده
     * @throw EmptyFieldException اگر نام خالی باشد
     */
    void setAuthor(const std::string& author);

    /**
     * @brief تنظیم ISBN کتاب
     * @throw InvalidISBNException اگر ISBN نامعتبر باشد
     */
    void setISBN(const std::string& isbn);

    /**
     * @brief تنظیم سال انتشار
     * @throw InvalidYearException اگر سال نامعتبر باشد
     */
    void setPublicationYear(int year);

    // ============================================================
    // متدهای کمکی (Utility Methods)
    // ============================================================

    /**
     * @brief نمایش اطلاعات کتاب در خروجی استاندارد
     */
    void display() const;

    /**
     * @brief تبدیل کتاب به رشته فرمت‌بندی شده
     * @return رشته‌ای شامل اطلاعات کامل کتاب
     */
    std::string toString() const;

    // ============================================================
    // Operator Overloading (سربارگذاری عملگرها)
    // ============================================================

    /**
     * @brief عملگر برابری - دو کتاب بر اساس ISBN مقایسه می‌شوند
     * @return true اگر ISBN هر دو کتاب یکسان باشد
     */
    bool operator==(const Book& other) const;

    /**
     * @brief عملگر خروجی - برای چاپ راحت کتاب با std::cout
     *
     * مثال: std::cout << book;
     */
    friend std::ostream& operator<<(std::ostream& os, const Book& book);

    /**
     * @brief عملگر ورودی - برای خواندن کتاب از stream
     */
    friend std::istream& operator>>(std::istream& is, Book& book);
};

#endif  // BOOK_H
