// CTCBase::SerialiseLevelState(bool saving, CCharString& data, bool arg) -- serialise the component's level state into (or out of) a string: skip unsaved components when saving, then run OnSerialise between BeginSerialising and EndSerialising
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00686E10, length 191 bytes (+ int3 padding), `ret 0Ch`.
// Callees: virtual slot 23 (+0x5C, the "should be saved" test; CTCBase's own returns false, landed CTCBase::Serialise
// 0x00686AA0 names it ShouldBeSaved) and slot 1 (+0x04, OnSerialise(CPersistContext&, bool)), CBaseClass ctor 0x0099A2D0 /
// dtor 0x0099A2E0, CCharString() @ 0x0099E4B0 / ~CCharString @ 0x0099EAE0, malloc @ 0x00BFEA0E (the comment list's head node),
// ?BeginSerialising@CTCBase@@AAE_N_NAAVCCharString@@AAVCStringParser@@AAVCPersistContext@@@Z @ 0x00686B00,
// ?EndSerialising@CTCBase@@AAEX_NAAVCCharString@@@Z @ 0x006869E0, the comment list's destructor @ 0x004046B0.
// Layout: engine/CTCBase.h (vptr word, ParentThing +4, flags +8) -- modelled locally because the two virtuals are called;
// the parser is the retail 0x28-byte CStringParser (vtable 0x0122D874, landed copy ctor 0x0046381B: one-word comment list
// at +0x14), the persist context the 0x2C-byte PDB CPersistContext (built by BeginSerialising, no constructor here).
// Shape: the parser's inline default constructor (CBaseClass, vtable, the two strings, the self-linked list head, then the
// body assignment String = 0) and its compiler-generated destructor (no vtable store: list, strings, ~CBaseClass).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CCharString {
public:
    void* PStringData;
    CCharString();                                 // 0x0099E4B0
    ~CCharString();                                // 0x0099EAE0
};
// NF245 (2026-11-22, the 0x004046B0 family; nf243 ruling 2 (a)): the row-local std::allocator / std::list stand-ins replaced by the
//   shared STLport list (the visible out-of-line _List_base dtor) and the element by engine/CStringParser.h's partial mode: the comment list's destructor call
//   binds ??1?$_List_base@VCStringParserCommentBoundaries@@..@std@@QAE@XZ (0x004046B0; list's own ~list is the 5-byte jmp 0x00404710).
#define FABLE_RETAIL_STLP_LIST_BASE_DTOR_VISIBLE_NOINLINE
#define FABLE_RETAIL_CSTRINGPARSER_COMMENT_BOUNDARIES_ONLY
#include "engine/CStringParser.h"
#include "stlport/fable_containers.h"
struct CCursorPos {
    bool          InQuotes;                        // +0x0
    unsigned long StringPos;                       // +0x4
    unsigned long Line;                            // +0x8
};
typedef void (*TStringParserErrorCallback)(const char*);
class CStringParser : public CBaseClass {          // vtable 0x0122D874
public:
    char*                      String;             // +0x04
    unsigned long              BaseLineOffset;     // +0x08
    CCharString                Filename;           // +0x0C
    CCharString                TempCharString;     // +0x10
    std::list<CStringParserCommentBoundaries> Comments;           // +0x14
    TStringParserErrorCallback ErrorCallback;      // +0x18
    CCursorPos                 CursorPos;          // +0x1C
    CStringParser() { String = 0; }
};
class CPersistContext { public:                           // Ego_r.pdb, 0x2C bytes (filled by BeginSerialising)
    unsigned char _body[0x2c];
};

class CTCBase {
public:
#define VF(n) virtual void v##n();
    virtual void v00();
    virtual void OnSerialise(CPersistContext& context, bool arg);   // slot 1 (+0x04)
    VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09) VF(10) VF(11) VF(12) VF(13) VF(14) VF(15) VF(16)
    VF(17) VF(18) VF(19) VF(20) VF(21) VF(22)
    virtual bool ShouldBeSaved();                  // slot 23 (+0x5C)
    void*         ParentThing;                     // +0x04
    bool          ToBeRemoved;                     // +0x08
    bool          IsInGlobalUpdateTCs;             // +0x09
    bool BeginSerialising(bool saving, CCharString& data, CStringParser& parser, CPersistContext& context);   // 0x00686B00
    void EndSerialising(bool saving, CCharString& data);                                                       // 0x006869E0
    void SerialiseLevelState(bool saving, CCharString& data, bool arg);
};

void CTCBase::SerialiseLevelState(bool saving, CCharString& data, bool arg)
{
    if (saving && !ShouldBeSaved())
        return;
    CStringParser parser;
    CPersistContext context;
    if (BeginSerialising(saving, data, parser, context)) {
        OnSerialise(context, arg);
        EndSerialising(saving, data);
    }
}