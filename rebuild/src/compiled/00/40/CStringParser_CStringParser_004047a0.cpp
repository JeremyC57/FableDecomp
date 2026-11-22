// CStringParser::~CStringParser() -- destroy the comment list, the temporary and file-name strings, then the CBaseClass base
// Leaf: ??1CStringParser@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004047A0, 35 bytes, thiscall: ~_List_base(+0x14), ~CCharString(+0x10), ~CCharString(+0x0C),
//   then `jmp ~CBaseClass`.
// Identity (sweep3; FINDINGS chunk 481 "Callees: 0x4047A0 = ~CStringParser"; the catalog name was already right, the
//   source was a `_Dest_val<allocator<CMacro>,CMacro>` stand-in with invented member types): Ego_r.pdb/FableWin.pdb
//   ??1CStringParser@@UAE@XZ; Ego_r CStringParser (vptr, String, BaseLineOffset, Filename +0x0C, TempCharString +0x10, Comments
//   +0x14 = list<CStringParserCommentBoundaries>, ErrorCallback, CursorPos) is destroyed in exactly this reverse order; the
//   callees are the landed std::_List_base<CStringParserCommentBoundaries>::~_List_base 0x004046B0, CCharString::~CCharString 0x0099EAE0 and
//   CBaseClass::~CBaseClass 0x0099A2E0 (family pass 2: ??1CBaseClass@@UAE@XZ); 41 callers (persist/parse code).
// Shape: the destructor is compiler-generated (implicitly virtual through CBaseClass): retail does not re-store the
//   CStringParser vptr (a user-written `~CStringParser() { }` adds `mov [esi],vftable`, 41 bytes vs 35), and it is a COMDAT
//   placed in an early TU (Ego_r ??1/??_GCStringParser at 0x00403F20/0x00403F80), not in the CStringParser TU (0x009B9xxx).
// Layout: retail 0x28 bytes (STLport list = one node pointer); engine/CStringParser.h is the Ego_r 0x30 layout (header debt).
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class CCharStringData;
class CCharString {
public:
    ~CCharString();                                         // 0x0099EAE0
    CCharStringData* PStringData;
};


// NF245 (2026-11-22, the 0x004046B0 family; nf243 ruling 2 (a)): the row-local std::allocator / std::list stand-ins replaced by the
//   shared STLport list (declaration-only _List_base ctor / dtor) and the element by engine/CStringParser.h's partial mode: the comment list's destructor call
//   binds ??1?$_List_base@VCStringParserCommentBoundaries@@..@std@@QAE@XZ (0x004046B0; list's own ~list is the 5-byte jmp 0x00404710).
#define FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE
#define FABLE_RETAIL_CSTRINGPARSER_COMMENT_BOUNDARIES_ONLY
#include "engine/CStringParser.h"
#include "stlport/fable_containers.h"

struct CCursorPos {
    bool InQuotes;
    long StringPos;
    long Line;
};

class CStringParser : public CBaseClass {               // no user-declared destructor (see Shape)
public:
    char* String;                                           // +0x04
    unsigned long BaseLineOffset;                           // +0x08
    CCharString Filename;                                   // +0x0C
    CCharString TempCharString;                             // +0x10
    std::list<CStringParserCommentBoundaries> Comments;     // +0x14
    void (__cdecl* ErrorCallback)(CCharString*);            // +0x18
    CCursorPos CursorPos;                                   // +0x1C
};

// Emission helper (not a retail symbol): makes the compiler emit the implicit destructor as its own COMDAT, the way the
// first retail TU that needed it did.
void EmitStringParserDestructor(CStringParser* parser)
{
    parser->CStringParser::~CStringParser();
}
