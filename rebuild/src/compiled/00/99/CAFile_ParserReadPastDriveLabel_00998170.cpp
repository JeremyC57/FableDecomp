// CAFile::ParserReadPastDriveLabel(CStringParser& parser, CParsedItem& item) -- read "<name> :" from the parser (a drive label such as "C:")
// Retail: Fable.exe (TLC, VC7.1) @ 0x00998170, 96 bytes. FableWin.pdb
// ?ParserReadPastDriveLabel@CAFile@@CI_NAAVCStringParser@@AAVCParsedItem@@@Z: a private static `__fastcall` (parser in
// ecx, item in edx), plain `ret`.
// Callees: CStringParser::ReadNextItem(CParsedItem&) @ 0x009BA080 (x2), CParsedItem::GetSymbol() const @ 0x009B9870
// (out of line, `lea eax,[ecx+8]`; the item is a symbol, type 1), CBasicString<char>::operator==(const char*) const @ 0x004115A0.
// Literals: ":" @ 0x0124C528, "" @ 0x0122D70E.
// Shape: one `&&` chain; the forced-inline (a plain inline one stays a call) CCharString `== ":"` tests the data block -- none: the intrinsic strcmp of the two
// literals (`repe cmpsb` over sizeof(":")) in the braced `if (strcmp(text, "") == 0) return true; return false;` form,
// otherwise the out-of-line CBasicString compare -- and folds into the caller's true/false exits.
// Layout: CParsedItem {EParsedItemType Type +0, union +4, CCharString String +8} (Ego_r.pdb, 12 bytes); item types
// 2 (the drive name) then 1 (a symbol).
// Fix (sweep 3, FINDINGS chunk 482): the callee 0x009B9870 is CParsedItem::GetSymbol (retail body order = the PDB's
// GetSymbol, GetString, GetInteger, GetFloat run 0x009B9870..0x009B98A0); GetString is 0x009B9880.
// Leaf: ?ParserReadPastDriveLabel@CAFile
#include <string.h>

struct CBasicStringChar {                               // CBasicString<char> (packed 13 bytes in retail)
    char*         PData;                                // +0x00
    unsigned long StringLength;                         // +0x04
    bool operator==(const char* text) const;            // 0x004115A0
};
class CCharStringData { public:
    CBasicStringChar Data;                              // +0x00
};

class CCharString
{
public:
    CCharStringData* PStringData;                       // +0x00
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};

class CParsedItem
{
public:
    long        Type;                                   // +0x00 EParsedItemType
    long        Value;                                  // +0x04 numeric union
    CCharString String;                                 // +0x08
    const CCharString& GetSymbol() const;               // 0x009B9870
};

class CStringParser
{
public:
    bool ReadNextItem(CParsedItem& item);               // 0x009BA080
};

class CAFile
{
public:
    static bool __fastcall ParserReadPastDriveLabel(CStringParser& parser, CParsedItem& item);
};

bool __fastcall CAFile::ParserReadPastDriveLabel(CStringParser& parser, CParsedItem& item)
{
    if (parser.ReadNextItem(item) && item.Type == 2 && parser.ReadNextItem(item) && item.Type == 1 &&
        item.GetSymbol() == ":")
        return true;
    return false;
}