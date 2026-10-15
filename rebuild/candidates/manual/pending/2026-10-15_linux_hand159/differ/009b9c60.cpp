// CStringParser::SkipWhitespace() -- step the cursor past spaces/newlines, keep the in-quotes flag current, return the count skipped
// Grade: DIFFER(125v124) -- entry `lea esi,[eax+ecx] ; cmp byte ptr [esi],0` (retail) vs the folded `mov al,[esi+ecx] ; add esi,ecx ; test al,al` (ours), and the skipped count built in eax (`mov eax,esi ; sub eax,ebp`) where ours subtracts in esi
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B9C60, 124 bytes. Callees: isspace @ 0x00BFEDBE.
// Shape: MoveStringPos and CalcInQuotes are both inline members, so the quote scan is inlined here.
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
    long SkipWhitespace();
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

long CStringParser::SkipWhitespace()
{
    const char* p = String + CursorPos.StringPos;
    if (*p) {
        for (;;) {
            if (!isspace((unsigned char)*p) && *p != '\n')
                break;
            if (!*p)
                break;
            ++p;
        }
        long n = (p - String) - CursorPos.StringPos;
        MoveStringPos(n);
        return n;
    }
    return 0;
}
