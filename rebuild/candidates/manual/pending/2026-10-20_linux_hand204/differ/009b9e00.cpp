// Grade: DIFFER(219v219) -- one operand order: the quote-scan end pointer is `mov edx,edi(pos) ; add edx,[esi+4](String)` in retail, `mov edx,[esi+4] ; add edx,edi` here
// CStringParser::SkipPastNextItemIfIs(const CCharString& item) -- after whitespace, if the text at the cursor starts with `item`, step past it (keeping the in-quotes flag current) and return true; otherwise restore the cursor and return false
// Leaf: SkipPastNextItemIfIs
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B9E00, 217 bytes, `ret 4`.
// Callee: CStringParser::SkipWhitespace @ 0x009B9C60.
// Layout: the retail 0x28-byte CStringParser of the landed siblings (String +0x04, CursorPos +0x1c = InQuotes, StringPos
//   +0x20, Line +0x24); CCharString's data block is {Chars, Length}, the empty text the shared "" at 0x0129AAF4.
// Shape: the whole cursor is saved as a 12-byte local copy before the skip and written back on a mismatch; the
//   comparison re-reads the item's length through the inline null-safe GetLength() each pass (signed index) and its text
//   through the ""-preloading GetPString(); a full match runs the inline MoveStringPos (quote toggling over the
//   consumed characters, `n > 0` guard), the shape of the CStringParser Skip* family.
struct CCharStringData {
    const char*   Chars;                 // +0
    long          Length;                // +4
};
struct CCharString {                     // one CCharStringData pointer
    CCharStringData* PStringData;
    long GetLength() const { return PStringData == 0 ? 0 : PStringData->Length; }
    const char* GetPString() const { const char* s = ""; if (PStringData) s = PStringData->Chars; return s; }
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
    long SkipWhitespace();               // 0x009B9C60
    bool SkipPastNextItemIfIs(const CCharString& item);
protected:
    __forceinline bool CalcInQuotes(bool inQuotes, const char* from, const char* to) const
    {
        for (const char* p = from; p != to; ++p)
            if (*p == '"' || *p == '\'')
                inQuotes = !inQuotes;
        return inQuotes;
    }
    __forceinline void MoveStringPos(unsigned long n)
    {
        if (n > 0) {
            unsigned long pos = CursorPos.StringPos;
            const char* from = String + pos;
            CursorPos.InQuotes = CalcInQuotes(CursorPos.InQuotes, from, String + pos + n);
            CursorPos.StringPos = pos + n;
        }
    }
};
bool CStringParser::SkipPastNextItemIfIs(const CCharString& item)
{
    CCursorPos saved = CursorPos;
    SkipWhitespace();
    long i = 0;
    while (i < item.GetLength() && item.GetPString()[i] == String[CursorPos.StringPos + i])
        ++i;
    if (i == item.GetLength()) {
        MoveStringPos(i);
        return true;
    }
    CursorPos = saved;
    return false;
}
