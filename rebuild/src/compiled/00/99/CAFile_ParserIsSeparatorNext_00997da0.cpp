// CAFile::ParserIsSeparatorNext(CStringParser& parser) -- peek (without consuming) the parser's next item: true when it is the symbol "\" or "/"
// Leaf: ?ParserIsSeparatorNext@CAFile@@CI_NAAVCStringParser@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00997DA0, length 236 bytes (+ int3 padding), private static fastcall.
// Layout: CParsedItem (Ego_r.pdb, 12 bytes): Type +0 (EParsedItemType, 1 = symbol), a union +4, CCharString String +8.
// Callees: CCharString::CCharString() @ 0x0099E4B0 and ~CCharString @ 0x0099EAE0 (CParsedItem's inline ctor/dtor),
// CStringParser::PeekNextItem(CParsedItem&) @ 0x009BA8C0, CParsedItem::GetSymbol() const @ 0x009B9870 (`lea eax,[ecx+8]`,
// shared with GetString; the manifest's "CModeBlock::GetAnimFilter" is a propagated label).
// Shape: CCharString::operator==(char) is the __forceinline null-test-positive form from the landed
// CCharString::operator!= @ 0x0099E520 ({c, 0} built on the stack, intrinsic strcmp, `== 0 ? true : false`).
// Twin: CAFile::ParserReadPastSeparator @ 0x00997E90 (same body over CStringParser::ReadNextItem @ 0x009BA080).
#include <string.h>
#pragma intrinsic(strcmp)

#pragma pack(push, 1)
class CCharStringData { public:                      // 17 bytes
    char*         PData;                      // +0x00
    unsigned long StringLength;               // +0x04
    unsigned long DataLength;                 // +0x08
    unsigned char Flags;                      // +0x0c
    long          NoRefs;                     // +0x0d
};
#pragma pack(pop)

class CCharString {
public:
    CCharString();                            // 0x0099E4B0
    ~CCharString();                           // 0x0099EAE0

    __forceinline bool operator==(char character) const
    {
        if (PStringData != 0) {
            char text[2];
            text[0] = character;
            text[1] = 0;
            return strcmp(PStringData->PData, text) == 0 ? true : false;
        }
        return false;
    }

    CCharStringData* PStringData;             // +0x00
};

enum EParsedItemType
{
    PARSED_ITEM_NONE,
    PARSED_ITEM_SYMBOL,
    PARSED_ITEM_INTEGER,
    PARSED_ITEM_FLOAT,
    PARSED_ITEM_STRING
};

class CParsedItem
{
public:
    CParsedItem() {}
    bool IsSymbol() const { return Type == PARSED_ITEM_SYMBOL; }
    const CCharString& GetSymbol() const;     // 0x009B9870

    EParsedItemType Type;                     // +0x00
    union
    {
        long  Integer;
        float Float;
    };                                        // +0x04
    CCharString String;                       // +0x08
};

class CStringParser
{
public:
    bool PeekNextItem(CParsedItem& item);     // 0x009BA8C0
};

class CAFile
{
private:
    static bool __fastcall ParserIsSeparatorNext(CStringParser& parser);
};

bool __fastcall CAFile::ParserIsSeparatorNext(CStringParser& parser)
{
    bool separator = false;
    CParsedItem item;
    if (parser.PeekNextItem(item) && item.IsSymbol())
    {
        if (item.GetSymbol() == '\\' || item.GetSymbol() == '/')
            separator = true;
    }
    return separator;
}