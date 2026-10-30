// CScriptBase::DoScriptDataPersist(CCharString& data, bool saving) -- persist the script's own data to or from its text form through a CPersistContext
// Leaf: DoScriptDataPersist
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB7CB0, length 171 bytes (`ret 8`).
// PDB: ?DoScriptDataPersist@CScriptBase@@IAEXAAVCCharString@@_N@Z.
// Callees: CPersistContext::CPersistContext(CCharString&) @ 0x009BAE10 (saving), CCharString::CCharString(const char*, long)
// @ 0x0099EBF0 and ~CCharString @ 0x0099EAE0 (the "" file name), CStringParser::CStringParser(const CCharString&,
// callback, const CCharString&, long) @ 0x00404720, CPersistContext::CPersistContext(CStringParser*, callback, bool,
// CDefinitionManager*, CSymbolMap*, CCharString*) @ 0x009BADD0 (loading), the virtual CScriptBase::OnPersist(CPersistContext&)
// (slot 4, +0x10), and the inlined ~CStringParser: ~list @ 0x004046B0, two ~CCharString, CBaseClass::~CBaseClass @ 0x0099A2E0.
// Shape: two scoped arms (the save context and the parser share the frame base); the loading arm is the landed
// CGameTimeManager::Load 0x006BBBA0 form with the "" temporary homed in the dead `saving` slot; CPersistContext has a
// trivial destructor, so neither context is destroyed.
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
struct CStringParserCommentList {                  // retail std::list<CStringParserCommentBoundaries>: sentinel only
    void* m_pHead;
    ~CStringParserCommentList();                   // 0x004046B0
};
class CStringParser : public CBaseClass { public:                // retail 0x28 bytes
    char*                    String;               // +0x04
    unsigned long            BaseLineOffset;       // +0x08
    CCharString              Filename;             // +0x0c
    CCharString              TempCharString;       // +0x10
    CStringParserCommentList Comments;             // +0x14
    void (*ErrorCallback)(CCharString*);           // +0x18
    long                     CursorPos[3];         // +0x1c CCursorPos
    CStringParser(const CCharString& text, void (*errorCallback)(CCharString*), const CCharString& filename,
                  long baseLineOffset);            // 0x00404720
};
class CDefinitionManager;
class CSymbolMap;
class CPersistContext { public:                           // retail 0x2c bytes, trivial destructor
    unsigned char _pad00[0x2c];
    CPersistContext(CCharString& output);          // 0x009BAE10 (saving: writes into the string)
    CPersistContext(CStringParser* parser, void (*errorCallback)(CCharString*), bool reportErrors,
                    CDefinitionManager* definitions, CSymbolMap* symbols, CCharString* sourceText);   // 0x009BADD0
};
extern const char g_emptyString_0122d70e[];       // 0x0122D70E ("")
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"

void CScriptBase::DoScriptDataPersist(CCharString& data, bool saving)
{
    if (saving) {
        CPersistContext context(data);
        OnPersist(context);
    }
    else {
        CStringParser parser(data, 0, CCharString(g_emptyString_0122d70e, -1), 0);
        CPersistContext context(&parser, 0, true, 0, 0, &data);
        OnPersist(context);
    }
}