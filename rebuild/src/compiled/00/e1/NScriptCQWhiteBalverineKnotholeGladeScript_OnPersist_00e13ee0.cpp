// NScript::CQ_WhiteBalverineKnotholeGladeScript::OnPersist(CPersistContext& context) -- saves/loads QuestStartScreenShown by name (default false)
// Leaf: ?OnPersist@CQ_WhiteBalverineKnotholeGladeScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E13EE0, 33 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<bool> 0x004045C0 (??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z, already bound).
// Identity (naming wave 35, family op): outer script vtable 012e0768 slot 4 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00e13f10 (slot 2) registers 6 entity scripts whose PDB Alloc outer is CQ_WhiteBalverineKnotholeGladeScript; PDB ?OnPersist@CQ_WhiteBalverineKnotholeGladeScript@NScript@@; refs=1; was _unattributed::PersistTransfer_QuestStartScreenShown_00e13ee0.
//   ?OnPersist@CQ_WhiteBalverineKnotholeGladeScript@NScript@@UAEXAAVCPersistContext@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00e13ee0::PersistTransfer_QuestStartScreenShown_00e13ee0(CPersistContext* context) -- persists the "QuestStartScreenShown" flag at +0x60 Retail: Fable.exe (TLC, VC7.1) function @ 0x00E13EE0, oracle length... (catalog stand-in _unattributed).
// Class: the class model of the landed constructor row 0x00E13C20 (rebuild/src/compiled/00/e1/NScriptCQWhiteBalverineKnotholeGladeScript_CQWhiteBalverineKnotholeGladeScript_00e13c20.cpp); the PDB OnPersist declaration added; its bool QuestStartScreenShown; members QuestStartScreenShown +0x60 added at the offsets of the landed byte-matching source (the persisted keys are their names). CPersistContext: engine/CPersistContext.h. Only this member is graded.
#include "engine/CPersistContext.h"
class CScriptDataBase;
class CGameScriptInterfaceBase;
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing() { }
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_WhiteBalverineKnotholeGladeScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_WhiteBalverineKnotholeGladeScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual ~CQ_WhiteBalverineKnotholeGladeScript();
    unsigned char _members[0x54 - 0x48];          // POD members up to the script thing
    CScriptThing  KG_Chief;                            // +0x54
    bool QuestStartScreenShown;   // +0x60 (persisted under this name; offset from the landed byte-matching OnPersist)
public:
    virtual void OnPersist(CPersistContext& context);   // PDB ?OnPersist@CQ_WhiteBalverineKnotholeGladeScript@NScript@@UAEXAAVCPersistContext@@@Z (slot 4)
};
CQ_WhiteBalverineKnotholeGladeScript::CQ_WhiteBalverineKnotholeGladeScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}

void NScript::CQ_WhiteBalverineKnotholeGladeScript::OnPersist(CPersistContext& context)
{
    context.Transfer("QuestStartScreenShown", QuestStartScreenShown, false);
}
