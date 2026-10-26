// Behaviour test for 009ba080: src/009ba080.cpp copied verbatim; the whitespace skip and the three item readers stubbed
// so the dispatch (digit or '-' + digit -> number, letter or '_' -> string, anything else -> symbol) is checked.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CStringParser::ReadNextItem(CParsedItem& item) -- skip whitespace and parse the next number, string or symbol into item
// Retry 10b (getsw sweep, FIXES B-15): the string position is read through an inline getter `GetStringPos()`; the plain member read gives the parked register/scheduling difference.
// Retail: Fable.exe (TLC, VC7.1) @ 0x009BA080, 136 bytes. Callees: SkipWhitespace @ 0x009B9C60, isdigit @ 0x00BFEDA6,
//          isalpha @ 0x00BFEDB8, ReadNextNumber @ 0x009B9EE0, ReadNextString @ 0x009B9FC0, ReadNextSymbol @ 0x009B99C0.
// Shape: a digit, or '-' followed by a digit, starts a number; a letter or '_' starts a string; anything else a symbol.
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
    unsigned long GetStringPos() const { return StringPos; }
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
    long SkipWhitespace();                                  // 0x009B9C60
    bool ReadNextItem(CParsedItem& item);                   // 0x009BA080
    long ReadAsInteger();
    float ReadAsFloat();
    CCharString ReadAsString();
    CCharString ReadAsSymbol();
    EParsedItemType PeekNextItemType();
protected:
    void Error(const CCharString& message);                 // 0x009B95F0
    bool ReadNextNumber(CParsedItem& item);                 // 0x009B9EE0
    bool ReadNextString(CParsedItem& item);                 // 0x009B9FC0
    bool ReadNextSymbol(CParsedItem& item);                 // 0x009B99C0
};

bool CStringParser::ReadNextItem(CParsedItem& item)
{
    SkipWhitespace();
    unsigned char c = String[CursorPos.GetStringPos()];
    if (isdigit(c) || (c == '-' && isdigit((unsigned char)String[CursorPos.GetStringPos() + 1])))
        return ReadNextNumber(item);
    else if (isalpha(c) || c == '_')
        return ReadNextString(item);
    else
        return ReadNextSymbol(item);
}
// ---- end source ----
static int g_skips = 0; static char g_last = 0;
long CStringParser::SkipWhitespace() { ++g_skips; while (String[CursorPos.StringPos] == ' ') ++CursorPos.StringPos; return 0; }
bool CStringParser::ReadNextNumber(CParsedItem&) { g_last = 'N'; return true; }
bool CStringParser::ReadNextString(CParsedItem&) { g_last = 'S'; return true; }
bool CStringParser::ReadNextSymbol(CParsedItem&) { g_last = 'Y'; return false; }
static char g_text[64];
static char Parse(const char* s)
{
    static unsigned char raw[sizeof(CStringParser)]; memset(raw, 0, sizeof(raw));
    CStringParser* p = (CStringParser*)raw; strcpy(g_text, s); p->String = g_text; p->CursorPos.StringPos = 0;
    CParsedItem& item = *(CParsedItem*)g_text;
    g_last = 0; p->ReadNextItem(item); return g_last;
}
int main()
{
    CHECK(Parse("  42") == 'N');
    CHECK(Parse("-7") == 'N');
    CHECK(Parse("-x") == 'Y');
    CHECK(Parse("abc") == 'S');
    CHECK(Parse("_id") == 'S');
    CHECK(Parse("{") == 'Y');
    CHECK(g_skips == 6);
    if (fails == 0) printf("009ba080_TEST PASS\n"); else printf("009ba080_TEST FAIL\n");
    return fails;
}