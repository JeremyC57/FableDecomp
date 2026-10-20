// Behaviour test for CStringParser::ReadNextNumber (0x009B9EE0).
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif
static int g_live = 0;
struct CCharStringData { const char* Chars; unsigned long Length; };
struct CCharString {
    CCharStringData* PStringData;
    CCharString() : PStringData(0) { ++g_live; }
    CCharString(const char* text, long length)
    {
        ++g_live; PStringData = 0;
        if (length > 0) { char* c = (char*)std::malloc(length + 1); std::memcpy(c, text, length); c[length] = 0; PStringData = new CCharStringData; PStringData->Chars = c; PStringData->Length = length; }
    }
    ~CCharString() { if (PStringData) { std::free((void*)PStringData->Chars); delete PStringData; } --g_live; }
    operator const char*() const { return PStringData ? PStringData->Chars : ""; }
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->Length; }
    void SetAsSubString(const char* text, long length) { this->~CCharString(); new (this) CCharString(text, length); }
};
struct CCursorPos { bool InQuotes; unsigned long StringPos; unsigned long Line; };
enum EParsedItemType { PIT_NONE = 0, PIT_SYMBOL = 1, PIT_STRING = 2, PIT_INTEGER = 3, PIT_FLOAT = 4 };
struct CParsedItem {
    EParsedItemType Type; union { long Integer; float Float; }; CCharString String;
    void InitAsInteger(const char* text, unsigned long length) { char b[64]; std::memcpy(b, text, length); b[length] = 0; Integer = std::atol(b); Type = PIT_INTEGER; }
    void InitAsFloat(const char* text, unsigned long length) { char b[64]; std::memcpy(b, text, length); b[length] = 0; Float = (float)std::atof(b); Type = PIT_FLOAT; }
    void InitAsString(const char* text, unsigned long length) { String.SetAsSubString(text, length); Type = PIT_STRING; }
};
struct CStringParser {
    char* String; unsigned long BaseLineOffset; CCharString Filename; CCharString TempCharString; void* Comments;
    void (__fastcall* ErrorCallback)(const CCharString&); CCursorPos CursorPos;
    bool CalcInQuotes(bool inQuotes, const char* from, const char* to) const
    { for (const char* p = from; p != to; ++p) if (*p == '"' || *p == '\'') inQuotes = !inQuotes; return inQuotes; }
    void MoveStringPos(unsigned long n)
    {
        if (n > 0) {
            unsigned long pos = CursorPos.StringPos;
            const char* from = String + pos;
            CursorPos.InQuotes = CalcInQuotes(CursorPos.InQuotes, from, from + n);
            CursorPos.StringPos = pos + n;
        }
    }
    bool ReadNextNumber(CParsedItem& item);
    bool ReadNextString(CParsedItem& item);
};
bool CStringParser::ReadNextNumber(CParsedItem& item)
{
    const char* p = String + CursorPos.StringPos;
    bool isFloat = false;
    while (*p) {
        if (!(isdigit((unsigned char)*p) || (*p == '.' && !isFloat) || (*p == '-' && p == String + CursorPos.StringPos)))
            break;
        if (*p == '.')
            isFloat = true;
        ++p;
    }
    const char* start = String + CursorPos.StringPos;
    CCharString text(start, p - start);
    if (!isFloat)
        item.InitAsInteger(text, text.GetLength());
    else
        item.InitAsFloat(text, text.GetLength());
    MoveStringPos(p - (String + CursorPos.StringPos));
    return true;
}
static int Run()
{
    char buf[] = "-12.5x 7; 3.4.5";
    CStringParser p; std::memset(&p, 0, sizeof(p)); p.String = buf;
    CParsedItem it; it.Type = PIT_NONE; it.String.PStringData = 0;
    if (!p.ReadNextNumber(it) || it.Type != PIT_FLOAT || it.Float != -12.5f || p.CursorPos.StringPos != 5) return 1;
    p.CursorPos.StringPos = 7;
    if (!p.ReadNextNumber(it) || it.Type != PIT_INTEGER || it.Integer != 7 || p.CursorPos.StringPos != 8) return 1;
    p.CursorPos.StringPos = 10;   /* second '.' ends the number */
    if (!p.ReadNextNumber(it) || it.Type != PIT_FLOAT || it.Float != 3.4f || p.CursorPos.StringPos != 13) return 1;
    p.CursorPos.StringPos = 5;    /* no digits: empty integer, cursor unchanged */
    if (!p.ReadNextNumber(it) || it.Type != PIT_INTEGER || it.Integer != 0 || p.CursorPos.StringPos != 5) return 1;
    char q[] = "4-2"; p.String = q; p.CursorPos.StringPos = 0;   /* '-' only at the start */
    if (!p.ReadNextNumber(it) || it.Integer != 4 || p.CursorPos.StringPos != 1 || p.CursorPos.InQuotes) return 1;
    return 0;
}
int main()
{
    int base = g_live;
    if (Run() != 0 || g_live != base) { std::printf("009b9ee0_TEST FAIL\n"); return 1; }
    std::printf("009b9ee0_TEST PASS\n");
    return 0;
}