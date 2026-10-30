// Behaviour test for the word-wrap character classes IsNonBeginningChar (0x00A0FE80), IsNonEndingChar (0x00A10170),
// IsEastAsianChar (0x00A10240), WordWrap_CanBreakLineAt (0x00A102B0) and WordWrap_TokenizeW (0x00A10720) (reduced tables).
#include <cstdio>
#include <cstring>
static unsigned long g_WordWrapOptions;
#define WS(c) ((c) == 0x0009 || (c) == 0x000D || (c) == 0x0020 || (c) == 0x3000)
static bool IsNonBeginningChar(unsigned short ch) { if (!(g_WordWrapOptions & 1)) return false; return ch == 0x3002 || ch == 0x002C || ch == 0xFF0C; }
static bool IsNonEndingChar(unsigned short c) { if (!(g_WordWrapOptions & 1)) return false; return c == 0x300C || c == 0x0028; }
static bool IsEastAsianChar(unsigned short c)
{
    if (g_WordWrapOptions & 4) { if ((c >= 0x1100 && c <= 0x11FF) || (c >= 0x3130 && c <= 0x318F) || (c >= 0xAC00 && c <= 0xD7A3)) return false; }
    return (c >= 0x1100 && c <= 0x11FF) || (c >= 0x3000 && c <= 0xD7AF) || (c >= 0xF900 && c <= 0xFAFF) || (c >= 0xFF00 && c <= 0xFFDC);
}
static bool CanBreakLineAt(const unsigned short* text, const unsigned short* start)
{
    if (text == start) return false;
    return (WS(*text) || IsEastAsianChar(*text) || IsEastAsianChar(text[-1])) && !IsNonBeginningChar(*text) && !IsNonEndingChar(text[-1]);
}
static long TokenCount(const unsigned short* text)
{
    long count = 0, n = 0; while (text[n]) ++n;
    const unsigned short* cursor = text + 1;
    for (long remaining = n; remaining > 0; --remaining, ++cursor) if (CanBreakLineAt(cursor, 0) || *cursor == 0) ++count;
    return count;
}
int main()
{
    g_WordWrapOptions = 1;
    static const unsigned short ab[] = { 'a', ' ', 'b', 0 }, cjk[] = { 0x4E00, 0x4E01, 0x3002, 0 }, open_[] = { 0x300C, 0x4E00, 0 };
    if (!IsEastAsianChar(0x4E00) || IsEastAsianChar('a') || !IsNonBeginningChar(0x3002) || !IsNonEndingChar(0x300C)) { std::printf("00a10720_TEST FAIL\n"); return 1; }
    if (CanBreakLineAt(ab, ab) || !CanBreakLineAt(ab + 1, ab) || CanBreakLineAt(ab + 2, ab)) { std::printf("00a10720_TEST FAIL\n"); return 1; }
    if (!CanBreakLineAt(cjk + 1, cjk) || CanBreakLineAt(cjk + 2, cjk) || CanBreakLineAt(open_ + 1, open_)) { std::printf("00a10720_TEST FAIL\n"); return 1; }
    g_WordWrapOptions = 4;
    if (IsEastAsianChar(0xAC00) || !IsEastAsianChar(0x4E00) || IsNonBeginningChar(0x3002)) { std::printf("00a10720_TEST FAIL\n"); return 1; }
    g_WordWrapOptions = 0;
    if (TokenCount(ab) != 2) { std::printf("00a10720_TEST FAIL\n"); return 1; }
    std::printf("00a10720_TEST PASS\n"); return 0;
}