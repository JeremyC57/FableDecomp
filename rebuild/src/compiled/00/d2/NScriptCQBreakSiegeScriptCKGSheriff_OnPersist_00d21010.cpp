// NScript::CQ_BreakSiegeScript::CKG_Sheriff::OnPersist(CPersistContext& context) -- saves/loads ToldHeroToGatherGuards by name (default false)
// Leaf: ?OnPersist@CKG_Sheriff@CQ_BreakSiegeScript@NScript@@EAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D21010, 33 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<bool> 0x004045C0 (??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z, already bound).
// Identity (naming wave 35, family op): reg 00d1d3c6 "KG_Sheriff" in 00d1d100; Alloc 00d1df20; vtable 012caf1c slot 4; PDB ?OnPersist@CKG_Sheriff@CQ_BreakSiegeScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was _unattributed::PersistTransfer_ToldHeroToGatherGuards_00d21010.
//   ?OnPersist@CKG_Sheriff@CQ_BreakSiegeScript@NScript@@EAEXAAVCPersistContext@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00d21010::PersistTransfer_ToldHeroToGatherGuards_00d21010(CPersistContext* context) -- persists the "ToldHeroToGatherGuards" flag at +0x1c Retail: Fable.exe (TLC, VC7.1) function @ 0x00D21010, oracle leng... (catalog stand-in _unattributed).
// Class: the class model of the landed Alloc row 0x00D1DF20 (rebuild/src/compiled/00/d1/NScriptCQBreakSiegeScriptCKGSheriff_Alloc_00d1df20.cpp); the PDB OnPersist declaration added; its bool ToldHeroToGatherGuards. CPersistContext: engine/CPersistContext.h. Only this member is graded.
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
class CQ_BreakSiegeScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CKG_Sheriff : public CEntityScriptBase {
    public:
        CQ_BreakSiegeScript* ParentClass;               // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        bool ToldHeroToGatherGuards;                    // +0x1C
        CKG_Sheriff(CQ_BreakSiegeScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CKG_Sheriff();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_BreakSiegeScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
private:
    virtual void OnPersist(CPersistContext& context);   // PDB ?OnPersist@CKG_Sheriff@CQ_BreakSiegeScript@NScript@@EAEXAAVCPersistContext@@@Z (slot 4)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CQ_BreakSiegeScript::CKG_Sheriff::Alloc(CQ_BreakSiegeScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CKG_Sheriff(parent, data, me));
}
}

void NScript::CQ_BreakSiegeScript::CKG_Sheriff::OnPersist(CPersistContext& context)
{
    context.Transfer("ToldHeroToGatherGuards", ToldHeroToGatherGuards, false);
}
