// CStringParser::ReadNextString(CParsedItem& item) -- scan an identifier-like run at the cursor (digits, letters, '_'), store it in the item as a string, and step the cursor past it
// Leaf: ReadNextString
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B9FC0, 191 bytes, `ret 4`.
// Identity: Ego_r.pdb `?ReadNextString@CStringParser@@IAE_NAAVCParsedItem@@@Z` (the landed ReadAsSymbol 0x009BA790
//   declares it at this address).
// Callees: isdigit @ 0x00BFEDA6, isalpha @ 0x00BFEDB8 (CRT), CCharString(const char*, long) @ 0x0099EBF0,
//   CCharString::operator const char* @ 0x0099E4C0, CCharString::SetAsSubString(const char*, long) @ 0x0099EF40 (the
//   inline CParsedItem::InitAsString), CStringParser::CalcInQuotes @ 0x009B9570 (the inline MoveStringPos), ~CCharString
//   @ 0x0099EAE0.
// Layout: the retail 0x28-byte CStringParser of the landed siblings (String +0x04, CursorPos +0x1c = InQuotes, StringPos
//   +0x20, Line +0x24).
// Shape: the sibling of ReadNextNumber 0x009B9EE0: the text is a CCharString local over [start, p); the item is filled
//   through the inline InitAsString(const char*, unsigned long) (substring copy, then Type = PIT_STRING) with the length
//   taken first through the null-safe GetLength(); MoveStringPos keeps StringPos in a local across CalcInQuotes.
extern "C" int __cdecl isdigit(int c);   // 0x00BFEDA6 (CRT)
extern "C" int __cdecl isalpha(int c);   // 0x00BFEDB8 (CRT)
struct CCharStringData {
    const char*   Chars;                 // +0
    unsigned long Length;                // +4
};
struct CCharString {                     // one CCharStringData pointer
    CCharStringData* PStringData;
    CCharString(const char* text, long length);   // 0x0099EBF0
    ~CCharString();                      // 0x0099EAE0
    operator const char*() const;        // 0x0099E4C0
    void SetAsSubString(const char* text, long length);   // 0x0099EF40
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->Length; }
};
struct CCursorPos {                      // PDB: InQuotes / StringPos / Line (12 bytes)
    bool          InQuotes;
    unsigned long StringPos;
    unsigned long Line;
};
enum EParsedItemType { PIT_NONE = 0, PIT_SYMBOL = 1, PIT_STRING = 2, PIT_INTEGER = 3, PIT_FLOAT = 4 };
struct CParsedItem {                     // PDB: Type, a union of the numeric value, String
    EParsedItemType Type;
    union { long Integer; float Float; };
    CCharString     String;
    void InitAsString(const char* text, unsigned long length) { String.SetAsSubString(text, length); Type = PIT_STRING; }
};
// Retail layout (0x28 bytes; the Ego_r PDB's debug-build layout is 0x30):
struct CStringParser {
    virtual ~CStringParser();
    char*          String;               // +0x04
    unsigned long  BaseLineOffset;       // +0x08
    CCharString    Filename;             // +0x0c
    CCharString    TempCharString;       // +0x10
    void*          Comments;             // +0x14 STLport list node pointer
    void (__fastcall* ErrorCallback)(const CCharString&);   // +0x18
    CCursorPos     CursorPos;            // +0x1c
protected:
    bool CalcInQuotes(bool inQuotes, const char* from, const char* to) const;   // 0x009B9570
    void MoveStringPos(unsigned long n)
    {
        if (n > 0) {
            unsigned long pos = CursorPos.StringPos;
            const char* from = String + pos;
            CursorPos.InQuotes = CalcInQuotes(CursorPos.InQuotes, from, from + n);
            CursorPos.StringPos = pos + n;
        }
    }
    bool ReadNextString(CParsedItem& item);
};
bool CStringParser::ReadNextString(CParsedItem& item)
{
    const char* p = String + CursorPos.StringPos;
    while (*p) {
        if (!(isdigit((unsigned char)*p) || isalpha((unsigned char)*p) || *p == '_'))
            break;
        ++p;
    }
    const char* start = String + CursorPos.StringPos;
    CCharString text(start, p - start);
    item.InitAsString(text, text.GetLength());
    MoveStringPos(p - (String + CursorPos.StringPos));
    return true;
}
