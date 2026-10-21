// Behaviour test for 005dc650: src/005dc650.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CAnimComponentCombatComboChain::Initialise(CPersistContext&, const CCharString&)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CAnimComponentCombatComboChain::Initialise(CPersistContext&, const CCharString&) -- reset, parse "<id>" or "<stage>,<id>" from the text into ComboStage / ComboID
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DC650, length 210 bytes (virtual, `ret 8`), always returns true.
// Callees: this's virtual slot 8 (+0x20, the component reset), CCharString::CCharString(const char*, long)
// @ 0x0099EBF0 ("" and ","), CStringParser::CStringParser @ 0x00404720 (landed), CCharString::~CCharString @ 0x0099EAE0,
// TransferIn(CPersistContext*, CStringParser*, long*) @ 0x009E2590 (__fastcall), CStringParser::SkipPastNextItemIfIs(const CCharString&)
// @ 0x009B9E00, std::list<CStringParserCommentBoundaries>::~list @ 0x004046B0 (landed), CBase::~CBase @ 0x0099A2E0.
// Shape: the landed sibling CAnimComponentFlags::Initialise (0x005DC3C0) parser and inline parse helper. The first value is
// homed in the dead context-argument slot (context itself stays in edi), the "," string and the second value in the
// dead text slot: the "," string is a NAMED local in its own block around the skip call (a bare temporary takes the
// context slot and pushes the first value into the text slot); the skip result is a bool declared before that block (bl). A single value is the
// ComboID; a second value that is not -1 makes the first the ComboStage.
// Layout: rebuild/include/engine/CAnimComponentCombatComboChain.h is data-only (ComboStage +0x08, ComboID +0x0c), so the
// polymorphic class is modelled locally.
// Leaf: ?Initialise@CAnimComponentCombatComboChain
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length); // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};

struct CBase {
    virtual void v00();
    ~CBase();                                   // 0x0099A2E0
};

struct CStringParserCommentList {               // retail std::list<CStringParserCommentBoundaries>: sentinel only
    void* m_pHead;
    ~CStringParserCommentList();                // 0x004046B0
};

struct CStringParser : CBase {                  // retail 0x28 bytes
    char* String;                               // +0x04
    unsigned long BaseLineOffset;               // +0x08
    CCharString Filename;                       // +0x0c
    CCharString TempCharString;                 // +0x10
    CStringParserCommentList Comments;          // +0x14
    void (*ErrorCallback)(CCharString*);        // +0x18
    long CursorPos[3];                          // +0x1c CCursorPos
    CStringParser(const CCharString& text, void (*errorCallback)(CCharString*), const CCharString& filename,
                  long baseLineOffset);         // 0x00404720
    bool SkipPastNextItemIfIs(const CCharString& item);   // 0x009B9E00
};

struct CPersistContext;

void __fastcall TransferIn(CPersistContext* context, CStringParser* parser, long* value);   // 0x009E2590

// The parse step as an inline helper: its local is homed in the caller's dead argument slot.
inline long ParseLong(CPersistContext& context, CStringParser& parser)
{
    long value;
    TransferIn(&context, &parser, &value);
    return value;
}

struct CAnimComponentBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void Reset();                       // slot 8 (+0x20)
    long _base04;                               // +0x04
};

struct CAnimComponentCombatComboChain : CAnimComponentBase {
    long ComboStage;                            // +0x08
    long ComboID;                               // +0x0c
    virtual bool Initialise(CPersistContext& context, const CCharString& text);
};

bool CAnimComponentCombatComboChain::Initialise(CPersistContext& context, const CCharString& text)
{
    Reset();
    CStringParser parser(text, 0, CCharString("", -1), 0);
    long first = ParseLong(context, parser);
    bool hasSecond;
    {
        CCharString comma(",", -1);
        hasSecond = parser.SkipPastNextItemIfIs(comma);
    }
    if (hasSecond) {
        long second = ParseLong(context, parser);
        if (second != -1) {
            ComboStage = first;
            ComboID = second;
            return true;
        }
    }
    ComboID = first;
    return true;
}

// ---- stubs and driver ----
// stubs: strings count constructions/destructions and keep the text; the parser hands out scripted values
static int g_strCtor = 0, g_strDtor = 0, g_resets = 0, g_parserCtor = 0, g_skipCalls = 0, g_transfers = 0;
static const char* g_skipItem = 0; static const CCharString* g_parsedText = 0;
static long g_values[4]; static int g_nextValue = 0; static bool g_hasComma = false;
static CPersistContext* g_ctx = 0;
CCharString::CCharString(const char* text, long length) { ++g_strCtor; m_pData = (void*)text; CHECK(length == -1); }
CCharString::~CCharString() { ++g_strDtor; }
void CBase::v00() { }
CBase::~CBase() { }
CStringParserCommentList::~CStringParserCommentList() { }
CStringParser::CStringParser(const CCharString& text, void (*cb)(CCharString*), const CCharString& filename, long baseLineOffset)
    : Filename(0, -1), TempCharString(0, -1)
{ ++g_parserCtor; g_parsedText = &text; CHECK(cb == 0 && baseLineOffset == 0 && strcmp((const char*)filename.m_pData, "") == 0); }
bool CStringParser::SkipPastNextItemIfIs(const CCharString& item) { ++g_skipCalls; g_skipItem = (const char*)item.m_pData; return g_hasComma; }
void __fastcall TransferIn(CPersistContext* context, CStringParser* parser, long* value) { ++g_transfers; CHECK(context == g_ctx); *value = g_values[g_nextValue++]; }
void CAnimComponentBase::v00() { } void CAnimComponentBase::v01() { } void CAnimComponentBase::v02() { } void CAnimComponentBase::v03() { }
void CAnimComponentBase::v04() { } void CAnimComponentBase::v05() { } void CAnimComponentBase::v06() { } void CAnimComponentBase::v07() { }
void CAnimComponentBase::Reset() { ++g_resets; }
static void Run(CAnimComponentCombatComboChain& c, bool comma, long a, long b)
{
    g_strCtor = g_strDtor = g_resets = g_parserCtor = g_skipCalls = g_transfers = 0; g_nextValue = 0;
    g_values[0] = a; g_values[1] = b; g_hasComma = comma; g_skipItem = 0;
    c.ComboStage = 111; c.ComboID = 222;
    CCharString text("5,7", -1);
    g_strCtor = 0;
    bool r = c.Initialise(*g_ctx, text);
    CHECK(r == true && g_resets == 1 && g_parserCtor == 1 && g_parsedText == &text);
    CHECK(g_skipCalls == 1 && g_skipItem && strcmp(g_skipItem, ",") == 0);
    CHECK(g_transfers == (comma ? 2 : 1));
    CHECK(g_strCtor == 4 && g_strDtor == 4);   // "" and "," plus the parser's two members, all destroyed before the return
}
int main()
{
    static CAnimComponentCombatComboChain c;
    g_ctx = (CPersistContext*)0x1234;
    Run(c, false, 9, 0);   CHECK(c.ComboID == 9 && c.ComboStage == 111);       // "<id>"
    Run(c, true, 3, 12);   CHECK(c.ComboStage == 3 && c.ComboID == 12);        // "<stage>,<id>"
    Run(c, true, 4, -1);   CHECK(c.ComboID == 4 && c.ComboStage == 111);       // second -1: the first is the id
    if (fails == 0) printf("005dc650_TEST PASS\n");
    return fails;
}
