// Behaviour test for 009ba390: src/009ba390.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CStringParser::ReadLineAsString()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CStringParser::ReadLineAsString() -- returns the text from the cursor through the end of the line and its newline characters (the cursor moves past them), or an empty string when the cursor is at the end of the text
// Leaf: ReadLineAsString
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BA390, 71 bytes (+9 int3 padding). Callees:
// CStringParser::SkipPastNewline @ 0x009B9D70 (long), CCharString::CCharString(const char*, long) @ 0x0099EBF0.
// Layout: retail CStringParser is 0x28 bytes, not the PDB's 0x30 (engine/CStringParser.h): the comment list is ONE word
// here (head pointer, 16-byte self-linked head node) where the PDB has 12 bytes, so ErrorCallback sits at +0x18 and
// CursorPos (InQuotes, StringPos, Line) at +0x1C. The header disagrees with the bytes, so the class is modelled locally
// with the PDB member names (CBase root with the vtable 0x0122D874).
// Shape: the result is returned through the hidden pointer; the start is String + StringPos taken before the skip; `this`
// stays in ecx across SkipPastNewline because the callee's body is in the same TU (callee register-usage knowledge).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

class CBase {
public:
    CBase();                                       // 0x0099A2D0
    virtual ~CBase();
};

struct CCharStringData {
    char*         PData;                           // +0x0
    unsigned long Length;                          // +0x4
};
extern char g_emptyLiteral_0129aaf4[];             // 0x0129AAF4: ""

class CCharString {
public:
    CCharStringData* PStringData;
    CCharString();                                 // 0x0099E4B0
    CCharString(const CCharString& other);         // 0x0099EC30
    CCharString(const char* text, long length);    // 0x0099EBF0 (-1 = strlen)
    ~CCharString();                                // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    char* GetString() const { return PStringData == 0 ? g_emptyLiteral_0129aaf4 : PStringData->PData; }
};

struct CStringParserCommentList {                  // retail list<CStringParserCommentBoundaries>: the head pointer only
    struct SNode { SNode* Next; SNode* Prev; char* PStart; char* PEnd; };
    SNode* m_pHead;
    CStringParserCommentList() : m_pHead(0)
    {
        SNode* node = (SNode*)malloc(sizeof(SNode));
        node->Next = node;
        node->Prev = node;
        m_pHead = node;
    }
    CStringParserCommentList(const CStringParserCommentList& other);   // 0x00463871
};

struct CCursorPos {
    bool          InQuotes;                        // +0x0
    unsigned long StringPos;                       // +0x4
    unsigned long Line;                            // +0x8
};

struct CParsedItem;
typedef void (*TStringParserErrorCallback)(CCharString* message);

class CStringParser : public CBase {               // vtable 0x0122D874
public:
    char*                      String;             // +0x04
    unsigned long              BaseLineOffset;     // +0x08
    CCharString                Filename;           // +0x0C
    CCharString                TempCharString;     // +0x10
    CStringParserCommentList   Comments;           // +0x14
    TStringParserErrorCallback ErrorCallback;      // +0x18
    CCursorPos                 CursorPos;          // +0x1C
    long SkipPastNewline();                        // 0x009B9D70
    CCharString ReadLineAsString();
};

// Same-TU body of CStringParser::SkipPastNewline @ 0x009B9D70 (retail's TU; this body differs from retail only in the
// scan's register choice): with it above the caller VC7.1 knows the call leaves ecx alone, as retail's codegen does.
__declspec(noinline) long CStringParser::SkipPastNewline()
{
    const char* start = String + CursorPos.StringPos;
    const char* p = start;
    while (*p) {
        if (*p == '\n' || *p == '\r') {
            while (*p == '\n' || *p == '\r') ++p;
            break;
        }
        ++p;
    }
    long count = p - start;
    if (count) {
        const char* c = String + CursorPos.StringPos;
        bool inQuotes = CursorPos.InQuotes;
        for (; c != String + CursorPos.StringPos + count; ++c)
            if (*c == '"' || *c == '\'') inQuotes = !inQuotes;
        CursorPos.StringPos += count;
        CursorPos.InQuotes = inQuotes;
    }
    return count;
}

CCharString CStringParser::ReadLineAsString()
{
    char* start = String + CursorPos.StringPos;
    if (SkipPastNewline())
        return CCharString(start, String + CursorPos.StringPos - start);
    return CCharString("", -1);
}

// ---- stubs and driver ----
#include <cstdlib>
char g_emptyLiteral_0129aaf4[] = "";
static int g_live = 0, g_bases = 0;
CBase::CBase() { ++g_bases; }
CBase::~CBase() { --g_bases; }
static void SetText(CCharString& s, const char* text, long n)
{
    if (n < 0) n = (long)strlen(text);
    s.PStringData = (CCharStringData*)malloc(sizeof(CCharStringData));
    s.PStringData->PData = (char*)malloc(n + 1); memcpy(s.PStringData->PData, text, n); s.PStringData->PData[n] = 0;
    s.PStringData->Length = n;
}
CCharString::CCharString() : PStringData(0) { ++g_live; }
CCharString::CCharString(const char* text, long length) : PStringData(0) { ++g_live; SetText(*this, text, length); }
CCharString::CCharString(const CCharString& other) : PStringData(0) { ++g_live; SetText(*this, other.GetString(), -1); }
CCharString::~CCharString() { --g_live; if (PStringData) { free(PStringData->PData); free(PStringData); } }
CCharString& CCharString::operator=(const CCharString& other) { CCharString t(other); PStringData = t.PStringData; t.PStringData = 0; return *this; }
int main()
{
    CStringParser* parser = new CStringParser;
    char text[] = "ab\"c\r\n\ndef";
    parser->String = text; parser->CursorPos.StringPos = 0; parser->CursorPos.InQuotes = false; parser->CursorPos.Line = 0;
    int base = g_live;
    {
        CCharString line = parser->ReadLineAsString();
        CHECK(strcmp(line.GetString(), "ab\"c\r\n\n") == 0 && line.PStringData->Length == 7);
        CHECK(parser->CursorPos.StringPos == 7 && parser->CursorPos.InQuotes);
    }
    {
        CCharString line = parser->ReadLineAsString();          // last line without a newline: the rest of the text
        CHECK(strcmp(line.GetString(), "def") == 0 && parser->CursorPos.StringPos == 10 && parser->CursorPos.InQuotes);
    }
    {
        CCharString line = parser->ReadLineAsString();          // at the end: empty string, cursor unchanged
        CHECK(strcmp(line.GetString(), "") == 0 && parser->CursorPos.StringPos == 10);
    }
    CHECK(g_live == base);
    char text2[] = "'x'\n'";
    parser->String = text2; parser->CursorPos.StringPos = 0; parser->CursorPos.InQuotes = false;
    {
        CCharString line = parser->ReadLineAsString();          // two quotes toggle back
        CHECK(strcmp(line.GetString(), "'x'\n") == 0 && !parser->CursorPos.InQuotes && parser->CursorPos.StringPos == 4);
    }
    if (fails == 0) printf("009ba390_TEST PASS\n");
    return fails;
}
