/* 16-bit (UTF-16) versions of the C library's wide-string functions; compiled with
   -fshort-wchar. Case mapping covers ASCII, Latin-1 and the common European blocks. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static uint32_t up16(uint32_t c) {
    if (c >= 'a' && c <= 'z') return c - 32;
    if (c >= 0xE0 && c <= 0xFE && c != 0xF7) return c - 32;
    if (c == 0xFF) return 0x178;
    if (c >= 0x100 && c < 0x180 && (c & 1)) return c - 1;
    if (c >= 0x3B1 && c <= 0x3C9 && c != 0x3C2) return c - 32;
    if (c >= 0x430 && c <= 0x44F) return c - 32;
    if (c >= 0x450 && c <= 0x45F) return c - 80;
    return c;
}
static uint32_t low16(uint32_t c) {
    if (c >= 'A' && c <= 'Z') return c + 32;
    if (c >= 0xC0 && c <= 0xDE && c != 0xD7) return c + 32;
    if (c == 0x178) return 0xFF;
    if (c >= 0x100 && c < 0x180 && !(c & 1)) return c + 1;
    if (c >= 0x391 && c <= 0x3A9) return c + 32;
    if (c >= 0x410 && c <= 0x42F) return c + 32;
    if (c >= 0x400 && c <= 0x40F) return c + 80;
    return c;
}
uint32_t w32_towupper(uint32_t c) { return up16(c); }
uint32_t w32_towlower(uint32_t c) { return low16(c); }
int w32_iswspace(uint32_t c) { return c == ' ' || (c >= 9 && c <= 13) || c == 0xA0 || c == 0x2000 || c == 0x3000 || c == 0x200B; }

size_t w32_wcslen(const wchar_t* s) { size_t n = 0; while (s[n]) ++n; return n; }
int w32_wcscmp(const wchar_t* a, const wchar_t* b) {
    for (;; ++a, ++b) {
        const uint16_t x = (uint16_t)*a, y = (uint16_t)*b;
        if (x != y) return x < y ? -1 : 1;
        if (!x) return 0;
    }
}
int w32_wcsncmp(const wchar_t* a, const wchar_t* b, size_t n) {
    for (; n; --n, ++a, ++b) {
        const uint16_t x = (uint16_t)*a, y = (uint16_t)*b;
        if (x != y) return x < y ? -1 : 1;
        if (!x) return 0;
    }
    return 0;
}
wchar_t* w32_wcscpy(wchar_t* d, const wchar_t* s) { wchar_t* r = d; while ((*d++ = *s++)) {} return r; }
wchar_t* w32_wcsncpy(wchar_t* d, const wchar_t* s, size_t n) {
    size_t i = 0;
    for (; i < n && s[i]; ++i) d[i] = s[i];
    for (; i < n; ++i) d[i] = 0;
    return d;
}
wchar_t* w32_wcscat(wchar_t* d, const wchar_t* s) { w32_wcscpy(d + w32_wcslen(d), s); return d; }
wchar_t* w32_wcschr(const wchar_t* s, wchar_t c) {
    for (;; ++s) {
        if (*s == c) return (wchar_t*)s;
        if (!*s) return NULL;
    }
}
wchar_t* w32_wcsrchr(const wchar_t* s, wchar_t c) {
    const wchar_t* r = NULL;
    for (;; ++s) {
        if (*s == c) r = s;
        if (!*s) return (wchar_t*)r;
    }
}
wchar_t* w32_wcsstr(const wchar_t* h, const wchar_t* n) {
    if (!*n) return (wchar_t*)h;
    for (; *h; ++h) {
        size_t i = 0;
        while (n[i] && h[i] == n[i]) ++i;
        if (!n[i]) return (wchar_t*)h;
    }
    return NULL;
}

/* Numbers: narrow the (ASCII) digits and use the C library. */
static size_t narrowNum(const wchar_t* s, char* buf, size_t cap) {
    size_t i = 0;
    while (i + 1 < cap && s[i] && (uint16_t)s[i] < 0x80) { buf[i] = (char)s[i]; ++i; }
    buf[i] = 0;
    return i;
}
long w32_wcstol(const wchar_t* s, wchar_t** end, int base) {
    char buf[128], *e;
    narrowNum(s, buf, sizeof buf);
    const long v = strtol(buf, &e, base);
    if (end) *end = (wchar_t*)s + (e - buf);
    return v;
}
unsigned long w32_wcstoul(const wchar_t* s, wchar_t** end, int base) {
    char buf[128], *e;
    narrowNum(s, buf, sizeof buf);
    const unsigned long v = strtoul(buf, &e, base);
    if (end) *end = (wchar_t*)s + (e - buf);
    return v;
}
double w32_wcstod(const wchar_t* s, wchar_t** end) {
    char buf[128], *e;
    narrowNum(s, buf, sizeof buf);
    const double v = strtod(buf, &e);
    if (end) *end = (wchar_t*)s + (e - buf);
    return v;
}
