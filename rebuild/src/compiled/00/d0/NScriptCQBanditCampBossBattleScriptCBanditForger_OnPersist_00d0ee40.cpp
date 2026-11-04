// NScript::CQ_BanditCampBossBattleScript::CBanditForger::OnPersist(CPersistContext& context) -- saves/loads GivenPass by name (default false)
// Leaf: ?OnPersist@CBanditForger@CQ_BanditCampBossBattleScript@NScript@@EAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D0EE40, 33 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<bool> 0x004045C0 (??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z, already bound).
// Identity (naming wave 35, family op): reg 00d03d96 "BanditForger" in 00d038e0; Alloc 00d0c2c0; vtable 012c9700 slot 4; PDB ?OnPersist@CBanditForger@CQ_BanditCampBossBattleScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was _unattributed::PersistTransfer_GivenPass_00d0ee40.
//   ?OnPersist@CBanditForger@CQ_BanditCampBossBattleScript@NScript@@EAEXAAVCPersistContext@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00d0ee40::PersistTransfer_GivenPass_00d0ee40(CPersistContext* context) -- persists the "GivenPass" flag at +0x1c Retail: Fable.exe (TLC, VC7.1) function @ 0x00D0EE40, oracle length 33 bytes. (catalog stand-in _unattributed).
// Class: the class model of the landed Alloc row 0x00D0C2C0 (rebuild/src/compiled/00/d0/NScriptCQBanditCampBossBattleScriptCBanditForger_Alloc_00d0c2c0.cpp); the PDB OnPersist declaration added; its bool GivenPass. CPersistContext: engine/CPersistContext.h. Only this member is graded.
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
class CQ_BanditCampBossBattleScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CBanditForger : public CEntityScriptBase {
    public:
        CQ_BanditCampBossBattleScript* ParentClass;     // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        bool GivenPass;                                 // +0x1C
        CBanditForger(CQ_BanditCampBossBattleScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CBanditForger();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_BanditCampBossBattleScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
private:
    virtual void OnPersist(CPersistContext& context);   // PDB ?OnPersist@CBanditForger@CQ_BanditCampBossBattleScript@NScript@@EAEXAAVCPersistContext@@@Z (slot 4)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CQ_BanditCampBossBattleScript::CBanditForger::Alloc(CQ_BanditCampBossBattleScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CBanditForger(parent, data, me));
}
}

void NScript::CQ_BanditCampBossBattleScript::CBanditForger::OnPersist(CPersistContext& context)
{
    context.Transfer("GivenPass", GivenPass, false);
}
