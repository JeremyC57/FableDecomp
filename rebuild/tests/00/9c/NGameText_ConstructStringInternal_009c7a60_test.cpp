// Behaviour test for 0x009C7A60 NGameText::ConstructStringInternal(CWideString&, long, ...).
// NF146 (2026-11-20, q4285): rewritten on the shared header the row now uses (fable_string.h with FABLE_RETAIL_CWIDESTRING; the row also
//   expands engine/GFIntToWideString.h). It links the row object and defines the out-of-line callees the row binds (CWideString default /
//   copy / text constructors, destructor, Sprintf, operator+(const wchar_t*, const CWideString&), Contains, Replace, and the separator
//   formatter 0x00A0DAC0) as small fakes over a string table, then checks the %1..%n replacement, the -1 terminator, the early false
//   return when a placeholder is missing, thousands separators, and that every temporary string is destroyed.
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#define FABLE_RETAIL_CWIDESTRING
#include "fable_string.h"

namespace NGameText { bool __cdecl ConstructStringInternal(CWideString& output, long first, ...); }

struct Slot { const CWideString* owner; unsigned short text[128]; };
static Slot g_slots[64];
static int g_alive = 0, g_replacements = 0, g_fail = 0;
static unsigned short g_separator = ',';
static unsigned Len(const unsigned short* p) { unsigned n = 0; while (p[n]) ++n; return n; }
static Slot* Find(const CWideString* s) { for (int i = 0; i < 64; ++i) if (g_slots[i].owner == s) return &g_slots[i]; return 0; }
static Slot* Make(const CWideString* s, const unsigned short* t)
{
    for (int i = 0; i < 64; ++i) if (!g_slots[i].owner) {
        g_slots[i].owner = s; unsigned n = t ? Len(t) : 0; if (n > 127) { ++g_fail; n = 127; }
        if (t) memcpy(g_slots[i].text, t, n * 2); g_slots[i].text[n] = 0; ++g_alive; return &g_slots[i];
    }
    ++g_fail; return 0;
}
static const unsigned short* T(const CWideString& s) { Slot* x = Find(&s); if (!x) { ++g_fail; static unsigned short z = 0; return &z; } return x->text; }
static void SetT(const CWideString& s, const unsigned short* t) { Slot* x = Find(&s); unsigned n = Len(t); if (!x || n > 127) { ++g_fail; return; } memmove(x->text, t, n * 2); x->text[n] = 0; }
static void Ascii(unsigned short* d, const char* s) { while ((*d++ = (unsigned char)*s++) != 0) { } }
static bool Eq(const CWideString& s, const char* a) { const unsigned short* t = T(s); unsigned i = 0; for (; a[i]; ++i) if (t[i] != (unsigned char)a[i]) return false; return t[i] == 0; }
static bool Starts(const unsigned short* a, const unsigned short* b) { for (; *b; ++a, ++b) if (*a != *b) return false; return true; }

CWideString::CWideString() { Make(this, 0); }
CWideString::CWideString(const CWideString& other) { Make(this, T(other)); }
CWideString::CWideString(const wchar_t* text) { Make(this, (const unsigned short*)text); }
CWideString::~CWideString() { Slot* x = Find(this); if (x) { x->owner = 0; --g_alive; } else ++g_fail; }
void __cdecl CWideString::Sprintf(const wchar_t* format, ...)
{
    if (!(format[0] == '%' && format[1] == 'd' && format[2] == 0)) ++g_fail;
    va_list args; va_start(args, format); long n = va_arg(args, long); va_end(args);
    char b[32]; sprintf(b, "%ld", n); unsigned short w[32]; Ascii(w, b); SetT(*this, w);
}
CWideString FABLE_FASTCALL operator+(const wchar_t* left, const CWideString& right)
{
    unsigned short b[128]; unsigned n = Len((const unsigned short*)left); memcpy(b, left, n * 2); memcpy(b + n, T(right), (Len(T(right)) + 1) * 2);
    return CWideString((const wchar_t*)b);
}
bool CWideString::Contains(const CWideString& other) const
{
    const unsigned short* h = T(*this); const unsigned short* n = T(other);
    for (; *h; ++h) if (Starts(h, n)) return true; return false;
}
void CWideString::Replace(const CWideString& search, const CWideString& replacement)
{
    ++g_replacements; const unsigned short* a = T(*this); const unsigned short* f = T(search); const unsigned short* r = T(replacement);
    unsigned short b[128]; unsigned n = 0, k = Len(f);
    while (*a) { if (k && Starts(a, f)) { for (unsigned j = 0; r[j]; ++j) b[n++] = r[j]; a += k; } else b[n++] = *a++; }
    b[n] = 0; SetT(*this, b);
}
void __fastcall FormatNumberWithSeparators(long value, unsigned short* text)
{
    char d[32]; sprintf(d, "%ld", value); int len = (int)strlen(d), o = 0;
    for (int i = 0; i < len; ++i) { text[o++] = (unsigned char)d[i]; int left = len - 1 - i; if (left > 0 && left % 3 == 0 && d[i] != '-') text[o++] = g_separator; }
    text[o] = 0;
}

int main()
{
    using NGameText::ConstructStringInternal;
    unsigned short w[64];
    bool ok = true;
    { Ascii(w, "%1 then %2"); CWideString s((const wchar_t*)w);
      ok = ok && ConstructStringInternal(s, -1) && Eq(s, "%1 then %2") && g_replacements == 0;
      ok = ok && ConstructStringInternal(s, 7, -8, -1) && Eq(s, "7 then -8") && g_replacements == 2; }
    ok = ok && g_alive == 0;
    { Ascii(w, "%1 %1 %2"); CWideString s((const wchar_t*)w);
      ok = ok && ConstructStringInternal(s, 0, 2147483647L, -1) && Eq(s, "0 0 2,147,483,647"); }
    { Ascii(w, "%1 done"); CWideString s((const wchar_t*)w); int before = g_replacements;
      ok = ok && !ConstructStringInternal(s, 1234L, 9, -1) && Eq(s, "1,234 done") && g_replacements == before + 1; }
    { Ascii(w, "unchanged"); CWideString s((const wchar_t*)w); int before = g_replacements;
      ok = ok && !ConstructStringInternal(s, 5, -1) && Eq(s, "unchanged") && g_replacements == before; }
    { Ascii(w, "%1"); CWideString s((const wchar_t*)w); g_separator = '.';
      ok = ok && ConstructStringInternal(s, 1234, -1) && Eq(s, "1.234"); g_separator = ','; }
    ok = ok && g_alive == 0 && g_fail == 0;
    if (ok) printf("009c7a60_TEST PASS\n"); else printf("009c7a60_TEST FAIL alive=%d fail=%d repl=%d\n", g_alive, g_fail, g_replacements);
    return ok ? 0 : 1;
}
