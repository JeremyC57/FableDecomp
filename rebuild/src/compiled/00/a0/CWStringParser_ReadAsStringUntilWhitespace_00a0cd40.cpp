// CWStringParser::ReadAsStringUntilWhitespace(CWParsedItem*) -- read a run of non-whitespace as a string item
// Retail: Fable.exe @ 0x00A0CD40, 131 bytes (+int3). PDB ?ReadAsStringUntilWhitespace@CWStringParser@@QAE_NPAVCWParsedItem@@@Z
// CWStringParser (16 bytes, PDB): +0x04 String, +0x08 StringPos, +0x0C Line. CWParsedItem (12): +0 Type, +4 value, +8 String.
// Callees: iswspace 0x00BFEB90 (CRT), CWideString::SetAsSubString(const wchar_t*, long) 0x0099B770 (label "CCharString_Replace" is wrong).
// Shape: an empty input (NUL at the cursor) fails first; then the same walk as ReadAsStringUntilNonWhitespace with the
// iswspace test inverted.
#include <stddef.h>   // wchar_t = unsigned short under VC7.1 without /Zc:wchar_t

extern "C" int __cdecl iswspace(wchar_t c);   // 0x00BFEB90 (CRT, wint_t argument)

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
    bool ReadAsStringUntilNonWhitespace(CWParsedItem* item);
    bool ReadAsStringUntilWhitespace(CWParsedItem* item);
    bool ReadNextString(CWParsedItem* item);
};

bool CWStringParser::ReadAsStringUntilWhitespace(CWParsedItem* item)
{
    wchar_t c = String[StringPos];
    if (c == 0)
        return false;
    unsigned long length = 0;
    for (;;)
    {
        if (c == L'\n')
            ++Line;
        if (iswspace(c) || c == 0)
            break;
        c = String[StringPos + ++length];
    }
    if (length > 0)
    {
        item->InitAsString(&String[StringPos], length);
        StringPos += length;
        return true;
    }
    return false;
}