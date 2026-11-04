// CWStringParser::ReadNextNumber(CWParsedItem* item) -- scan a number at the cursor (digits, one '.', a leading '-'), counting line feeds, and store it in the item as a float when it had a point, else as an integer; the cursor moves past it
// Grade: DIFFER(ours v retail) -- DIFFER(178v178) (try f): the cursor-ebp / character-edi colouring and the length now match; in the second exit arm ('-' not leading, or not a number character) VC7.1 hoists the shared argument setup (StringPos, String, pos - StringPos) above the isFloat test, where retail tests bl first and jumps into the first arm's float tail (cross-jumped), with the integer tail written out after it. Exits written in full keep the tails but swap ebp/edi; a break to one shared tail is 188 bytes.
// Leaf: ?ReadNextNumber@CWStringParser@@AAE_NPAVCWParsedItem@@@Z
// Flags: /Zc:wchar_t
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0CDD0, 178 bytes (padded to 192), `ret 4`. Ego_r.pdb ?ReadNextNumber@CWStringParser@@AAE_NPAVCWParsedItem@@@Z.
// Callees: iswdigit 0x00BFEDE2 (CRT import thunk, called directly), CWParsedItem::InitAsFloat 0x00A0CA90 /
//   InitAsInteger 0x00A0CA20 (Ego_r.pdb `(const wchar_t*, unsigned long)`, native wchar_t: ?InitAsInteger@CWParsedItem@@QAEXPB_WK@Z).
// Shape: the landed narrow twin's scan condition (CStringParser::ReadNextNumber 0x009B9EE0) on a local cursor, with the wide
//   parser's line-feed count in the advance (the landed ReadNextString 0x00A0CE90 loop); VC7.1 threads the '.' / '-' arms past
//   the tests their character already decides.
extern "C" int __cdecl iswdigit(wchar_t c);        // 0x00BFEDE2 (MSVCR71 import thunk)
class CWideStringData;
class CWideString { public:
    CWideStringData* PStringData;
};
class CWParsedItem { public:                       // Ego_r.pdb, 12 bytes
    long        Type;                              // +0x00 EParsedItemType
    union { long Integer; float Float; };          // +0x04
    CWideString String;                            // +0x08
    void InitAsInteger(const wchar_t* text, unsigned long length);   // 0x00A0CA20
    void InitAsFloat(const wchar_t* text, unsigned long length);     // 0x00A0CA90
};
class CWStringParser {                             // Ego_r.pdb, 16 bytes
public:
    virtual ~CWStringParser();
private:
    bool ReadNextNumber(CWParsedItem* item);
    wchar_t*      String;                          // +0x04
    unsigned long StringPos;                       // +0x08
    unsigned long Line;                            // +0x0c
};
bool CWStringParser::ReadNextNumber(CWParsedItem* item)
{
    unsigned long pos = StringPos;
    bool isFloat = false;
    wchar_t c = String[pos];
    for (;;) {
        if (!iswdigit(c)) {
            if (c == L'.') {
                if (isFloat) {
                    item->InitAsFloat(&String[StringPos], pos - StringPos);
                    StringPos = pos;
                    return true;
                }
            }
            else if (c != L'-' || pos != StringPos) {
                if (!isFloat)
                    item->InitAsInteger(&String[StringPos], pos - StringPos);
                else
                    item->InitAsFloat(&String[StringPos], pos - StringPos);
                StringPos = pos;
                return true;
            }
        }
        if (c == L'\n')
            ++Line;
        if (c == L'.')
            isFloat = true;
        c = String[++pos];
    }
}
