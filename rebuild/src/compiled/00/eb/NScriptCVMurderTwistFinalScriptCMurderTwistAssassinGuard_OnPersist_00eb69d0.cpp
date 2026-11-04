// NScript::CV_MurderTwistFinalScript::CMurderTwistAssassinGuard::OnPersist(CPersistContext& context) -- saves/loads HasBeenSpokenTo by name (default false)
// Leaf: ?OnPersist@CMurderTwistAssassinGuard@CV_MurderTwistFinalScript@NScript@@EAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB69D0, 33 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<bool> 0x004045C0 (??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z, already bound).
// Identity (naming wave 35, family op): reg 00eaeb95 "MurderTwistAssassinGuard" in 00eaea20; Alloc 00eb5450; vtable 012f036c slot 4; PDB ?OnPersist@CMurderTwistAssassinGuard@CV_MurderTwistFinalScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was _unattributed::PersistTransfer_HasBeenSpokenTo_00eb69d0.
//   ?OnPersist@CMurderTwistAssassinGuard@CV_MurderTwistFinalScript@NScript@@EAEXAAVCPersistContext@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00eb69d0::PersistTransfer_HasBeenSpokenTo_00eb69d0(CPersistContext* context) -- persists the "HasBeenSpokenTo" flag at +0x30 Retail: Fable.exe (TLC, VC7.1) function @ 0x00EB69D0, oracle length 33 bytes. (catalog stand-in _unattributed).
// Class: the class model of the landed Alloc row 0x00EB5450 (rebuild/src/compiled/00/eb/NScriptCVMurderTwistFinalScriptCMurderTwistAssassinGuard_Alloc_00eb5450.cpp); the PDB OnPersist declaration added; its bool HasBeenSpokenTo. CPersistContext: engine/CPersistContext.h. Only this member is graded.
#include "engine/CPersistContext.h"
class CGameScriptInterfaceBase;
class CScriptDataBase;

// 0x00CDEE00 is CCountedPointer<T>::DeleteData (engine/CCountedPointer.h; reloc_bindings binds both spellings)

#include "engine/CCountedPointer.h"


// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CScriptGameResourceObjectScriptedThingBase {      // 16 bytes (Ego_r): vptr, +4, PImp +8
public:
    virtual ~CScriptGameResourceObjectScriptedThingBase();
    unsigned char _base04[4];
    CCountedPointer<CScriptGameResourceObjectScriptedThingBase> PImp;   // +0x08
    CScriptGameResourceObjectScriptedThingBase();       // 0x007E72A0
};

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_MurderTwistFinalScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CMurderTwistAssassinGuard : public CEntityScriptBase {
    public:
        CV_MurderTwistFinalScript* ParentClass;         // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        CScriptGameResourceObjectScriptedThingBase MyEntityHandle;// +0x1C (Ego_r)
        unsigned long NextLine;                         // +0x2C
        bool HasBeenSpokenTo;                           // +0x30
        bool PreemptiveAttackInProgress;                // +0x31
        CMurderTwistAssassinGuard(CV_MurderTwistFinalScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CMurderTwistAssassinGuard();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_MurderTwistFinalScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
private:
    virtual void OnPersist(CPersistContext& context);   // PDB ?OnPersist@CMurderTwistAssassinGuard@CV_MurderTwistFinalScript@NScript@@EAEXAAVCPersistContext@@@Z (slot 4)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CV_MurderTwistFinalScript::CMurderTwistAssassinGuard::Alloc(CV_MurderTwistFinalScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CMurderTwistAssassinGuard(parent, data, me));
}
}

void NScript::CV_MurderTwistFinalScript::CMurderTwistAssassinGuard::OnPersist(CPersistContext& context)
{
    context.Transfer("HasBeenSpokenTo", HasBeenSpokenTo, false);
}
