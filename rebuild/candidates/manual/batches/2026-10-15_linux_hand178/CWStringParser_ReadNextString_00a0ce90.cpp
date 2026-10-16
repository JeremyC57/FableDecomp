// CWStringParser::ReadNextString(CWParsedItem*) -- read an identifier-like run (digits, letters, '_') as a string item
// Retail: Fable.exe @ 0x00A0CE90, 113 bytes (+int3). PDB ?ReadNextString@CWStringParser@@AAE_NPAVCWParsedItem@@@Z (private).
// CWStringParser (16 bytes, PDB): +0x04 String, +0x08 StringPos, +0x0C Line. CWParsedItem (12): +0 Type, +4 value, +8 String.
// Callees: iswdigit 0x00BFEDE2, iswalpha 0x00BFEDE8 (CRT import thunks), CWideString::SetAsSubString 0x0099B770.
// Shape: a local cursor (ebx) walks from StringPos; String is re-read every step; the item spans StringPos..cursor
// and StringPos is set to the cursor. Always succeeds.
#include <stddef.h>   // wchar_t = unsigned short under VC7.1 without /Zc:wchar_t

extern "C" int __cdecl iswdigit(wchar_t c);   // 0x00BFEDE2
extern "C" int __cdecl iswalpha(wchar_t c);   // 0x00BFEDE8

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

bool CWStringParser::ReadNextString(CWParsedItem* item)
{
    unsigned long pos = StringPos;
    wchar_t c = String[pos];
    while (iswdigit(c) || iswalpha(c) || c == L'_')
    {
        if (c == L'\n')
            ++Line;
        c = String[++pos];
    }
    item->InitAsString(&String[StringPos], pos - StringPos);
    StringPos = pos;
    return true;
}
