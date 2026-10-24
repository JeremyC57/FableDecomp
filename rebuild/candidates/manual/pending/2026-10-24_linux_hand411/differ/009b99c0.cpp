// CStringParser::ReadNextSymbol(CParsedItem& item) -- take the single character at the cursor as a symbol item and step the cursor past it; false at the end of the text
// Leaf: ?ReadNextSymbol@CStringParser@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B99C0, 85 bytes (+ int3 padding), thiscall `ret 4`.
// Grade: DIFFER(84v85) -- after the SetAsSubString call retail loads String into edx (`mov edx,[esi+4] ; mov edi,[esi+20h] ;
//   lea eax,[edx+edi]`) and InQuotes into edx before the pushes; ours uses eax/`add` and loads InQuotes after pushing
//   `from + 1` (register/scheduling only; a same-TU CalcInQuotes body with `StringPos += n` gave the same code).
// Name: Ego_r.pdb `?ReadNextSymbol@CStringParser@@IAE_NAAVCParsedItem@@@Z`; the landed ReadNextItem 0x009BA080 and
// ReadAsSymbol 0x009BA790 already declare it at 0x009B99C0. Label correction: `CStringParser_ParseSingleChar`.
// Callees: CCharString::SetAsSubString(const char*, long) @ 0x0099EF40 (the inline CParsedItem::InitAsSymbol),
// CStringParser::CalcInQuotes(bool, const char*, const char*) const @ 0x009B9570 (the inline MoveStringPos).
// Layout: the retail 0x28-byte CStringParser of the landed siblings (String +0x04, CursorPos +0x1C = InQuotes,
// StringPos +0x20, Line +0x24); CParsedItem {Type; numeric union; CCharString String +0x08}.
// Shape: the sibling of ReadNextString 0x009B9FC0: the entry test goes through the inline GetCurrent()/IsAtEndOfText()
// pair of the landed SkipUntilString 0x009B9A20 (`add eax,ecx ; cmp byte ptr [eax],0`), in the positive-if form; the item takes a one-character substring and Type = PIT_SYMBOL
// (1); MoveStringPos(1) folds its `n > 0` test away and keeps StringPos in edi across CalcInQuotes (`inc edi`).

class CCharString {                     // one CCharStringData pointer
public:
    void SetAsSubString(const char* text, long length);    // 0x0099EF40
private:
    void* PStringData;
};

class CCursorPos {                      // PDB: InQuotes / StringPos / Line (12 bytes)
public:
    bool          InQuotes;             // +0x00
    unsigned long StringPos;            // +0x04
    unsigned long Line;                 // +0x08
};

enum EParsedItemType { PIT_NONE = 0, PIT_SYMBOL = 1, PIT_STRING = 2, PIT_INTEGER = 3, PIT_FLOAT = 4 };

class CParsedItem {                     // PDB: Type, a union of the numeric value, String
public:
    EParsedItemType Type;               // +0x00
    union { long Integer; float Float; };   // +0x04
    CCharString     String;             // +0x08
    void InitAsSymbol(const char* text, unsigned long length) { String.SetAsSubString(text, length); Type = PIT_SYMBOL; }
};

// Retail layout (0x28 bytes; the Ego_r PDB's debug-build layout is 0x30):
class CStringParser {
public:
    virtual ~CStringParser();
    char*          String;               // +0x04
    unsigned long  BaseLineOffset;       // +0x08
    CCharString    Filename;             // +0x0c
    CCharString    TempCharString;       // +0x10
    void*          Comments;             // +0x14 STLport list node pointer
    void (__fastcall* ErrorCallback)(const CCharString&);   // +0x18
    CCursorPos     CursorPos;            // +0x1c
    const char* GetCurrent() const { return &String[CursorPos.StringPos]; }
    bool IsAtEndOfText() const { return String[CursorPos.StringPos] == 0; }
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
    bool ReadNextSymbol(CParsedItem& item);
};

bool CStringParser::ReadNextSymbol(CParsedItem& item)
{
    const char* p = GetCurrent();
    if (!IsAtEndOfText()) {
        item.InitAsSymbol(p, 1);
        MoveStringPos(1);
        return true;
    }
    return false;
}
