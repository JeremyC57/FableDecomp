// NScript::CQ_PrisonEscapeRescueMotherScript::CMotherTorture::OnPersist(CPersistContext& context) -- saves/loads BrainState by name (default 0L)
// Leaf: ?OnPersist@CMotherTorture@CQ_PrisonEscapeRescueMotherScript@NScript@@EAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DD94F0, 35 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<long> 0x00410BE0 (??$Transfer@J@CPersistContext@@QAEXPBDAAJABJ@Z, already bound).
// Identity (naming wave 35, family op): reg 00dd2f52 "MotherTorture" in 00dd2ce0; Alloc 00dd68a0; vtable 012dc108 slot 4; PDB ?OnPersist@CMotherTorture@CQ_PrisonEscapeRescueMotherScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was _unattributed::PersistTransfer_BrainState_00dd94f0.
//   ?OnPersist@CMotherTorture@CQ_PrisonEscapeRescueMotherScript@NScript@@EAEXAAVCPersistContext@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00dd94f0::PersistTransfer_BrainState_00dd94f0(CPersistContext* context) -- persists the BrainState dword at +0x1c with a zero default Retail: Fable.exe (TLC, VC7.1) function @ 0x00DD94F0, oracle length 35... (catalog stand-in _unattributed).
// Class: the class model of the landed Alloc row 0x00DD68A0 (rebuild/src/compiled/00/dd/NScriptCQPrisonEscapeRescueMotherScriptCMotherTorture_Alloc_00dd68a0.cpp); the PDB OnPersist declaration added; its long BrainState. CPersistContext: engine/CPersistContext.h. Only this member is graded.
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
    class CMotherTorture : public CEntityScriptBase {
    public:
        CQ_PrisonEscapeRescueMotherScript* ParentClass; // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        long BrainState;                                // +0x1C
        CMotherTorture(CQ_PrisonEscapeRescueMotherScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CMotherTorture();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_PrisonEscapeRescueMotherScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
private:
    virtual void OnPersist(CPersistContext& context);   // PDB ?OnPersist@CMotherTorture@CQ_PrisonEscapeRescueMotherScript@NScript@@EAEXAAVCPersistContext@@@Z (slot 4)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CQ_PrisonEscapeRescueMotherScript::CMotherTorture::Alloc(CQ_PrisonEscapeRescueMotherScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CMotherTorture(parent, data, me));
}
}

void NScript::CQ_PrisonEscapeRescueMotherScript::CMotherTorture::OnPersist(CPersistContext& context)
{
    context.Transfer("BrainState", BrainState, 0L);
}
