// CStringParser::SkipUntilWhitespace() -- step the cursor to the next whitespace character (or the end), keep the in-quotes flag current, return the count skipped
// Grade: DIFFER(131v131) -- one instruction: retail computes the skipped count as `mov eax,esi ; sub eax,ebp`, ours `sub esi,ebp ; mov eax,esi` (register colouring only)
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B9CE0, 131 bytes. Callees: isspace @ 0x00BFEDBE.
// Shape: MoveStringPos and CalcInQuotes are inline members, both inlined here.
extern "C" int __cdecl isspace(int c);   // 0x00BFEDBE (CRT)
extern "C" int __cdecl isdigit(int c);   // 0x00BFEDA6 (CRT)
extern "C" int __cdecl isalpha(int c);   // 0x00BFEDB8 (CRT)
extern "C" char* __cdecl strstr(const char* s, const char* find);   // 0x00BFEB3A (CRT)
struct CCharString {                     // one CCharStringData pointer
    void* PStringData;
    CCharString();                       // 0x0099E4B0
    CCharString(const char* text, long length);   // 0x0099EBF0
    CCharString(const CCharString& other);        // 0x0099EC30
    ~CCharString();                      // 0x0099EAE0
    operator const char*() const;        // 0x0099E4C0
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
};
// Retail layout (0x28 bytes; the Ego_r PDB's debug-build layout is 0x30, its list is 12 bytes there):
struct CStringParser {
    virtual ~CStringParser();
    char*          String;               // +0x04
    unsigned long  BaseLineOffset;       // +0x08
    CCharString    Filename;             // +0x0c
    CCharString    TempCharString;       // +0x10
    void*          Comments;             // +0x14 STLport list node pointer
    void (__fastcall* ErrorCallback)(const CCharString&);   // +0x18
    CCursorPos     CursorPos;            // +0x1c
    long SkipUntilWhitespace();
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
        if (n) {
            CursorPos.InQuotes = CalcInQuotes(CursorPos.InQuotes, String + CursorPos.StringPos, String + CursorPos.StringPos + n);
            CursorPos.StringPos += n;
        }
    }
};

long CStringParser::SkipUntilWhitespace()
{
    const char* p = String + CursorPos.StringPos;
    while (!isspace((unsigned char)*p) && *p)
        ++p;
    long n = (p - String) - CursorPos.StringPos;
    MoveStringPos(n);
    return n;
}
