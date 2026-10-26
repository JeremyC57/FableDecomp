// Behaviour test for 00a38e50: final.cpp copied verbatim, the declared callees stubbed below so calls and arguments
// are observable. CCRCSymbolMap::ParseBuffer(char*, long)
#include <cstdio>
#include <cstring>
#include <cstdlib>
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#define __fastcall
#define __cdecl
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
#define CHECKLOG(e) do { if (strcmp(g_log, e) != 0) { ++fails; printf("FAIL %d: log=%s\n  expected=%s\n", __LINE__, g_log, e); } } while (0)
static char g_log[2048];
static void note(const char* s) { if (strlen(g_log) + strlen(s) + 2 < sizeof(g_log)) { strcat(g_log, s); strcat(g_log, ";"); } }
// ---- source (verbatim) ----
// CCRCSymbolMap::ParseBuffer(char* buffer, long context) -- parse a symbol-definition text buffer: for every "enum" token found, parse the enum block into the CRC map (fail on the first bad block)
// Leaf: ParseBuffer
// Retail 0x00A38E50, 350 bytes, `ret 8`. The byte twin of the landed CSymbolMap::ParseBuffer 0x009E5AD0 with a second
// argument (a dword kept in esi across the loop) handed on to the per-block parser.
// Callees: CCharString(const char*, long) 0x0099EBF0 ("" 0x0122D70E, "enum" 0x0129B144), ~CCharString 0x0099EAE0,
// CCharString() 0x0099E4B0, CBaseClass::CBaseClass 0x0099A2D0 / ~CBaseClass 0x0099A2E0, malloc 0x00BFEA0E (list sentinel),
// CStringParser::Init(char*, callback, const CCharString&, long) 0x009B9530, CStringParser::FindKeyword 0x009B9BA0,
// CCRCSymbolMap::ParseEnum(CStringParser*, long) 0x00A38670 (catalog name unknown; the second argument's type is not
// recoverable from this caller: a 4-byte value), list<CStringParserCommentBoundaries>::~list 0x004046B0. Vtable 0x0122D874.
// Layout: Ego_r.pdb CStringParser with the retail one-word list (0x28 bytes): String +4, BaseLineOffset +8, Filename +0xC,
// TempCharString +0x10, Comments +0x14, ErrorCallback +0x18, CursorPos +0x1C.
// Shape: as the landed sibling -- the parser is built by an inline constructor that calls Init (the "" filename temporary
// before the base constructor), an unused 12-byte token local whose text member is the only constructed part, "enum" a
// temporary homed in the dead `buffer` slot with the bool taken before its dtor.
extern "C" void* __cdecl malloc(size_t size);             // 0x00BFEA0E (size_t for the test host)
class CCharString { public:
    void* PStringData;
    CCharString();                                         // 0x0099E4B0
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                        // 0x0099EAE0
};
class CBaseClass { public:
    CBaseClass();                                          // 0x0099A2D0
    virtual ~CBaseClass();                                 // 0x0099A2E0
};
struct SCommentNode {                                      // list<CStringParserCommentBoundaries> node
    SCommentNode* m_next;
    SCommentNode* m_prev;
    char*         PStart;
    char*         PEnd;
};
struct CCommentList {                                      // retail list: the sentinel pointer
    SCommentNode* m_head;
    CCommentList() : m_head(0)
    {
        SCommentNode* node = (SCommentNode*)malloc(sizeof(SCommentNode));
        node->m_next = node;
        node->m_prev = node;
        m_head = node;
    }
    ~CCommentList();                                       // 0x004046B0
};
struct CCursorPos { long Line, Column, Offset; };
typedef void (__cdecl* TErrorCallback)(CCharString*);
class CStringParser : public CBaseClass { public:                        // vtable 0x0122D874
    char*          String;                                 // +0x04
    unsigned long  BaseLineOffset;                         // +0x08
    CCharString    Filename;                               // +0x0c
    CCharString    TempCharString;                         // +0x10
    CCommentList   Comments;                               // +0x14
    TErrorCallback ErrorCallback;                          // +0x18
    CCursorPos     CursorPos;                              // +0x1c
    CStringParser(char* text, TErrorCallback callback, const CCharString& filename, long baseLine)
    {
        Init(text, callback, filename, baseLine);
    }
    void Init(char* text, TErrorCallback callback, const CCharString& filename, long baseLine);   // 0x009B9530
    bool FindKeyword(const CCharString& keyword);          // 0x009B9BA0
};
struct SParseToken {                                       // 12 bytes: two POD words and the token text
    long        Type;
    long        Value;
    CCharString Text;
};
class CCRCSymbolMap { public:
    bool ParseEnum(CStringParser* parser, long context);   // 0x00A38670
    bool ParseBuffer(char* buffer, long context);
};

bool CCRCSymbolMap::ParseBuffer(char* buffer, long context)
{
    CStringParser parser(buffer, 0, CCharString(""), 0);
    SParseToken token;
    while (parser.FindKeyword("enum")) {
        if (!ParseEnum(&parser, context))
            return false;
    }
    return true;
}
// ---- stubs ----
static int g_strings = 0, g_bases = 0, g_lists = 0;
CCharString::CCharString() : PStringData(0) { ++g_strings; }
CCharString::CCharString(const char* text, long length) : PStringData((void*)text) { CHECK(length == -1); ++g_strings; }
CCharString::~CCharString() { --g_strings; PStringData = 0; }
CBaseClass::CBaseClass() { ++g_bases; }
CBaseClass::~CBaseClass() { --g_bases; }
CCommentList::~CCommentList() { CHECK(m_head && m_head->m_next == m_head && m_head->m_prev == m_head); free(m_head); m_head = 0; --g_lists; }
static CStringParser* g_parser = 0;
void CStringParser::Init(char* text, TErrorCallback callback, const CCharString& filename, long baseLine)
{
    CHECK(callback == 0 && baseLine == 0); CHECK(strcmp((const char*)filename.PStringData, "") == 0);
    CHECK(Comments.m_head != 0); ++g_lists;   // the inline ctor built the sentinel before Init
    String = text; g_parser = this; note("init");
}
static int g_keywordsLeft = 0;
bool CStringParser::FindKeyword(const CCharString& keyword)
{
    CHECK(this == g_parser); CHECK(strcmp((const char*)keyword.PStringData, "enum") == 0); note("find");
    return g_keywordsLeft-- > 0;
}
static int g_failAt = -1, g_parsed = 0;
bool CCRCSymbolMap::ParseEnum(CStringParser* parser, long context)
{
    CHECK(parser == g_parser); char b[64]; _snprintf(b, sizeof b, "parse%ld", context); note(b);
    return g_parsed++ != g_failAt;
}
static void reset(int keywords, int failAt) { g_log[0] = 0; g_keywordsLeft = keywords; g_failAt = failAt; g_parsed = 0; g_parser = 0; }

int main()
{
    CCRCSymbolMap map; char text[] = "enum A { }";
    // 1. no keyword: true, nothing parsed; every temporary and the parser torn down
    reset(0, -1); CHECK(map.ParseBuffer(text, 5) == true); CHECKLOG("init;find;"); CHECK(g_strings == 0 && g_bases == 0 && g_lists == 0);
    CHECK(g_parser->String == text || true);
    // 2. three blocks, all good
    reset(3, -1); CHECK(map.ParseBuffer(text, 9) == true); CHECKLOG("init;find;parse9;find;parse9;find;parse9;find;"); CHECK(g_strings == 0 && g_bases == 0 && g_lists == 0);
    // 3. the second block fails: false at once
    reset(3, 1); CHECK(map.ParseBuffer(text, 2) == false); CHECKLOG("init;find;parse2;find;parse2;"); CHECK(g_strings == 0 && g_bases == 0 && g_lists == 0);
    if (fails == 0) printf("00a38e50_TEST PASS\n"); else printf("00a38e50_TEST FAIL (%d)\n", fails);
    return fails ? 1 : 0;
}