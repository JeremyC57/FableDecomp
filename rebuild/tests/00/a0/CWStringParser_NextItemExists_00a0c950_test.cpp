// Behaviour test for CWStringParser::NextItemExists (0x00A0C950): true when a non-space character follows before the
// terminator; line feeds on the way are counted; the cursor does not move.
#include <cstdio>
#include <stddef.h>

static int IsSpace(wchar_t c) { return c == L' ' || c == L'\t' || c == L'\n' || c == L'\r'; }   // iswspace stand-in

class CWStringParser {
public:
    virtual ~CWStringParser() {}
    wchar_t*      String;
    unsigned long StringPos;
    unsigned long Line;
    bool NextItemExists();
};

bool CWStringParser::NextItemExists()
{
    unsigned long pos = StringPos;
    wchar_t c = String[pos];
    while (c != 0)
    {
        if (c == L'\n')
            ++Line;
        if (!IsSpace(c))
            return true;
        c = String[++pos];
    }
    return false;
}

int main()
{
    wchar_t text[] = L"x \n y";
    CWStringParser p; p.String = text; p.StringPos = 1; p.Line = 0;
    if (!p.NextItemExists() || p.StringPos != 1 || p.Line != 1) { std::printf("00a0c950_TEST FAIL\n"); return 1; }
    wchar_t tail[] = L"z \n\n ";
    p.String = tail; p.StringPos = 1; p.Line = 0;
    if (p.NextItemExists() || p.StringPos != 1 || p.Line != 2) { std::printf("00a0c950_TEST FAIL\n"); return 1; }
    wchar_t empty[] = L"";
    p.String = empty; p.StringPos = 0; p.Line = 0;
    if (p.NextItemExists() || p.Line != 0) { std::printf("00a0c950_TEST FAIL\n"); return 1; }
    std::printf("00a0c950_TEST PASS\n");
    return 0;
}