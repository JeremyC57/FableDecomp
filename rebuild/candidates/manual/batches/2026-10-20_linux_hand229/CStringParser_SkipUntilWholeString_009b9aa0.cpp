// CStringParser::SkipUntilWholeString(const CCharString& word) -- find the next whole-word occurrence of `word` (not preceded or followed by a letter, digit or '_') and move the cursor to it; false when there is none
// Leaf: SkipUntilWholeString
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B9AA0, 241 bytes (+15 int3 padding), `ret 4`.
// Callees: CCharString::operator const char* 0x0099E4C0, strstr 0x00BFEB3A, isalpha 0x00BFEDB8, isdigit 0x00BFEDA6 (CRT),
//   CStringParser::CalcInQuotes(bool, const char*, const char*) 0x009B9570 (landed; inside the inline MoveStringPos).
// Layout: the retail 0x28-byte CStringParser of the landed siblings (String +0x04, CursorPos +0x1c = InQuotes, StringPos
//   +0x20, Line +0x24; engine/CStringParser.h carries the PDB's 0x30-byte debug layout, so the model is local).
// Shape: strstr from the cursor; for each hit the character before (unless the hit is at offset 0) and the character
//   `word.GetLength()` past the hit (null-safe inline length) must not be alphanumeric or '_'; the next search starts
//   one past the hit. The accepted hit is consumed through the landed ReadNextString's inline MoveStringPos. Registers:
//   one long local carries the cursor position and then the hit offset (ebp), and the hit pointer starts life as the
//   search start `String + pos` (edi); a fresh `strstr(String + pos, word)` initialiser swaps the two.
extern "C" int __cdecl isdigit(int c);   // 0x00BFEDA6 (CRT)
extern "C" int __cdecl isalpha(int c);   // 0x00BFEDB8 (CRT)
extern "C" char* __cdecl strstr(const char* text, const char* sub);   // 0x00BFEB3A (CRT)
struct CCharStringData {
    const char*   Chars;                 // +0
    unsigned long Length;                // +4
};
struct CCharString {                     // one CCharStringData pointer
    CCharStringData* PStringData;
    operator const char*() const;        // 0x0099E4C0
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->Length; }
};
struct CCursorPos {                      // PDB: InQuotes / StringPos / Line (12 bytes)
    bool          InQuotes;
    unsigned long StringPos;
    unsigned long Line;
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
public:
    bool SkipUntilWholeString(const CCharString& word);
};

bool CStringParser::SkipUntilWholeString(const CCharString& word)
{
    long offset = CursorPos.StringPos;
    char* found = String + offset;
    found = strstr(found, word);
    while (found) {
        offset = found - String;
        unsigned char before;
        if (offset == 0 || !(isalpha(before = String[offset - 1]) || isdigit(before) || before == '_')) {
            unsigned char after = String[word.GetLength() + offset];
            if (!(isalpha(after) || isdigit(after) || after == '_')) {
                MoveStringPos(found - (String + CursorPos.StringPos));
                return true;
            }
        }
        found = strstr(found + 1, word);
    }
    return false;
}
