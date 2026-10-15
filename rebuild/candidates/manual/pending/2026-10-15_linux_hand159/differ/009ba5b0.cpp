// CStringParser::ReadAsUHUGE() -- skip whitespace, read a run of decimal digits as an unsigned 64-bit value and move the cursor past it
// Grade: DIFFER(150v155) -- the cursor advance after CalcInQuotes: retail `mov eax,[edi+20h] ; add eax,esi ; mov [edi+20h],eax`, ours `add dword ptr [edi+20h],esi` (+=, assignment, getter and pointer forms all fold)
// Retail: Fable.exe (TLC, VC7.1) @ 0x009BA5B0, 155 bytes (catalog label "CStringParser::ReadInt64"; PDB name ?ReadAsUHUGE@CStringParser@@QAE_KXZ).
// Callees: SkipWhitespace @ 0x009B9C60, isdigit @ 0x00BFEDA6, __allmul @ 0x00BFEB50, CalcInQuotes @ 0x009B9570 (out of line here).
// Shape: MoveStringPos is the inline cursor advance; the digit is sign-extended (`*p - '0'` on a plain char) before the 64-bit multiply.
// Leaf: ReadAsUHUGE
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
    long SkipWhitespace();                                  // 0x009B9C60
    unsigned __int64 ReadAsUHUGE();
protected:
    bool CalcInQuotes(bool inQuotes, const char* from, const char* to) const;   // 0x009B9570
    __forceinline void MoveStringPos(unsigned long n)
    {
        if (n) {
            CursorPos.InQuotes = CalcInQuotes(CursorPos.InQuotes, String + CursorPos.StringPos, String + CursorPos.StringPos + n);
            CursorPos.StringPos += n;
        }
    }
};

unsigned __int64 CStringParser::ReadAsUHUGE()
{
    SkipWhitespace();
    const char* p = String + CursorPos.StringPos;
    unsigned __int64 value = 0;
    while (*p && isdigit((unsigned char)*p)) {
        value = value * 10 + (*p - '0');
        ++p;
    }
    MoveStringPos((p - String) - CursorPos.StringPos);
    return value;
}
