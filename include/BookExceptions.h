/**
 * @file BookExceptions.h
 * @brief تعریف کلاس‌های استثنای سفارشی برای پروژه مدیریت کتابخانه
 *
 * این فایل یک سلسله‌مراتب از استثناهای (Exceptions) سفارشی تعریف می‌کند
 * که برای مدیریت خطاها در طول اجرای برنامه استفاده می‌شوند.
 * استفاده از استثناهای سفارشی به ما اجازه می‌دهد خطاهای مختلف را
 * به صورت دقیق‌تر مدیریت کنیم و پیام‌های معناداری به کاربر نمایش دهیم.
 *
 * نکته: پیام‌های خطا به زبان انگلیسی هستند تا در تمام کنسول‌ها
 * (به خصوص CMD ویندوز) بدون مشکل encoding نمایش داده شوند.
 */

#ifndef BOOK_EXCEPTIONS_H
#define BOOK_EXCEPTIONS_H

#include <stdexcept>  // برای std::runtime_error
#include <string>

/**
 * @brief کلاس پایه برای تمام استثناهای مربوط به کتاب
 *
 * این کلاس از std::runtime_error ارث‌بری می‌کند تا بتوان آن را با
 * مکانیزم try/catch استاندارد C++ مدیریت کرد.
 */
class BookException : public std::runtime_error {
public:
    explicit BookException(const std::string& message)
        : std::runtime_error(message) {}
};

/**
 * @brief استثنای ISBN نامعتبر
 *
 * زمانی پرتاب می‌شود که فرمت ISBN وارد شده معتبر نباشد.
 * ISBN معتبر باید 10 یا 13 رقم داشته باشد و ممکن است شامل خط تیره باشد.
 */
class InvalidISBNException : public BookException {
public:
    explicit InvalidISBNException(const std::string& isbn)
        : BookException("Invalid ISBN: '" + isbn +
                        "'. ISBN must have 10 or 13 digits.") {}
};

/**
 * @brief استثنای سال انتشار نامعتبر
 *
 * زمانی پرتاب می‌شود که سال وارد شده خارج از محدوده منطقی باشد.
 * محدوده منطقی: 1450 (اختراع چاپ) تا 2026 (سال جاری + 1)
 */
class InvalidYearException : public BookException {
public:
    explicit InvalidYearException(int year)
        : BookException("Invalid publication year: " + std::to_string(year) +
                        ". Year must be between 1450 and 2026.") {}
};

/**
 * @brief استثنای فیلد خالی
 *
 * زمانی پرتاب می‌شود که یک فیلد ضروری (مانند عنوان یا نویسنده) خالی باشد.
 */
class EmptyFieldException : public BookException {
public:
    explicit EmptyFieldException(const std::string& fieldName)
        : BookException("Field '" + fieldName + "' cannot be empty.") {}
};

/**
 * @brief استثنای کتاب یافت نشد
 *
 * زمانی پرتاب می‌شود که کتابی با ISBN مشخص شده در کتابخانه وجود نداشته باشد.
 */
class BookNotFoundException : public BookException {
public:
    explicit BookNotFoundException(const std::string& isbn)
        : BookException("Book with ISBN '" + isbn + "' not found.") {}
};

/**
 * @brief استثنای ISBN تکراری
 *
 * زمانی پرتاب می‌شود که سعی شود کتابی با ISBN تکراری اضافه شود.
 */
class DuplicateISBNException : public BookException {
public:
    explicit DuplicateISBNException(const std::string& isbn)
        : BookException("Book with ISBN '" + isbn +
                        "' already exists. ISBN must be unique.") {}
};

/**
 * @brief استثنای خطای عملیات فایل
 *
 * زمانی پرتاب می‌شود که عملیات خواندن یا نوشتن فایل با شکست مواجه شود.
 */
class FileOperationException : public BookException {
public:
    explicit FileOperationException(const std::string& operation,
                                     const std::string& filename)
        : BookException("Error during " + operation +
                        " operation on file '" + filename + "'.") {}
};

#endif  // BOOK_EXCEPTIONS_H
