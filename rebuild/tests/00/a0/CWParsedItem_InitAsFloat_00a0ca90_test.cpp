// Behaviour test for 0x00A0CA90 CWParsedItem::InitAsFloat: the row's includes and body verbatim, on a zeroed item image. Digits before
// '.' build the whole part, after it the fraction (divided by 10^n), a '-' anywhere negates, only `size` wide characters are read (the
// text need not be terminated there), size 0 gives 0, characters are read 16 bits wide and zero-extended, Float is cleared before the
// text is read (text aliased onto Float), and the item becomes a float.
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stddef.h>
#define private public   // test access to the item's private fields only
#include "engine/CParsedItem.h"
#include "engine/CWParsedItem.h"

void CWParsedItem::InitAsFloat(const wchar_t* string, unsigned long size)
{
    wchar_t ch;
    float top = 0.0f;
    float bottom = 0.0f;
    float bottom_divide = 1.0f;
    bool doing_top = true;
    bool is_negative = false;
    unsigned long c1;
    Float = 0.0f;
    if (false) {} else for (c1 = 0, ch = string[c1]; c1 < size; ++c1, ch = string[c1]) {
        if (ch == L'-')
            is_negative = true;
        else if (ch == L'.')
            doing_top = false;
        else if (doing_top == true) {
            top *= 10.0f;
            top += ch - L'0';
        } else {
            bottom *= 10.0;
            bottom += ch - L'0';
            bottom_divide *= 10.0;
        }
    }
    Float = bottom / bottom_divide + top;
    if (is_negative == true)
        Float *= -1.0;
    Type = PARSED_ITEM_FLOAT;
}

static double g_item[sizeof(CWParsedItem) / 8 + 1];
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
static float ParseW(const wchar_t* w, unsigned long n)
{
    memset(g_item, 0, sizeof(g_item));
    CWParsedItem* item = reinterpret_cast<CWParsedItem*>(g_item);
    item->Float = 123.0f;
    item->InitAsFloat(w, n);
    if (item->Type != PARSED_ITEM_FLOAT) { printf("FAIL type\n"); g_fail = 1; }
    return item->Float;
}
static float Parse(const char* s, unsigned long n)
{
    wchar_t w[64];
    unsigned long i;
    for (i = 0; s[i]; ++i) w[i] = (wchar_t)(unsigned char)s[i];
    w[i] = 0;
    return ParseW(w, n);
}
static int Near(float a, float b) { return fabs(a - b) <= 1e-4f * (fabs(b) + 1.0f); }
int main()
{
    check(Near(Parse("12.25", 5), 12.25f), "whole and fraction");
    check(Near(Parse("-3.5", 4), -3.5f), "leading minus");
    check(Near(Parse("7-", 2), -7.0f), "a minus anywhere negates");
    check(Near(Parse("1234", 4), 1234.0f), "whole only");
    check(Near(Parse(".0625", 5), 0.0625f), "fraction only");
    check(Near(Parse("98.76xyz", 3), 98.0f), "only size characters read (98.)");
    check(Near(Parse("98.76", 4), 98.7f), "size cuts the fraction");
    check(Near(Parse("98.76", 5), 98.76f), "size reaches the last digit");
    check(Parse("5", 0) == 0.0f, "size 0 -> 0 (Float cleared)");
    check(Near(Parse("0.5", 3), 0.5f), "leading zero");
    check(Near(Parse("1.2.5", 5), 1.25f), "a second '.' keeps the fraction going");
    {
        wchar_t w[3];
        w[0] = (wchar_t)0x0131; w[1] = (wchar_t)'2'; w[2] = 0;          // 16-bit char: 0x131 - '0' = 257
        check(Near(ParseW(w, 2), 2572.0f), "16-bit characters, zero-extended");
        w[0] = (wchar_t)0x8030; w[1] = (wchar_t)'-'; w[2] = 0;          // 0x8030 - '0' = 32768 (not negative)
        check(Near(ParseW(w, 2), -32768.0f), "high 16-bit character zero-extends");
    }
    {
        // The initial `Float = 0` store comes before the first read of the text (retail 0x00A0CAB1 `mov [esi+4],edx`, then 0x00A0CAB4
        // `mov ax,[edi]`; every later character is re-read from memory): text aliased onto the item's own Float word reads the
        // cleared word (L'\0', L'\0' -> top = -48, then -528), not the preset L'5', L'7' (57). MSVC test only (no type-based aliasing).
        memset(g_item, 0, sizeof(g_item));
        CWParsedItem* item = reinterpret_cast<CWParsedItem*>(g_item);
        item->Integer = 0x00370035;
        item->InitAsFloat(reinterpret_cast<const wchar_t*>(&item->Integer), 2);
        check(Near(item->Float, -528.0f), "Float cleared before the text is read (aliased text)");
        check(item->Type == PARSED_ITEM_FLOAT, "aliased: type");
    }
    if (!g_fail) printf("00a0ca90_TEST PASS\n");
    return g_fail;
}