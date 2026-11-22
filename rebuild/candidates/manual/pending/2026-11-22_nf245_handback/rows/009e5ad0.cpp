// CSymbolMap::ParseBuffer(char* buffer) -- parse a symbol-definition text buffer: for every "enum" token found, parse the enum block (fail on the first bad block)
// Leaf: ParseBuffer
// Retail 0x009E5AD0, 352 bytes, `ret 4`.
// Callees: CCharString(const char*, long) 0x0099EBF0 ("" 0x0122D70E, "enum" 0x0129B144), ~CCharString 0x0099EAE0,
// CCharString() 0x0099E4B0, CBaseClass::CBaseClass 0x0099A2D0 / ~CBaseClass 0x0099A2E0, malloc 0x00BFEA0E (list sentinel),
// CStringParser::Init(char*, callback, const CCharString&, long) 0x009B9530, the parser's keyword search 0x009B9BA0
// (bool, const CCharString&; labelled CThingCreatureBase::SerialiseGameState), CSymbolMap::ParseEnum(CStringParser*)
// 0x009E55A0, std::_List_base<CStringParserCommentBoundaries>::~_List_base 0x004046B0. Vtable 0x0122D874 (CStringParser).
// Layout: Ego_r.pdb CStringParser with the retail one-word list (0x28 bytes): String +4, BaseLineOffset +8, Filename +0xC,
// TempCharString +0x10, Comments +0x14, ErrorCallback +0x18, CursorPos +0x1C.
// Shape: the parser is built by an inline constructor that calls Init, so the "" filename temporary is built before the
// parser's base constructor (an explicit `CCharString("")`, whose ctor result stays in esi); an unused 12-byte token local
// follows (its text CCharString at +8 is the only constructed member: retail's frame has two dead words before it); the parser has no user destructor (no vtable reset in
// the inlined teardown); `"enum"` is a temporary homed in the dead `buffer` slot, the bool taken before its dtor.
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E
class CCharString { public:
    void* PStringData;
    CCharString();                                         // 0x0099E4B0
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                        // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct SCommentNode {                                      // list<CStringParserCommentBoundaries> node
    SCommentNode* m_next;
    SCommentNode* m_prev;
    char*         PStart;
    char*         PEnd;
};
// NF245 (2026-11-22, the 0x004046B0 family; nf243 ruling 2 (a)): the row-local std::allocator / std::list stand-ins replaced by the
//   shared STLport list (the visible out-of-line _List_base dtor) and the element by engine/CStringParser.h's partial mode: the comment list's destructor call
//   binds ??1?$_List_base@VCStringParserCommentBoundaries@@..@std@@QAE@XZ (0x004046B0; list's own ~list is the 5-byte jmp 0x00404710).
#define FABLE_RETAIL_STLP_LIST_BASE_DTOR_VISIBLE_NOINLINE
#define FABLE_RETAIL_CSTRINGPARSER_COMMENT_BOUNDARIES_ONLY
#include "engine/CStringParser.h"
#include "stlport/fable_containers.h"
struct CCursorPos { long Line, Column, Offset; };
typedef void (__fastcall* TErrorCallback)(const CCharString&);
class CStringParser : public CBaseClass { public:                        // vtable 0x0122D874
    char*          String;                                 // +0x04
    unsigned long  BaseLineOffset;                         // +0x08
    CCharString    Filename;                               // +0x0c
    CCharString    TempCharString;                         // +0x10
    std::list<CStringParserCommentBoundaries> Comments;                               // +0x14
    TErrorCallback ErrorCallback;                          // +0x18
    CCursorPos     CursorPos;                              // +0x1c
    CStringParser(const char* text, TErrorCallback callback, const CCharString& filename, long baseLine)
    {
        Init(text, callback, filename, baseLine);
    }
    void Init(const char* text, TErrorCallback callback, const CCharString& filename, long baseLine);   // 0x009B9530
    bool SkipPastString(const CCharString& keyword);          // 0x009B9BA0 NF35: Ego_r ?SkipPastString@CStringParser@@QAE_NABVCCharString@@@Z (catalog CStringParser::SkipPastString)
};
struct SParseToken {                                       // 12 bytes: two POD words and the token text
    long        Type;
    long        Value;
    CCharString Text;
};
class CSymbolMap { public:
    bool ParseEnum(CStringParser* parser);                 // 0x009E55A0
    bool ParseBuffer(char* buffer);
};

bool CSymbolMap::ParseBuffer(char* buffer)
{
    CStringParser parser(buffer, 0, CCharString(""), 0);
    SParseToken token;
    while (parser.SkipPastString("enum")) {
        if (!ParseEnum(&parser))
            return false;
    }
    return true;
}