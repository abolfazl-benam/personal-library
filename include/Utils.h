/**
 * @file Utils.h
 * @brief توابع کمکی برای اعتبارسنجی ورودی و عملیات رایج رابط کاربری
 *
 * این فایل توابع کاربردی فراوانی را در فضای نام Utils فراهم می‌کند
 * که در سراسر برنامه برای خواندن ورودی امن از کاربر، اعتبارسنجی
 * فیلدها و پاکسازی صفحه استفاده می‌شوند.
 *
 * نکته: تمام پیام‌های نمایش داده شده به کاربر به زبان انگلیسی هستند
 * تا در تمام کنسول‌ها بدون مشکل encoding نمایش داده شوند.
 */

#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <limits>
#include <algorithm>
#include <cctype>
#include <sstream>

// ============================================================
// پشتیبانی Cross-Platform (ویندوز و لینوکس)
// ============================================================
#if defined(_WIN32) || defined(_WIN64)
    // ویندوز
    #include <io.h>          // برای _isatty
    #include <cstdio>        // برای _fileno
    #define IS_TTY(x) _isatty(x)
    #define STDIN_FD _fileno(stdin)
    #define CLEAR_COMMAND "cls"
#else
    // لینوکس و مک
    #include <unistd.h>      // برای isatty
    #define IS_TTY(x) isatty(x)
    #define STDIN_FD STDIN_FILENO
    #define CLEAR_COMMAND "clear"
#endif

#include "BookExceptions.h"

/**
 * @namespace Utils
 * @brief فضای نام شامل توابع کمکی پروژه
 */
namespace Utils {

    // ============================================================
    // توابع اعتبارسنجی (Validation Functions)
    // ============================================================

    /**
     * @brief بررسی می‌کند که آیا رشته خالی یا فقط شامل فضای خالی است
     * @param str رشته ورودی
     * @return true اگر رشته خالی یا فقط فضای خالی باشد
     */
    inline bool isEmptyOrWhitespace(const std::string& str) {
        return str.empty() ||
               std::all_of(str.begin(), str.end(),
                           [](unsigned char c) { return std::isspace(c); });
    }

    /**
     * @brief بررسی اعتبار ISBN
     *
     * ISBN معتبر باید:
     * - فقط شامل ارقام و خط تیره ('-') باشد
     * - بعد از حذف خط تیره‌ها، دقیقاً 10 یا 13 رقم داشته باشد
     *
     * @param isbn رشته ISBN برای بررسی
     * @return true اگر ISBN معتبر باشد
     */
    inline bool isValidISBN(const std::string& isbn) {
        int digitCount = 0;
        for (char c : isbn) {
            if (std::isdigit(static_cast<unsigned char>(c))) {
                ++digitCount;
            } else if (c != '-') {
                return false;  // کاراکتر غیرمجاز
            }
        }
        return digitCount == 10 || digitCount == 13;  // ISBN-10 یا ISBN-13
    }

    /**
     * @brief بررسی اعتبار سال انتشار
     *
     * سال باید بین 1450 (اختراع چاپ) و 2026 (سال جاری + 1) باشد.
     *
     * @param year سال برای بررسی
     * @return true اگر سال معتبر باشد
     */
    inline bool isValidYear(int year) {
        const int MIN_YEAR = 1450;
        const int MAX_YEAR = 2026;
        return year >= MIN_YEAR && year <= MAX_YEAR;
    }

    /**
     * @brief حذف خط تیره از ISBN برای ذخیره‌سازی یکنواخت
     * @param isbn رشته ISBN
     * @return ISBN بدون خط تیره
     */
    inline std::string normalizeISBN(const std::string& isbn) {
        std::string result;
        result.reserve(isbn.size());
        for (char c : isbn) {
            if (c != '-') {
                result += c;
            }
        }
        return result;
    }

    // ============================================================
    // توابع خواندن ورودی امن (Safe Input Functions)
    // ============================================================

    /**
     * @brief پاکسازی بافر ورودی برای جلوگیری از خطاهای خواندن
     *
     * اگر کاربر به جای عدد، رشته وارد کند، بافر ورودی آلوده می‌شود.
     * این تابع بافر را پاک می‌کند تا عملیات خواندن بعدی بدون مشکل انجام شود.
     */
    inline void clearInputStream() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    /**
     * @brief توقف اجرا تا زمانی که کاربر Enter را فشار دهد
     *
     * برای کاربرد در منوها، تا کاربر بتواند خروجی را قبل از پاک شدن صفحه بخواند.
     * اگر ورودی از فایل pipe شده (non-interactive)، این تابع فورا برمی‌گردد
     * تا برنامه در حلقه بی‌نهایت نیفتد.
     */
    inline void pauseScreen() {
        if (!IS_TTY(STDIN_FD) || std::cin.eof()) {
            return;  // در حالت non-interactive، منتظر نمی‌مانیم
        }
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    /**
     * @brief پاکسازی صفحه کنسول
     *
     * از دستور سیستم cls روی ویندوز و clear روی لینوکس استفاده می‌کند.
     * در حالت non-interactive، کاری نمی‌کند.
     */
    inline void clearScreen() {
        if (!IS_TTY(STDIN_FD)) {
            return;
        }
        std::system(CLEAR_COMMAND);
    }

    /**
     * @brief خواندن یک رشته غیرخالی از کاربر
     *
     * @param prompt پیام نمایش داده شده به کاربر
     * @param fieldName نام فیلد (برای پیام خطا)
     * @return رشته وارد شده توسط کاربر
     */
    inline std::string getNonEmptyString(const std::string& prompt,
                                          const std::string& fieldName) {
        std::string input;
        while (true) {
            std::cout << prompt;
            std::getline(std::cin, input);

            if (isEmptyOrWhitespace(input)) {
                std::cerr << "[X] Field '" << fieldName
                          << "' cannot be empty. Please try again.\n";
                continue;
            }
            return input;
        }
    }

    /**
     * @brief خواندن یک عدد صحیح در محدوده مشخص
     *
     * @param prompt پیام نمایش داده شده به کاربر
     * @param min حداقل مقدار مجاز
     * @param max حداکثر مقدار مجاز
     * @return عدد صحیح وارد شده
     */
    inline int getIntInRange(const std::string& prompt, int min, int max) {
        int value;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value) {
                if (value >= min && value <= max) {
                    clearInputStream();
                    return value;
                }
                std::cerr << "[X] Value must be between " << min << " and "
                          << max << ". Please try again.\n";
            } else {
                std::cerr << "[X] Invalid input. Please enter an integer.\n";
            }
            clearInputStream();
        }
    }

    /**
     * @brief خواندن یک کاراکتر yes/no از کاربر
     * @param prompt پیام نمایش داده شده به کاربر
     * @return true اگر کاربر y/Y وارد کند، false در غیر این صورت
     */
    inline bool getYesNo(const std::string& prompt) {
        char choice;
        while (true) {
            std::cout << prompt;
            std::cin >> choice;
            clearInputStream();
            if (choice == 'y' || choice == 'Y') return true;
            if (choice == 'n' || choice == 'N') return false;
            std::cerr << "[X] Please enter 'y' or 'n'.\n";
        }
    }

    /**
     * @brief خواندن ISBN معتبر از کاربر
     *
     * @param prompt پیام نمایش داده شده به کاربر
     * @return ISBN نرمال‌شده (بدون خط تیره)
     */
    inline std::string getValidISBN(const std::string& prompt) {
        std::string isbn;
        while (true) {
            isbn = getNonEmptyString(prompt, "ISBN");
            if (isValidISBN(isbn)) {
                return normalizeISBN(isbn);
            }
            std::cerr << "[X] Invalid ISBN. Must have 10 or 13 digits "
                         "(you can use hyphens).\n";
        }
    }

    /**
     * @brief خواندن سال انتشار معتبر از کاربر
     * @return سال معتبر
     */
    inline int getValidYear() {
        return getIntInRange("Publication year (1450-2026): ", 1450, 2026);
    }

    /**
     * @brief تبدیل رشته به حروف کوچک (برای جستجوی case-insensitive)
     */
    inline std::string toLower(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        return result;
    }

}  // namespace Utils

#endif  // UTILS_H
