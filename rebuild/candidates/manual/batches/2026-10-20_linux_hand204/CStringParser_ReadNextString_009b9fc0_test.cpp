// Behaviour test for CStringParser::ReadNextString (0x009B9FC0).
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
bool CStringParser::ReadNextString(CParsedItem& item)
{
    const char* p = String + CursorPos.StringPos;
    while (*p) {
        if (!(isdigit((unsigned char)*p) || isalpha((unsigned char)*p) || *p == '_'))
            break;
        ++p;
    }
    const char* start = String + CursorPos.StringPos;
    CCharString text(start, p - start);
    item.InitAsString(text, text.GetLength());
    MoveStringPos(p - (String + CursorPos.StringPos));
    return true;
}
static int Run()
{
    char buf[] = "ab_9'z\"+ x";
    CStringParser p; std::memset(&p, 0, sizeof(p)); p.String = buf;
    CParsedItem it; it.Type = PIT_NONE; it.String.PStringData = 0;
    if (!p.ReadNextString(it) || it.Type != PIT_STRING || std::strcmp(it.String, "ab_9") != 0 || p.CursorPos.StringPos != 4 || p.CursorPos.InQuotes) return 1;
    p.CursorPos.StringPos = 7;    /* '+' : empty string, nothing consumed */
    if (!p.ReadNextString(it) || it.Type != PIT_STRING || it.String.GetLength() != 0 || p.CursorPos.StringPos != 7) return 1;
    char q[] = "it's"; p.String = q; p.CursorPos.StringPos = 0;
    if (!p.ReadNextString(it) || std::strcmp(it.String, "it") != 0 || p.CursorPos.StringPos != 2) return 1;
    char r[] = "x'y"; p.String = r; p.CursorPos.StringPos = 2; p.CursorPos.InQuotes = true;
    if (!p.ReadNextString(it) || std::strcmp(it.String, "y") != 0 || p.CursorPos.StringPos != 3 || !p.CursorPos.InQuotes) return 1;
    return 0;
}
int main()
{
    int base = g_live;
    if (Run() != 0 || g_live != base) { std::printf("009b9fc0_TEST FAIL\n"); return 1; }
    std::printf("009b9fc0_TEST PASS\n");
    return 0;
}
