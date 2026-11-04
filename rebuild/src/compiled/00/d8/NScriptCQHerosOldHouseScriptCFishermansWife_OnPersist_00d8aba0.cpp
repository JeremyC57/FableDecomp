// NScript::CQ_HerosOldHouseScript::CFishermansWife::OnPersist(CPersistContext& context) -- saves/loads TalkedTo and LeavingHappy by name (default false)
// Leaf: ?OnPersist@CFishermansWife@CQ_HerosOldHouseScript@NScript@@EAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D8ABA0, 67 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<bool> 0x004045C0 (??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z, already bound).
// Identity (naming wave 35, family op): reg 00d89fa4 "FishermansWife" in 00d89e60; Alloc 00d8a650; vtable 012d50dc slot 4; PDB ?OnPersist@CFishermansWife@CQ_HerosOldHouseScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was _unattributed::PersistTransfer2_00d8aba0.
//   ?OnPersist@CFishermansWife@CQ_HerosOldHouseScript@NScript@@EAEXAAVCPersistContext@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00d8aba0::PersistTransfer2_00d8aba0(CPersistContext* context) -- persists the "TalkedTo" and "LeavingHappy" byte flags at +0x1c/+0x1d Retail: Fable.exe (TLC, VC7.1) function @ 0x00D8ABA0, oracle length 67... (catalog stand-in _unattributed).
// Class: the class model of the landed Alloc row 0x00D8A650 (rebuild/src/compiled/00/d8/NScriptCQHerosOldHouseScriptCFishermansWife_Alloc_00d8a650.cpp); the PDB OnPersist declaration added; its bool TalkedTo, bool LeavingHappy. CPersistContext: engine/CPersistContext.h. Only this member is graded.
#include "engine/CPersistContext.h"
class CGameScriptInterfaceBase;
class CScriptDataBase;

// 0x00CDEE00 is CCountedPointer<T>::DeleteData (engine/CCountedPointer.h; reloc_bindings binds both spellings)

#include "engine/CCountedPointer.h"


// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HerosOldHouseScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CFishermansWife : public CEntityScriptBase {
    public:
        CQ_HerosOldHouseScript* ParentClass;            // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        bool TalkedTo;                                  // +0x1C
        bool LeavingHappy;                              // +0x1D
        float initial_angle;                            // +0x20
        CFishermansWife(CQ_HerosOldHouseScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CFishermansWife();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_HerosOldHouseScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
private:
    virtual void OnPersist(CPersistContext& context);   // PDB ?OnPersist@CFishermansWife@CQ_HerosOldHouseScript@NScript@@EAEXAAVCPersistContext@@@Z (slot 4)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CQ_HerosOldHouseScript::CFishermansWife::Alloc(CQ_HerosOldHouseScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CFishermansWife(parent, data, me));
}
}

void NScript::CQ_HerosOldHouseScript::CFishermansWife::OnPersist(CPersistContext& context)
{
    context.Transfer("TalkedTo", TalkedTo, false);
    context.Transfer("LeavingHappy", LeavingHappy, false);
}
