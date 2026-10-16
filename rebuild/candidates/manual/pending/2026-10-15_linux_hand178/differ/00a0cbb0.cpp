// CWStringParser::ReadAsStringUntilCharacter(wchar_t, CWParsedItem*) -- read a string item up to a terminator
// Grade: DIFFER(147v147) -- scheduling only: loop step emits add ecx,2 before inc edi (retail inc edi first)
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
        c = *++p;
        ++length;
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
