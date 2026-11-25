// CScriptBase::LoadGameState(const CCharString&) -- restore the script's own data and its persisted entity scripts from the START_/END_ marked game-state text
// Leaf: ?LoadGameState@CScriptBase@@QAEXABVCCharString@@@Z
// Retail 0x00CB8690, 400 bytes, ret 4 (the reader of the landed SaveGameState 0x00CB8820). Callees: CCharString(const
// char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CStringParser ctor 0x00404720 and its implicit inline dtor
// (~_List_base 0x004046B0, two ~CCharString, CBaseClass::~CBaseClass 0x0099A2E0), CStringParser::NextItemIs 0x009B9BA0,
// ReadAsStringUntilString 0x009BA330 (by value), CScriptBase::DoScriptDataPersist 0x00CB7CB0 (loading),
// NScript::CRuntimeScriptedEntityPersist::LoadFromGameStateScript 0x00CB8440 on the member at +0x30.
// Literals: "" 0x0122D70E, START_SCRIPT_DATA 0x012C16C8, END_SCRIPT_DATA 0x012C16B8, START_ENTITY_SCRIPTS 0x012C16A0,
// END_ENTITY_SCRIPTS 0x012C168C.
// Shape: the parser model of DoScriptDataPersist; each marker is a NAMED CCharString inside an inline helper (lea/push,
// destroyed straight after its call); the section text is a by-value result of an inline ReadUntilText whose end
// marker is likewise a named local; the closing marker test's result is ignored.
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CStringParserCommentBoundaries;
// NF245 (2026-11-22, the 0x004046B0 family; nf243 ruling 2 (a)): the row-local std::allocator / std::list stand-ins replaced by the
//   shared STLport list (declaration-only _List_base ctor / dtor): the comment list's destructor call
//   binds ??1?$_List_base@VCStringParserCommentBoundaries@@..@std@@QAE@XZ (0x004046B0; list's own ~list is the 5-byte jmp 0x00404710).
#define FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE
#include "stlport/fable_containers.h"
class CStringParser : public CBaseClass { public:                // retail 0x28 bytes
    char*                    String;               // +0x04
    unsigned long            BaseLineOffset;       // +0x08
    CCharString              Filename;             // +0x0c
    CCharString              TempCharString;       // +0x10
    std::list<CStringParserCommentBoundaries> Comments;             // +0x14
    void (*ErrorCallback)(CCharString*);           // +0x18
    long                     CursorPos[3];         // +0x1c CCursorPos
    CStringParser(const CCharString& text, void (__fastcall* errorCallback)(const CCharString&), const CCharString& filename,
                  long baseLineOffset);            // 0x00404720
    bool SkipPastString(const CCharString& item);              // 0x009B9BA0 NF35: Ego_r ?SkipPastString@CStringParser@@QAE_NABVCCharString@@@Z (catalog CStringParser::SkipPastString)
    bool SkipPastNextItemIfIs(const CCharString& item);    // 0x009B9E00
    CCharString ReadAsString();                            // 0x009BA6F0
    CCharString ReadAsStringUntilString(const CCharString& end);   // 0x009BA330
    CCharString ReadUntilText(const char* text)
    {
        CCharString end(text, -1);
        return ReadAsStringUntilString(end);
    }
    bool NextItemIsText(const char* text)
    {
        CCharString item(text, -1);
        return SkipPastString(item);
    }
    void SkipPastText(const char* text)
    {
        CCharString item(text, -1);
        SkipPastNextItemIfIs(item);
    }
};
// Literal (NF420, 2026-11-25): made-up data symbol(s) replaced by retail's pooled literal (/GF-merged ??_C@) at each use site:
//   g_startScriptData (table ?g_startScriptData@@3QBDB -> 0x012C16C8) -> "START_SCRIPT_DATA"
//   g_endScriptData (table ?g_endScriptData@@3QBDB -> 0x012C16B8) -> "END_SCRIPT_DATA"
//   g_startEntityScripts (table ?g_startEntityScripts@@3QBDB -> 0x012C16A0) -> "START_ENTITY_SCRIPTS"
//   g_endEntityScripts (table ?g_endEntityScripts@@3QBDB -> 0x012C168C) -> "END_ENTITY_SCRIPTS"
namespace NScript {
struct CRuntimeScriptedEntityPersist {
    unsigned char _body[0x14];
    void LoadFromGameStateScript(const CCharString& text);   // 0x00CB8440
};
}
class CScriptBase {
public:
    virtual void v00();
protected:
    void DoScriptDataPersist(CCharString& data, bool saving);   // 0x00CB7CB0 (protected in both PDBs: IAE)
public:
    void LoadGameState(const CCharString& state);
private:
    unsigned char _pad04[0x30 - 0x04];
    NScript::CRuntimeScriptedEntityPersist RuntimeScriptedEntityPersist;   // +0x30 (retail)
};

void CScriptBase::LoadGameState(const CCharString& state)
{
    CStringParser parser(state, 0, CCharString("", -1), 0);
    if (parser.NextItemIsText("START_SCRIPT_DATA")) {
        CCharString data = parser.ReadUntilText("END_SCRIPT_DATA");
        DoScriptDataPersist(data, false);
        parser.NextItemIsText("END_SCRIPT_DATA");
    }
    if (parser.NextItemIsText("START_ENTITY_SCRIPTS")) {
        CCharString data = parser.ReadUntilText("END_ENTITY_SCRIPTS");
        RuntimeScriptedEntityPersist.LoadFromGameStateScript(data);
        parser.NextItemIsText("END_ENTITY_SCRIPTS");
    }
}