// Behaviour test for CWStringParser::ReadAsInteger (0x00A0D0E0): an integer item (type 3) returns its value.
// (The throw path is not exercised: the lane builds tests without /EHsc.)
#include <cstdio>
#include <stddef.h>

static int g_strings = 0;
class CWideString {
public:
    void* PStringData;
    CWideString() : PStringData(0) { ++g_strings; }
    ~CWideString() { --g_strings; }
};
class CWParsedItem {
public:
    long        Type;
    union { long Integer; float Float; } Value;
    CWideString String;
};
class CGenericException { public: CGenericException() {} };
class CWStringParser {
public:
    virtual ~CWStringParser() {}
    wchar_t*      String;
    unsigned long StringPos;
    unsigned long Line;
    long nextType; long nextValue;
    bool ReadNextItem(CWParsedItem* item) { item->Type = nextType; item->Value.Integer = nextValue; return true; }
    long ReadAsInteger();
};

long CWStringParser::ReadAsInteger()
{
    CWParsedItem item;
    if (ReadNextItem(&item) && item.Type == 3)
        return item.Value.Integer;
    throw CGenericException();
}

int main()
{
    CWStringParser p; p.String = 0; p.StringPos = 0; p.Line = 0; p.nextType = 3; p.nextValue = -1234;
    if (p.ReadAsInteger() != -1234 || g_strings != 0) { std::printf("00a0d0e0_TEST FAIL\n"); return 1; }
    p.nextValue = 77;
    if (p.ReadAsInteger() != 77 || g_strings != 0) { std::printf("00a0d0e0_TEST FAIL\n"); return 1; }
    std::printf("00a0d0e0_TEST PASS\n");
    return 0;
}