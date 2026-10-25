// CWStringParser::ReadNextSymbol(CWParsedItem* item) -- read the one character at the cursor as a symbol item (type 1) and step past it
// Leaf: ?ReadNextSymbol@CWStringParser@@AAE_NPAVCWParsedItem@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0CF10, 44 bytes, thiscall, `ret 4`.
//   PDB name ?ReadNextSymbol@CWStringParser@@AAE_NPAVCWParsedItem@@@Z (private).
// Callees: CWideString::SetAsSubString(const wchar_t*, long) 0x0099B770.
// Layout: Ego_r.pdb CWStringParser (vptr, String +0x04, StringPos +0x08, Line +0x0c); CWParsedItem (12 bytes):
//   Type +0, value +4, String +8 (the landed ReadNextItem 0x00A0CFC0 inlines the same symbol arm).
// Shape: sub-string first, then the type store, `++StringPos` re-read from memory, constant `mov al,1` result.
#include <stddef.h>   // wchar_t = unsigned short under VC7.1 without /Zc:wchar_t

class CWideStringData;
class CWideString {
public:
    CWideStringData* PStringData;
    void SetAsSubString(const wchar_t* text, long length);   // 0x0099B770
};

class CWParsedItem {
public:
    long        Type;                       // +0x00 EParsedItemType (1 = symbol)
    long        Integer;                    // +0x04
    CWideString String;                     // +0x08
};

class CWStringParser {
public:
    virtual ~CWStringParser();
    wchar_t*      String;                   // +0x04
    unsigned long StringPos;                // +0x08
    unsigned long Line;                     // +0x0c
private:
    bool ReadNextSymbol(CWParsedItem* item);
};

bool CWStringParser::ReadNextSymbol(CWParsedItem* item)
{
    item->String.SetAsSubString(&String[StringPos], 1);
    item->Type = 1;
    ++StringPos;
    return true;
}