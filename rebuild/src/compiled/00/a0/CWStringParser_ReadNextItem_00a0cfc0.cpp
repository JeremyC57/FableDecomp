// CWStringParser::ReadNextItem(CWParsedItem* item) -- skips whitespace (counting line feeds) to the next item and reads it: a number (digit, or '-' then a digit), a string/identifier (letter or '_'), else a one-character symbol; false at the end of the text
// Leaf: ReadNextItem
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0CFC0, 210 bytes (padded to 224), `ret 4`.
//   PDB name ?ReadNextItem@CWStringParser@@QAE_NPAVCWParsedItem@@@Z.
// Callees: iswspace 0x00BFEB90, iswdigit 0x00BFEDE2, iswalpha 0x00BFEDE8 (CRT import thunks, called directly);
//   ReadNextNumber 0x00A0CDD0, ReadNextString 0x00A0CE90; CWideString::SetAsSubString(const wchar_t*, long) 0x0099B770.
// Shape: the cursor walks in a register and is published to StringPos once the item starts; the sub-readers are
//   returned straight (`call ; pop ; ret 4` tails), the symbol case stores the type and re-reads StringPos for `++`.
typedef unsigned short wchar16;                    // wchar_t under the era defaults (wint_t is the same 16-bit type)
extern "C" int __cdecl iswdigit(wchar16 c);        // 0x00BFEDE2 (MSVCR71 import thunk)
extern "C" int __cdecl iswalpha(wchar16 c);        // 0x00BFEDE8
extern "C" int __cdecl iswspace(wchar16 c);        // 0x00BFEB90
class CWideString { public:
    void* PStringData;
    void SetAsSubString(const wchar16* text, long length);   // 0x0099B770
};
class CWParsedItem { public:
    long Type;                                     // +0x00 EParsedItemType (1 = symbol, 3 = integer, 4 = float)
    union { long Integer; float Float; } Value;    // +0x04
    CWideString String;                            // +0x08
    void InitAsInteger(const wchar16* text, unsigned long length);   // 0x00A0CA20
    void InitAsFloat(const wchar16* text, unsigned long length);     // 0x00A0CA90
};
struct CWStringParser {                            // Ego_r.pdb, 16 bytes
    virtual ~CWStringParser();
    wchar16*      String;                          // +0x04
    unsigned long StringPos;                       // +0x08
    unsigned long Line;                            // +0x0c
    bool ReadNextNumber(CWParsedItem* item);       // 0x00A0CDD0
    bool ReadNextString(CWParsedItem* item);       // 0x00A0CE90
    bool ReadNextSymbol(CWParsedItem* item);
    bool ReadNextItem(CWParsedItem* item);
};
bool CWStringParser::ReadNextItem(CWParsedItem* item)
{
    unsigned long pos = StringPos;
    wchar16 c = String[pos];
    while (c != 0) {
        if (c == '\n')
            ++Line;
        if (!iswspace(c)) {
            StringPos = pos;
            if (iswdigit(c) || (c == '-' && iswdigit(String[pos + 1])))
                return ReadNextNumber(item);
            if (iswalpha(c) || c == '_')
                return ReadNextString(item);
            item->String.SetAsSubString(String + StringPos, 1);
            item->Type = 1;
            ++StringPos;
            return true;
        }
        c = String[++pos];
    }
    return false;
}