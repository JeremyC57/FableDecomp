// CCRCSymbolMap::ParseBuffer(const char* buffer, const CCharString& wantedName) -- parse a symbol-definition text buffer: for every "enum" token found, parse the enum block into the CRC map (fail on the first bad block)
// Leaf: ?ParseBuffer@CCRCSymbolMap@@IAE_NPBDABVCCharString@@@Z
// Naming (naming wave 24, labels_chunk691): Ego_r.pdb / FableWin.pdb ?ParseBuffer@CCRCSymbolMap@@IAE_NPBDABVCCharString@@@Z (protected;
//   the second argument, the 4-byte value kept in esi, is the wanted enum name handed on to ParseEnum) and its callee
//   ?ParseEnum@CCRCSymbolMap@@IAE_NAAVCStringParser@@ABVCCharString@@@Z (the parser by reference). The parser text is const char*
//   (the CStringParser::Init spelling the landed 0x009ADE90 already binds: PBD).
// Retail 0x00A38E50, 350 bytes, `ret 8`. The byte twin of the landed CSymbolMap::ParseBuffer 0x009E5AD0 with a second
// argument (a dword kept in esi across the loop) handed on to the per-block parser.
// Callees: CCharString(const char*, long) 0x0099EBF0 ("" 0x0122D70E, "enum" 0x0129B144), ~CCharString 0x0099EAE0,
// CCharString() 0x0099E4B0, CBaseClass::CBaseClass 0x0099A2D0 / ~CBaseClass 0x0099A2E0, malloc 0x00BFEA0E (list sentinel),
// CStringParser::Init(char*, callback, const CCharString&, long) 0x009B9530, CStringParser::FindKeyword 0x009B9BA0,
// CCRCSymbolMap::ParseEnum(CStringParser*, long) 0x00A38670 (catalog name unknown; the second argument's type is not
// recoverable from this caller: a 4-byte value), std::_List_base<CStringParserCommentBoundaries>::~_List_base 0x004046B0. Vtable 0x0122D874.
// Layout: Ego_r.pdb CStringParser with the retail one-word list (0x28 bytes): String +4, BaseLineOffset +8, Filename +0xC,
// TempCharString +0x10, Comments +0x14, ErrorCallback +0x18, CursorPos +0x1C.
// Shape: as the landed sibling -- the parser is built by an inline constructor that calls Init (the "" filename temporary
// before the base constructor), an unused 12-byte token local whose text member is the only constructed part, "enum" a
// temporary homed in the dead `buffer` slot with the bool taken before its dtor.
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
class CCRCSymbolMap {
protected:
    bool ParseEnum(CStringParser& parser, const CCharString& wantedName);   // 0x00A38670
    bool ParseBuffer(const char* buffer, const CCharString& wantedName);
};

bool CCRCSymbolMap::ParseBuffer(const char* buffer, const CCharString& wantedName)
{
    CStringParser parser(buffer, 0, CCharString(""), 0);
    SParseToken token;
    while (parser.SkipPastString("enum")) {
        if (!ParseEnum(parser, wantedName))
            return false;
    }
    return true;
}