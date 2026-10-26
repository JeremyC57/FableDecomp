// CStringParser::ReadAsStringUntilWhitespace() -- returns the text from the cursor up to the next whitespace (moving the cursor there), or an empty string at the end of the text
// Leaf: ReadAsStringUntilWhitespace
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BA4A0, 80 bytes. Callees:
// CStringParser::SkipUntilWhitespace @ 0x009B9CE0, CCharString::CCharString(const char*, long) @ 0x0099EBF0 (the empty
// literal 0x0122D70E with -1 on the end-of-text path).
// Layout: retail CStringParser is 0x28 bytes, not the PDB's 0x30 (engine/CStringParser.h): the comment list is ONE word
// here (head pointer, 16-byte self-linked head node) where the PDB has 12 bytes, so ErrorCallback sits at +0x18 and
// CursorPos (InQuotes, StringPos, Line) at +0x1C. The header disagrees with the bytes, so the class is modelled locally
// with the PDB member names (CBase root with the vtable 0x0122D874).
// Shape: the start pointer is kept in edi, `this` in esi; the end-of-text test is an inline member re-indexing
// String[StringPos] (CSE-d with the start pointer, so retail forms `lea edi,[eax+ecx]` and compares `byte ptr [edi]`
// from memory -- testing `*start` directly folds the load into `mov al,[edi+ecx] ; add edi,ecx ; test al,al` and
// recolours the length into eax); the length is (String + StringPos) - start after the skip; the reading path is the
// `if` body (fall-through, the empty-string return placed last); the result is returned through the hidden pointer.
// IsAtEndOfText is an inline stand-in (no out-of-line copy; name not in the PDB).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

class CBase {
public:
    CBase();                                       // 0x0099A2D0
    virtual ~CBase();
};

class CCharStringData { public:
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

class CParsedItem;
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
    long SkipUntilWhitespace();                    // 0x009B9CE0
    bool IsAtEndOfText() const { return String[CursorPos.StringPos] == 0; }
    CCharString ReadAsStringUntilWhitespace();
};

CCharString CStringParser::ReadAsStringUntilWhitespace()
{
    char* start = String + CursorPos.StringPos;
    if (!IsAtEndOfText()) {
        SkipUntilWhitespace();
        return CCharString(start, String + CursorPos.StringPos - start);
    }
    return CCharString("", -1);
}
// ---- harness ----
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static char g_last[64];
static long g_lastLen = -2;
CBase::CBase() { }
CBase::~CBase() { }
CCharString::CCharString(const char* text, long length)
{
    PStringData = 0;
    if (length < 0) length = (long)strlen(text);
    memcpy(g_last, text, length); g_last[length] = 0; g_lastLen = length;
}
CCharString::~CCharString() { }
long CStringParser::SkipUntilWhitespace()
{
    while (String[CursorPos.StringPos] != 0 && String[CursorPos.StringPos] != ' ')
        ++CursorPos.StringPos;
    return 1;
}
int main()
{
    static unsigned char mem[sizeof(CStringParser)];
    CStringParser* p = (CStringParser*)mem;
    char text[] = "hello world";
    p->String = text;
    p->CursorPos.StringPos = 0;
    { CCharString s = p->ReadAsStringUntilWhitespace(); }
    CHECK(g_lastLen == 5 && strcmp(g_last, "hello") == 0 && p->CursorPos.StringPos == 5);
    p->CursorPos.StringPos = 6;
    { CCharString s = p->ReadAsStringUntilWhitespace(); }
    CHECK(g_lastLen == 5 && strcmp(g_last, "world") == 0 && p->CursorPos.StringPos == 11);
    { CCharString s = p->ReadAsStringUntilWhitespace(); }
    CHECK(g_lastLen == 0 && g_last[0] == 0 && p->CursorPos.StringPos == 11);
    if (fails == 0) printf("009ba4a0_TEST PASS\n");
    return fails;
}