// CAFile::ParserIsSeparatorNext(CStringParser& parser) -- read the next item; true when it is the symbol '\' or '/'
// Retail: Fable.exe (TLC, VC7.1) @ 0x00997E90, 236 bytes. FableWin.pdb ?ParserIsSeparatorNext@CAFile@@CI_NAAVCStringParser@@@Z:
// a private static `__fastcall` (parser in ecx), plain `ret`.
// Callees: CCharString() @ 0x0099E4B0 (the item's string), CStringParser::ReadNextItem(CParsedItem&) @ 0x009BA080,
// CParsedItem::GetSymbol() const @ 0x009B9870 (out of line, `lea eax,[ecx+8]`, called once per compare),
// ~CCharString @ 0x0099EAE0.
// Shape: the result is a frame byte cleared up front; both operator==(char) bodies need `__forceinline` (plain inline
// stays a call). CCharString's skips a null data block
// (no materialised bool) and otherwise runs CBasicString<char>'s inline `== char`: a {c, 0} pair built in one frame
// word and the intrinsic strcmp loop against PData, whose bool is materialised (`sete`) before the `||` test.
// Layout: CParsedItem {Type +0, union +4, CCharString String +8}; item type 1 is a symbol.
// Fix (sweep 3): the callee 0x009B9870 is CParsedItem::GetSymbol (retail body order = the PDB's GetSymbol, GetString,
// GetInteger, GetFloat run 0x009B9870..0x009B98A0; FINDINGS chunk 482); it was declared as GetString.
// Leaf: ?ParserIsSeparatorNext@CAFile
#include <string.h>

struct CBasicStringChar {                               // CBasicString<char> (packed 13 bytes in retail)
    char*         PData;                                // +0x00
    unsigned long StringLength;                         // +0x04
    __forceinline bool operator==(char c) const
    {
        char text[2];
        text[0] = c;
        text[1] = 0;
        return strcmp(PData, text) == 0;
    }
};
struct CCharStringData {
    CBasicStringChar Data;                              // +0x00
};

class CCharString
{
public:
    CCharStringData* PStringData;                       // +0x00
    CCharString();                                      // 0x0099E4B0
    ~CCharString();                                     // 0x0099EAE0
    __forceinline bool operator==(char c) const
    {
        if (PStringData == 0)
            return false;
        return PStringData->Data == c;
    }
};

class CParsedItem
{
public:
    long        Type;                                   // +0x00 EParsedItemType
    long        Value;                                  // +0x04 numeric union
    CCharString String;                                 // +0x08
    CParsedItem() { }
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
    static bool __fastcall ParserIsSeparatorNext(CStringParser& parser);
};

bool __fastcall CAFile::ParserIsSeparatorNext(CStringParser& parser)
{
    bool result = false;
    CParsedItem item;
    if (parser.ReadNextItem(item) && item.Type == 1) {
        if (item.GetSymbol() == '\\' || item.GetSymbol() == '/')
            result = true;
    }
    return result;
}