// CStringParser::SetStringPos(unsigned long) -- moves the cursor to the given offset and recomputes whether that position is inside a quoted string (each ' or " before it toggles the state)
// Leaf: SetStringPos
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B9940, 62 bytes. No callees.
// Layout: retail CStringParser is 0x28 bytes, not the PDB's 0x30 (engine/CStringParser.h): the comment list is ONE word
// here (head pointer, 16-byte self-linked head node) where the PDB has 12 bytes, so ErrorCallback sits at +0x18 and
// CursorPos (InQuotes, StringPos, Line) at +0x1C. The header disagrees with the bytes, so the class is modelled locally
// with the PDB member names (CBase root with the vtable 0x0122D874).
// Shape: the position is stored before the scan (the char reads may alias it); the scan is the class's own
// CalcInQuotes(false, String, String + pos) inlined (its parameters are what move `this` to esi and put the flag in cl;
// the same loop written out keeps `this` in ecx with the flag in bl); speed TU (loop head aligned with `lea ecx,[ecx]`).
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
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
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
    void SetStringPos(unsigned long pos);
protected:
    // CStringParser::CalcInQuotes (Ego_r.pdb `?CalcInQuotes@CStringParser@@IBE_N_NPBD1@Z`, out of line at 0x009B9570),
    // inlined here.
    bool CalcInQuotes(bool seed, const char* p, const char* end) const
    {
        bool inQuotes = seed;
        while (p != end) {
            char c = *p;
            if (c == '"' || c == '\'')
                inQuotes = !inQuotes;
            ++p;
        }
        return inQuotes;
    }
};

void CStringParser::SetStringPos(unsigned long pos)
{
    CursorPos.StringPos = pos;
    CursorPos.InQuotes = CalcInQuotes(false, String, String + pos);
}
// ---- harness ----
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
int main()
{
    static unsigned char mem[sizeof(CStringParser)];
    CStringParser* p = (CStringParser*)mem;     // only String and CursorPos are touched
    char text[] = "a \"b\" 'c d' e\"f";
    p->String = text;
    p->CursorPos.InQuotes = true; p->CursorPos.Line = 7;
    p->SetStringPos(0);
    CHECK(p->CursorPos.StringPos == 0 && p->CursorPos.InQuotes == false && p->CursorPos.Line == 7);
    p->SetStringPos(3);                         // after the first "
    CHECK(p->CursorPos.StringPos == 3 && p->CursorPos.InQuotes == true);
    p->SetStringPos(5);                         // after the closing "
    CHECK(p->CursorPos.InQuotes == false);
    p->SetStringPos(8);                         // inside '...'
    CHECK(p->CursorPos.InQuotes == true);
    p->SetStringPos(11);
    CHECK(p->CursorPos.InQuotes == false);
    p->SetStringPos((unsigned long)strlen(text));   // the trailing unmatched "
    CHECK(p->CursorPos.InQuotes == true && p->CursorPos.StringPos == strlen(text));
    if (fails == 0) printf("009b9940_TEST PASS\n");
    return fails;
}