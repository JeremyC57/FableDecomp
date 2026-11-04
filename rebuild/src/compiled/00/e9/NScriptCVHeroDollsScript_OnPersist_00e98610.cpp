// NScript::CV_HeroDollsScript::OnPersist(CPersistContext& context) -- saves/loads KidIntroDone by name (default false)
// Leaf: ?OnPersist@CV_HeroDollsScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E98610, 33 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<bool> 0x004045C0 (??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z, already bound).
// Identity (naming wave 35, family op): outer script vtable 012ece7c slot 4 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00e96f10 (slot 2) registers 1 entity scripts whose PDB Alloc outer is CV_HeroDollsScript; PDB ?OnPersist@CV_HeroDollsScript@NScript@@; refs=1; was _unattributed::PersistTransfer_KidIntroDone_00e98610.
//   ?OnPersist@CV_HeroDollsScript@NScript@@UAEXAAVCPersistContext@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00e98610::PersistTransfer_KidIntroDone_00e98610(CPersistContext* context) -- persists the "KidIntroDone" flag at +0x49 Retail: Fable.exe (TLC, VC7.1) function @ 0x00E98610, oracle length 33 bytes. (catalog stand-in _unattributed).
// Class: the class model of the landed constructor row 0x00E96E30 (rebuild/src/compiled/00/e9/NScriptCVHeroDollsScript_CVHeroDollsScript_00e96e30.cpp); the PDB OnPersist declaration added; its bool KidIntroDone; members KidIntroDone +0x49 added at the offsets of the landed byte-matching source (the persisted keys are their names). CPersistContext: engine/CPersistContext.h. Only this member is graded.
#include "engine/CPersistContext.h"
class CScriptDataBase {
public:
    virtual ~CScriptDataBase();
};
namespace NScript { class CQ_SunnyvaleMasterData; }
class CGameScriptInterfaceBase;
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CV_HeroDollsScript : public CLevelScriptBase<NScript::CQ_SunnyvaleMasterData> {
public:
    CV_HeroDollsScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual void RegisterMain();
    virtual void Main();
public:
    unsigned char _pad048[0x49 - 0x48];   // members not touched here
    bool KidIntroDone;   // +0x49 (persisted under this name; offset from the landed byte-matching OnPersist)
public:
    virtual void OnPersist(CPersistContext& context);   // PDB ?OnPersist@CV_HeroDollsScript@NScript@@UAEXAAVCPersistContext@@@Z (slot 4)
};
CV_HeroDollsScript::CV_HeroDollsScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<NScript::CQ_SunnyvaleMasterData>(data, game)
{
}
}

void NScript::CV_HeroDollsScript::OnPersist(CPersistContext& context)
{
    context.Transfer("KidIntroDone", KidIntroDone, false);
}
