// NScript::CQ_PrisonEscapeRescueMotherScript::CPrisoner::OnPersist(CPersistContext& context) -- saves/loads ReleasedAndFollowing by name (default false)
// Leaf: ?OnPersist@CPrisoner@CQ_PrisonEscapeRescueMotherScript@NScript@@EAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DD94C0, 33 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<bool> 0x004045C0 (??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z, already bound).
// Identity (naming wave 35, family op): reg 00dd2e8a "Prisoner" in 00dd2ce0; Alloc 00dd5940; vtable 012dc02c slot 4; PDB ?OnPersist@CPrisoner@CQ_PrisonEscapeRescueMotherScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was _unattributed::PersistTransfer_ReleasedAndFollowing_00dd94c0.
//   ?OnPersist@CPrisoner@CQ_PrisonEscapeRescueMotherScript@NScript@@EAEXAAVCPersistContext@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00dd94c0::PersistTransfer_ReleasedAndFollowing_00dd94c0(CPersistContext* context) -- persists the "ReleasedAndFollowing" flag at +0x1c Retail: Fable.exe (TLC, VC7.1) function @ 0x00DD94C0, oracle length 3... (catalog stand-in _unattributed).
// Class: the class model of the landed Alloc row 0x00DD5940 (rebuild/src/compiled/00/dd/NScriptCQPrisonEscapeRescueMotherScriptCPrisoner_Alloc_00dd5940.cpp); the PDB OnPersist declaration added; its bool ReleasedAndFollowing. CPersistContext: engine/CPersistContext.h. Only this member is graded.
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
class CQ_PrisonEscapeRescueMotherScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CPrisoner : public CEntityScriptBase {
    public:
        CQ_PrisonEscapeRescueMotherScript* ParentClass; // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        bool ReleasedAndFollowing;                      // +0x1C
        CPrisoner(CQ_PrisonEscapeRescueMotherScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CPrisoner();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_PrisonEscapeRescueMotherScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
private:
    virtual void OnPersist(CPersistContext& context);   // PDB ?OnPersist@CPrisoner@CQ_PrisonEscapeRescueMotherScript@NScript@@EAEXAAVCPersistContext@@@Z (slot 4)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CQ_PrisonEscapeRescueMotherScript::CPrisoner::Alloc(CQ_PrisonEscapeRescueMotherScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CPrisoner(parent, data, me));
}
}

void NScript::CQ_PrisonEscapeRescueMotherScript::CPrisoner::OnPersist(CPersistContext& context)
{
    context.Transfer("ReleasedAndFollowing", ReleasedAndFollowing, false);
}
