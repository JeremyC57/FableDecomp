// Behaviour test for CWStringParser::ReadNextChunkAsString (0x00A0D0A0): whitespace at the cursor selects the
// until-non-whitespace reader, anything else the until-whitespace reader; the reader's result is returned.
#include <cstdio>
#include <stddef.h>

static int IsSpace(wchar_t c) { return c == L' ' || c == L'\t' || c == L'\n' || c == L'\r'; }   // iswspace stand-in
static int g_which = 0;

class CWParsedItem { public: long Type; };
class CWStringParser {
public:
    virtual ~CWStringParser() {}
    wchar_t*      String;
    unsigned long StringPos;
    unsigned long Line;
    bool ReadAsStringUntilNonWhitespace(CWParsedItem* item) { g_which = 1; item->Type = 2; return true; }
    bool ReadAsStringUntilWhitespace(CWParsedItem* item) { g_which = 2; item->Type = 2; return false; }
    bool ReadNextChunkAsString(CWParsedItem* item);
};

bool CWStringParser::ReadNextChunkAsString(CWParsedItem* item)
{
    if (IsSpace(String[StringPos]))
        return ReadAsStringUntilNonWhitespace(item);
    return ReadAsStringUntilWhitespace(item);
}

int main()
{
    wchar_t text[] = L"ab  cd";
    CWStringParser p; p.String = text; p.StringPos = 2; p.Line = 0;
    CWParsedItem item;
    if (!p.ReadNextChunkAsString(&item) || g_which != 1) { std::printf("00a0d0a0_TEST FAIL\n"); return 1; }
    p.StringPos = 4;
    if (p.ReadNextChunkAsString(&item) || g_which != 2) { std::printf("00a0d0a0_TEST FAIL\n"); return 1; }
    std::printf("00a0d0a0_TEST PASS\n");
    return 0;
}