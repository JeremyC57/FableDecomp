// Behaviour test for CWStringParser::ReadNextSymbol (0x00A0CF10): one-character sub-string at the cursor, type 1,
// cursor advanced by one, result true.
#include <cstdio>
#include <stddef.h>

class CWideString {
public:
    const wchar_t* text; long length;
    void SetAsSubString(const wchar_t* t, long n) { text = t; length = n; }
};
class CWParsedItem {
public:
    long        Type;
    long        Integer;
    CWideString String;
};
class CWStringParser {
public:
    virtual ~CWStringParser() {}
    wchar_t*      String;
    unsigned long StringPos;
    unsigned long Line;
    bool ReadNextSymbol(CWParsedItem* item);
};

bool CWStringParser::ReadNextSymbol(CWParsedItem* item)
{
    item->String.SetAsSubString(&String[StringPos], 1);
    item->Type = 1;
    ++StringPos;
    return true;
}

int main()
{
    wchar_t text[] = L"a+b";
    CWStringParser p; p.String = text; p.StringPos = 1; p.Line = 0;
    CWParsedItem item; item.Type = 0; item.String.text = 0; item.String.length = 0;
    bool ok = p.ReadNextSymbol(&item);
    if (!ok || item.Type != 1 || item.String.text != text + 1 || item.String.length != 1 || p.StringPos != 2)
        { std::printf("00a0cf10_TEST FAIL\n"); return 1; }
    std::printf("00a0cf10_TEST PASS\n");
    return 0;
}