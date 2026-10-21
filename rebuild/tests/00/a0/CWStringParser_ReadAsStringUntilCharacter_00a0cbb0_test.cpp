// Behaviour test for 00a0cbb0: src/00a0cbb0.cpp copied verbatim; reads up to a stop character, counting newlines,
// and makes a string item from the run (or an empty one when the run is empty and stopped on the terminator).
#include <cstdio>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CWStringParser::ReadAsStringUntilCharacter(wchar_t, CWParsedItem*) -- read a string item up to a terminator
// Retail: Fable.exe @ 0x00A0CBB0, 147 bytes (+int3 padding). PDB ?ReadAsStringUntilCharacter@CWStringParser@@QAE_N_WPAVCWParsedItem@@@Z
// Callees: CWideString::SetAsSubString(const wchar_t*, long) 0x0099B770 (manifest label "CCharString_Replace" is wrong;
// the PDB lists ?SetAsSubString@CWideString@@QAEXPB_WJ@Z and both call sites pass (text, length) on &item->String).
// CWStringParser (16 bytes, PDB): +0x04 String, +0x08 StringPos, +0x0C Line. CWParsedItem (12): +0 Type, +4 value, +8 String.
// Walks from String+StringPos counting characters until `stop` or NUL, bumping Line on every '\n' seen
// (including the terminator). A non-empty run becomes a string item (Type 2) and advances StringPos; an empty
// run that stopped on `stop` yields an empty string item; hitting NUL with nothing read fails.
#include <stddef.h>   // wchar_t = unsigned short under VC7.1 without /Zc:wchar_t

struct CWideStringData;
struct CWideString {
    CWideStringData* PStringData;
    void SetAsSubString(const wchar_t* text, long length);   // 0x0099B770
};

struct CWParsedItem {
    long        Type;      // +0x00 EParsedItemType (2 = string)
    long        Integer;   // +0x04
    CWideString String;    // +0x08
    void InitAsString(const wchar_t* text, unsigned long length) { String.SetAsSubString(text, length); Type = 2; }
};

struct CWStringParser {
    void*          __vftable;  // +0x00
    wchar_t*       String;     // +0x04
    unsigned long  StringPos;  // +0x08
    unsigned long  Line;       // +0x0C
    bool ReadAsStringUntilCharacter(wchar_t stop, CWParsedItem* item);
};

bool CWStringParser::ReadAsStringUntilCharacter(wchar_t stop, CWParsedItem* item)
{
    const wchar_t* start = &String[StringPos];
    wchar_t c = *start;
    unsigned long length = 0;
    const wchar_t* p = start;
    for (;;)
    {
        if (c == L'\n')
            ++Line;
        if (c == stop || c == 0)
            break;
        ++length;
        c = *++p;
    }
    if (length > 0)
    {
        item->InitAsString(start, length);
        StringPos += length;
        return true;
    }
    if (c == stop)
    {
        item->InitAsString(L"", 0);
        return true;
    }
    return false;
}
// ---- end source ----
static const wchar_t* g_text; static long g_len; static int g_calls;
void CWideString::SetAsSubString(const wchar_t* text, long length) { g_text = text; g_len = length; ++g_calls; }
int main()
{
    static wchar_t buf[] = { 'x', 'a', '\n', 'b', ',', 'c', 0 };
    CWStringParser parser; parser.__vftable = 0; parser.String = buf; parser.StringPos = 1; parser.Line = 0;
    CWParsedItem item; item.Type = 0; item.Integer = 0; item.String.PStringData = 0;
    CHECK(parser.ReadAsStringUntilCharacter(',', &item) == true);
    CHECK(item.Type == 2 && g_text == buf + 1 && g_len == 3 && g_calls == 1);
    CHECK(parser.StringPos == 4 && parser.Line == 1);
    // now at ',' : empty run stopped on the terminator -> empty string item, position unchanged
    item.Type = 0;
    CHECK(parser.ReadAsStringUntilCharacter(',', &item) == true);
    CHECK(item.Type == 2 && g_len == 0 && g_calls == 2 && g_text[0] == 0 && parser.StringPos == 4);
    // read to end with a terminator never seen
    parser.StringPos = 5;
    CHECK(parser.ReadAsStringUntilCharacter(';', &item) == true);
    CHECK(g_len == 1 && g_text == buf + 5 && parser.StringPos == 6);
    // at NUL with nothing read -> fail, no item
    item.Type = 0;
    CHECK(parser.ReadAsStringUntilCharacter(';', &item) == false);
    CHECK(item.Type == 0 && g_calls == 3);
    // newline as the terminator is counted too
    static wchar_t buf2[] = { 'q', '\n', 0 };
    parser.String = buf2; parser.StringPos = 0; parser.Line = 5;
    CHECK(parser.ReadAsStringUntilCharacter('\n', &item) == true);
    CHECK(parser.Line == 6 && g_len == 1 && parser.StringPos == 1);
    if (fails == 0) printf("00a0cbb0_TEST PASS\n"); else printf("00a0cbb0_TEST FAIL\n");
    return fails;
}