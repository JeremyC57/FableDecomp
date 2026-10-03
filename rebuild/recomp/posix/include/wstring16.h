/* POSIX build: UTF-16 strings for the host. wchar_t is 16-bit (-fshort-wchar), but the C++
   library's std::wstring is precompiled for its native 32-bit wchar_t, so the host uses
   basic_string with these traits instead (nothing of it is precompiled), and the C library's
   wide-string functions are replaced by 16-bit versions (w32_wcslen, ...; posix/w32/wide.c). */
#pragma once
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cwchar>
#include <ios>
#include <string>

namespace host {
struct WTraits {
    using char_type = wchar_t;
    using int_type = uint32_t;
    using off_type = std::streamoff;
    using pos_type = std::streampos;
    using state_type = std::mbstate_t;
    static constexpr void assign(char_type& a, const char_type& b) noexcept { a = b; }
    static constexpr bool eq(char_type a, char_type b) noexcept { return a == b; }
    static constexpr bool lt(char_type a, char_type b) noexcept { return static_cast<uint16_t>(a) < static_cast<uint16_t>(b); }
    static constexpr int compare(const char_type* a, const char_type* b, size_t n) noexcept {
        for (size_t i = 0; i < n; ++i)
            if (a[i] != b[i]) return lt(a[i], b[i]) ? -1 : 1;
        return 0;
    }
    static constexpr size_t length(const char_type* s) noexcept {
        size_t n = 0;
        while (s[n]) ++n;
        return n;
    }
    static constexpr const char_type* find(const char_type* s, size_t n, const char_type& c) noexcept {
        for (size_t i = 0; i < n; ++i)
            if (s[i] == c) return s + i;
        return nullptr;
    }
    static char_type* move(char_type* d, const char_type* s, size_t n) noexcept { return static_cast<char_type*>(std::memmove(d, s, n * sizeof(char_type))); }
    static char_type* copy(char_type* d, const char_type* s, size_t n) noexcept { return static_cast<char_type*>(std::memcpy(d, s, n * sizeof(char_type))); }
    static constexpr char_type* assign(char_type* d, size_t n, char_type c) noexcept {
        for (size_t i = 0; i < n; ++i) d[i] = c;
        return d;
    }
    static constexpr char_type to_char_type(int_type c) noexcept { return static_cast<char_type>(c); }
    static constexpr int_type to_int_type(char_type c) noexcept { return static_cast<uint16_t>(c); }
    static constexpr bool eq_int_type(int_type a, int_type b) noexcept { return a == b; }
    static constexpr int_type eof() noexcept { return 0xFFFFFFFFu; }
    static constexpr int_type not_eof(int_type c) noexcept { return c == eof() ? 0 : c; }
};
using wstring = std::basic_string<wchar_t, WTraits>;
}  // namespace host

/* 16-bit replacements for the C library's wide-string functions (posix/w32/wide.c). */
extern "C" {
size_t w32_wcslen(const wchar_t*);
int w32_wcscmp(const wchar_t*, const wchar_t*);
int w32_wcsncmp(const wchar_t*, const wchar_t*, size_t);
wchar_t* w32_wcscpy(wchar_t*, const wchar_t*);
wchar_t* w32_wcsncpy(wchar_t*, const wchar_t*, size_t);
wchar_t* w32_wcscat(wchar_t*, const wchar_t*);
wchar_t* w32_wcschr(const wchar_t*, wchar_t);
wchar_t* w32_wcsrchr(const wchar_t*, wchar_t);
wchar_t* w32_wcsstr(const wchar_t*, const wchar_t*);
long w32_wcstol(const wchar_t*, wchar_t**, int);
unsigned long w32_wcstoul(const wchar_t*, wchar_t**, int);
double w32_wcstod(const wchar_t*, wchar_t**);
int w32_iswspace(uint32_t);
uint32_t w32_towlower(uint32_t);
uint32_t w32_towupper(uint32_t);
}
namespace std {
using ::w32_wcslen; using ::w32_wcscmp; using ::w32_wcsncmp; using ::w32_wcscpy; using ::w32_wcsncpy; using ::w32_wcscat;
using ::w32_wcschr; using ::w32_wcsrchr; using ::w32_wcsstr; using ::w32_wcstol; using ::w32_wcstoul; using ::w32_wcstod;
using ::w32_iswspace; using ::w32_towlower; using ::w32_towupper;
}
#define wcslen w32_wcslen
#define wcscmp w32_wcscmp
#define wcsncmp w32_wcsncmp
#define wcscpy w32_wcscpy
#define wcsncpy w32_wcsncpy
#define wcscat w32_wcscat
#define wcschr w32_wcschr
#define wcsrchr w32_wcsrchr
#define wcsstr w32_wcsstr
#define wcstol w32_wcstol
#define wcstoul w32_wcstoul
#define wcstod w32_wcstod
#define iswspace w32_iswspace
#define towlower w32_towlower
#define towupper w32_towupper
