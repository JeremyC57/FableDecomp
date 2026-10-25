// Behaviour test for CWStringParser::SkipWhitespace (0x00A0C910): skips the whitespace run, counts line feeds,
// returns the number skipped and advances StringPos by it.
#include <cstdio>
#include <stddef.h>

static int IsSpace(wchar_t c) { return c == L' ' || c == L'\t' || c == L'\n' || c == L'\r'; }   // iswspace stand-in

class CWStringParser {
public:
    virtual ~CWStringParser() {}
    wchar_t*      String;
    unsigned long StringPos;
    unsigned long Line;
    long SkipWhitespace();
};

long CWStringParser::SkipWhitespace()
{
    long count = 0;
    wchar_t c = String[StringPos];
    for (;;)
    {
        if (c == L'\n')
            ++Line;
        if (!IsSpace(c))
            break;
        c = String[StringPos + ++count];
    }
    StringPos += count;
    return count;
}

int main()
{
    wchar_t text[] = L"ab \n\t\ncd";
    CWStringParser p; p.String = text; p.StringPos = 2; p.Line = 0;
    long n = p.SkipWhitespace();
    if (n != 4 || p.StringPos != 6 || p.Line != 2) { std::printf("00a0c910_TEST FAIL\n"); return 1; }
    n = p.SkipWhitespace();
    if (n != 0 || p.StringPos != 6 || p.Line != 2) { std::printf("00a0c910_TEST FAIL\n"); return 1; }
    std::printf("00a0c910_TEST PASS\n");
    return 0;
}