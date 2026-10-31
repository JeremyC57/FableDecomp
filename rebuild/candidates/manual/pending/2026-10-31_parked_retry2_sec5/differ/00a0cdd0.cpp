// CWStringParser::ReadNextNumber(CWParsedItem* item) -- scans a number at the cursor (digits, one '.', a leading '-'), counting line feeds, and stores it in the item as a float when it had a point, else as an integer; the cursor moves past it
// Leaf: ReadNextNumber
// Grade: DIFFER(164v178) -- character scoped inside the loop (wchar16 c = String[pos] at the top, ++pos in the for)
//   fixes the register swap (cursor ebp, character edi; 39 -> 27 diff lines); residue: retail re-reads the next
//   character at each back edge (String[pos] after ++Line, String[pos+1] then inc ebp) with Line as load/inc/store, ours
//   loops to one top load with inc dword ptr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0CDD0, 178 bytes (padded to 192), `ret 4`.
//   PDB name ?ReadNextNumber@CWStringParser@@AAE_NPAVCWParsedItem@@@Z.
// Callees: iswdigit 0x00BFEDE2 (CRT import thunk, called directly), CWParsedItem::InitAsFloat 0x00A0CA90,
//   CWParsedItem::InitAsInteger 0x00A0CA20.
// Shape: the exits return from inside the loop (a '.' after a point goes straight to the float tail), the digit path
//   counts line feeds with its own advance; still differs: retail keeps pos in ebp and the character in edi.
typedef unsigned short wchar16;                    // wchar_t under the era defaults (wint_t is the same 16-bit type)
extern "C" int __cdecl iswdigit(wchar16 c);        // 0x00BFEDE2 (MSVCR71 import thunk)
extern "C" int __cdecl iswalpha(wchar16 c);        // 0x00BFEDE8
extern "C" int __cdecl iswspace(wchar16 c);        // 0x00BFEB90
struct CWideString {
    void* PStringData;
    void SetAsSubString(const wchar16* text, long length);   // 0x0099B770
};
struct CWParsedItem {
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
    bool ReadNextNumber(CWParsedItem* item);
    bool ReadNextString(CWParsedItem* item);       // 0x00A0CE90
    bool ReadNextSymbol(CWParsedItem* item);
    bool ReadNextItem(CWParsedItem* item);
};
bool CWStringParser::ReadNextNumber(CWParsedItem* item)
{
    unsigned long pos = StringPos;
    bool isFloat = false;
    for (;; ++pos) {
        wchar16 c = String[pos];
        if (!iswdigit(c)) {
            if (c == '.') {
                if (isFloat) {
                    item->InitAsFloat(String + StringPos, pos - StringPos);
                    StringPos = pos;
                    return true;
                }
            }
            else if (c != '-' || pos != StringPos) {
                if (isFloat)
                    item->InitAsFloat(String + StringPos, pos - StringPos);
                else
                    item->InitAsInteger(String + StringPos, pos - StringPos);
                StringPos = pos;
                return true;
            }
        }
        if (c == '\n')
            ++Line;
        else if (c == '.')
            isFloat = true;
    }
}
