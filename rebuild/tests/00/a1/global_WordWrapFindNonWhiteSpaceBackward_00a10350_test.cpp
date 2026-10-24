// Behaviour test for 0x00A10350 WordWrap_FindNonWhiteSpaceBackward: walks back over white space and LFs, never past the start.
#include <cstdio>
#if !defined(_MSC_VER)
#define __fastcall
#endif
#define WORDWRAP_IS_WHITE_SPACE(c) ((c) == 0x0009 || (c) == 0x000D || (c) == 0x0020 || (c) == 0x3000)

const unsigned short* __fastcall WordWrap_FindNonWhiteSpaceBackward(const unsigned short* text, const unsigned short* start)
{
    while (text >= start && (WORDWRAP_IS_WHITE_SPACE(*text) || *text == 0x000A))
        --text;
    return text >= start ? text : 0;
}

int main()
{
    static const unsigned short buf[] = { 0x20, 'a', 'b', 0x0A, 0x3000, 0x0D, 0x20, 0 };
    if (WordWrap_FindNonWhiteSpaceBackward(buf + 6, buf + 1) != buf + 2) { std::printf("00a10350_TEST FAIL\n"); return 1; }
    if (WordWrap_FindNonWhiteSpaceBackward(buf + 2, buf + 1) != buf + 2) { std::printf("00a10350_TEST FAIL\n"); return 1; }
    if (WordWrap_FindNonWhiteSpaceBackward(buf + 6, buf + 3) != 0) { std::printf("00a10350_TEST FAIL\n"); return 1; }
    if (WordWrap_FindNonWhiteSpaceBackward(buf + 1, buf + 1) != buf + 1) { std::printf("00a10350_TEST FAIL\n"); return 1; }
    std::printf("00a10350_TEST PASS\n");
    return 0;
}