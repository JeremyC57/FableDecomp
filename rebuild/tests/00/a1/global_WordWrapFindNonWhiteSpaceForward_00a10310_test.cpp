// Behaviour test for 0x00A10310 WordWrap_FindNonWhiteSpaceForward: skips tab/CR/space/ideographic space, then one LF; null at the end of the text.
#include <cstdio>
#if !defined(_MSC_VER)
#define __fastcall
#endif
#define WORDWRAP_IS_WHITE_SPACE(c) ((c) == 0x0009 || (c) == 0x000D || (c) == 0x0020 || (c) == 0x3000)

const unsigned short* __fastcall WordWrap_FindNonWhiteSpaceForward(const unsigned short* text)
{
    while (WORDWRAP_IS_WHITE_SPACE(*text))
        ++text;
    if (*text == 0x000A)
        ++text;
    return *text ? text : 0;
}

int main()
{
    static const unsigned short a[] = { 0x20, 0x09, 0x3000, 0x0D, 'x', 0 };
    static const unsigned short b[] = { 0x20, 0x0A, 'y', 0 };
    static const unsigned short c[] = { 0x0A, 0x0A, 0 };
    static const unsigned short d[] = { 0x20, 0x20, 0 };
    static const unsigned short e[] = { 'z', 0x20, 0 };
    if (WordWrap_FindNonWhiteSpaceForward(a) != a + 4) { std::printf("00a10310_TEST FAIL\n"); return 1; }
    if (WordWrap_FindNonWhiteSpaceForward(b) != b + 2) { std::printf("00a10310_TEST FAIL\n"); return 1; }
    if (WordWrap_FindNonWhiteSpaceForward(c) != c + 1) { std::printf("00a10310_TEST FAIL\n"); return 1; }   // only one LF eaten
    if (WordWrap_FindNonWhiteSpaceForward(d) != 0) { std::printf("00a10310_TEST FAIL\n"); return 1; }
    if (WordWrap_FindNonWhiteSpaceForward(e) != e) { std::printf("00a10310_TEST FAIL\n"); return 1; }
    std::printf("00a10310_TEST PASS\n");
    return 0;
}