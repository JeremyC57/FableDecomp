// Behaviour test for CWStringParser::ReadAsFloat (0x00A0D140): a float item (type 4) returns its value.
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
    long nextType; float nextValue;
    bool ReadNextItem(CWParsedItem* item) { item->Type = nextType; item->Value.Float = nextValue; return true; }
    float ReadAsFloat();
};

float CWStringParser::ReadAsFloat()
{
    CWParsedItem item;
    if (ReadNextItem(&item) && item.Type == 4)
        return item.Value.Float;
    throw CGenericException();
}

int main()
{
    CWStringParser p; p.String = 0; p.StringPos = 0; p.Line = 0; p.nextType = 4; p.nextValue = 2.5f;
    float v = p.ReadAsFloat();
    if (v < 2.499f || v > 2.501f || g_strings != 0) { std::printf("00a0d140_TEST FAIL\n"); return 1; }
    std::printf("00a0d140_TEST PASS\n");
    return 0;
}