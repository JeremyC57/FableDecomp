// NScript::CV_HauntedHouseScript::OnPersist(CPersistContext& context) -- saves/loads CellarDoorOpen and HeroBeenToChest by name (default false)
// Leaf: ?OnPersist@CV_HauntedHouseScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E93E60, 67 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<bool> 0x004045C0 (??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z, already bound).
// Identity (naming wave 35, family op): CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00e96dd0 at 00cd8b2f with name "V_HauntedHouse"; Alloc news + stores script vtable 012ecb94; PDB ?Alloc@CV_HauntedHouseScript@NScript@@SIPAVCScriptBase@@; script vtable slot 4 (slot 1 pushes "Main" 0x012C2F94); PDB OnPersist; refs=1; was _unattributed::PersistTransfer2_00e93e60.
//   ?OnPersist@CV_HauntedHouseScript@NScript@@UAEXAAVCPersistContext@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00e93e60::PersistTransfer2_00e93e60(CPersistContext* context) -- persists the "CellarDoorOpen" and "HeroBeenToChest" byte flags at +0x50/+0x51 Retail: Fable.exe (TLC, VC7.1) function @ 0x00E93E60, oracle ... (catalog stand-in _unattributed).
// Class: the class model of the landed constructor row 0x00E93D50 (rebuild/src/compiled/00/e9/NScriptCVHauntedHouseScript_CVHauntedHouseScript_00e93d50.cpp); the PDB OnPersist declaration added; its bool CellarDoorOpen, bool HeroBeenToChest; members CellarDoorOpen +0x50, HeroBeenToChest +0x51 added at the offsets of the landed byte-matching source (the persisted keys are their names). CPersistContext: engine/CPersistContext.h. Only this member is graded.
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
class CV_HauntedHouseScript : public CLevelScriptBase<NScript::CQ_SunnyvaleMasterData> {
public:
    CV_HauntedHouseScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual void RegisterMain();
    virtual void Main();
public:
    unsigned char _pad048[0x50 - 0x48];   // members not touched here
    bool CellarDoorOpen;   // +0x50 (persisted under this name; offset from the landed byte-matching OnPersist)
    bool HeroBeenToChest;   // +0x51 (persisted under this name; offset from the landed byte-matching OnPersist)
public:
    virtual void OnPersist(CPersistContext& context);   // PDB ?OnPersist@CV_HauntedHouseScript@NScript@@UAEXAAVCPersistContext@@@Z (slot 4)
};
CV_HauntedHouseScript::CV_HauntedHouseScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<NScript::CQ_SunnyvaleMasterData>(data, game)
{
}
}

void NScript::CV_HauntedHouseScript::OnPersist(CPersistContext& context)
{
    context.Transfer("CellarDoorOpen", CellarDoorOpen, false);
    context.Transfer("HeroBeenToChest", HeroBeenToChest, false);
}
