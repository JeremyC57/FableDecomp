// CStringParser::SkipPastWholeString(const CCharString& word) -- find the next whole-word occurrence of `word` and move the cursor past it; false (cursor unchanged) when there is none
// Leaf: ?SkipPastWholeString@CStringParser@@QAE_NABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B9C00, 81 bytes, thiscall, `ret 4`.
// Name (2026-10-31, hand621 NAME_FIXES): Ego_r.pdb donor order SkipUntilWholeString 0x00B0E630 -> SkipPastString 0x00B0E710
//   -> SkipPastWholeString 0x00B0E750 -> SkipWhitespace 0x00B0E790 = retail 0x009B9AA0 / 0x009B9BA0 / 0x009B9C00 / 0x009B9C60;
//   the landed caller 0x009AF1E0 binds ?SkipPastWholeString@CStringParser@@QAE_NABVCCharString@@@Z. The previous landed
//   spelling (CThingCreatureBase::SerialiseGameState calling CThingBase::SerialiseGameState_009b9aa0 and
//   CThingCreatureBase::SerialiseElements 0x009B9570) was a propagated label over the same bytes.
// Callees: CStringParser::SkipUntilWholeString(const CCharString&) 0x009B9AA0 (landed), CStringParser::CalcInQuotes(bool,
//   const char*, const char*) const 0x009B9570 (protected).
// Layout: the retail 0x28-byte CStringParser of the landed siblings (String +0x04, CursorPos +0x1c = InQuotes, StringPos
//   +0x20, Line +0x24; engine/CStringParser.h carries the PDB's 0x30-byte debug layout, so the model is local, as 0x009B9AA0).
// Shape: SkipUntilWholeString leaves the cursor on the match; the word's length (null string data: no move) is then consumed
//   in place, the new StringPos stored before the CalcInQuotes result (retail order; the 0x009B9AA0 MoveStringPos inline
//   stores InQuotes first, so it is not used here); true on a match.
class CCharStringData { public:
    const char*   Chars;                 // +0
    unsigned long Length;                // +4
};
class CCharString { public:                     // one CCharStringData pointer
    CCharStringData* PStringData;
};
struct CCursorPos {                      // PDB: InQuotes / StringPos / Line (12 bytes)
    bool          InQuotes;
    unsigned long StringPos;
    unsigned long Line;
};
// Retail layout (0x28 bytes; the Ego_r PDB's debug-build layout is 0x30):
class CStringParser { public:
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
public:
    bool SkipUntilWholeString(const CCharString& word);    // 0x009B9AA0
    bool SkipPastWholeString(const CCharString& word);
};

bool CStringParser::SkipPastWholeString(const CCharString& word)
{
    if (SkipUntilWholeString(word)) {
        if (word.PStringData != 0) {
            unsigned long n = word.PStringData->Length;
            if (n > 0) {
                unsigned long pos = CursorPos.StringPos;
                const char* from = String + pos;
                bool inQuotes = CalcInQuotes(CursorPos.InQuotes, from, from + n);
                CursorPos.StringPos = pos + n;
                CursorPos.InQuotes = inQuotes;
            }
        }
        return true;
    }
    return false;
}
