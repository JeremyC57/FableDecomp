// CStringParser::SkipUntilString(const CCharString& s) -- move the cursor to the next occurrence of s (true), or to the end of the text (false)
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B9A20, 125 bytes. Callees: CCharString::operator const char* @ 0x0099E4C0,
//          strstr @ 0x00BFEB3A, SetStringPos @ 0x009B9940, CalcInQuotes @ 0x009B9570 (not inlined here).
// Shape: MoveStringPos is the inline cursor advance (quote state first, then the position). The entry test goes through the inline
// IsAtEndOfText (re-indexes String[StringPos], CSE-d with GetCurrent's pointer): retail `lea edi,[eax+ecx] ; cmp byte ptr [edi],0`.
// IsAtEndOfText is an inline stand-in (no out-of-line copy; same as 0x009BA4A0).
extern "C" int __cdecl isspace(int c);   // 0x00BFEDBE (CRT)
extern "C" int __cdecl isdigit(int c);   // 0x00BFEDA6 (CRT)
extern "C" int __cdecl isalpha(int c);   // 0x00BFEDB8 (CRT)
extern "C" char* __cdecl strstr(const char* s, const char* find);   // 0x00BFEB3A (CRT)
class CCharString { public:                     // one CCharStringData pointer
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
class CParsedItem { public:                     // PDB: Type, a union of the numeric value, String
    EParsedItemType Type;
    union { long Integer; float Float; };
    CCharString     String;
};
// Retail layout (0x28 bytes; the Ego_r PDB's debug-build layout is 0x30, its list is 12 bytes there):
class CStringParser { public:
    virtual ~CStringParser();
    char*          String;               // +0x04
    unsigned long  BaseLineOffset;       // +0x08
    CCharString    Filename;             // +0x0c
    CCharString    TempCharString;       // +0x10
    void*          Comments;             // +0x14 STLport list node pointer
    void (__fastcall* ErrorCallback)(const CCharString&);   // +0x18
    CCursorPos     CursorPos;            // +0x1c
    bool SkipUntilString(const CCharString& s);
    void SetStringPos(unsigned long pos);                  // 0x009B9940
    const char* GetCurrent() const { return &String[CursorPos.StringPos]; }
    bool IsAtEndOfText() const { return String[CursorPos.StringPos] == 0; }
protected:
    bool CalcInQuotes(bool inQuotes, const char* from, const char* to) const;   // 0x009B9570
    __forceinline void MoveStringPos(unsigned long n)
    {
        if (n) {
            unsigned long pos = CursorPos.StringPos;
            const char* from = String + pos;
            CursorPos.InQuotes = CalcInQuotes(CursorPos.InQuotes, from, from + n);
            CursorPos.StringPos = pos + n;
        }
    }
};

extern "C" unsigned int __cdecl strlen(const char*);
#pragma intrinsic(strlen)

bool CStringParser::SkipUntilString(const CCharString& s)
{
    const char* p = GetCurrent();
    if (!IsAtEndOfText()) {
        const char* found = strstr(p, s);
        if (!found) {
            SetStringPos(strlen(String) + 1);
            return false;
        }
        MoveStringPos((found - String) - CursorPos.StringPos);
        return true;
    }
    return false;
}

#include <cstdio>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)
CCharString::CCharString() : PStringData(0) { }
CCharString::~CCharString() { }
CCharString::operator const char*() const { return (const char*)PStringData; }
void CStringParser::SetStringPos(unsigned long pos) { CursorPos.StringPos = pos; }
static int g_calc = 0; static const char* g_from; static const char* g_to;
bool CStringParser::CalcInQuotes(bool inQuotes, const char* from, const char* to) const
{
    ++g_calc; g_from = from; g_to = to;
    for (const char* q = from; q < to; ++q) if (*q == '"') inQuotes = !inQuotes;
    return inQuotes;
}

int main()
{
    static unsigned char mem[sizeof(CStringParser)];
    CStringParser* p = (CStringParser*)mem;
    char text[] = "ab \"x\" findme rest";
    p->String = text; p->CursorPos.InQuotes = false; p->CursorPos.StringPos = 1; p->CursorPos.Line = 0;
    CCharString key; key.PStringData = (void*)"findme";
    CHECK(p->SkipUntilString(key) == true);
    CHECK(p->CursorPos.StringPos == 7 && g_calc == 1 && g_from == text + 1 && g_to == text + 7);
    CHECK(p->CursorPos.InQuotes == false);
    // already at the match: zero move, CalcInQuotes not called
    CHECK(p->SkipUntilString(key) == true && p->CursorPos.StringPos == 7 && g_calc == 1);
    CCharString none; none.PStringData = (void*)"zzz";
    CHECK(p->SkipUntilString(none) == false && p->CursorPos.StringPos == 19 && g_calc == 1);
    // at the end of the text: false without searching
    p->CursorPos.StringPos = 18;
    CHECK(p->SkipUntilString(key) == false && p->CursorPos.StringPos == 18);
    if (fails == 0) printf("009b9a20_TEST PASS\n");
    return fails;
}