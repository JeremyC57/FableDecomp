// NScript::CV_MayorsInvitationScript::CMI_InfoKeeper::OnPersist(CPersistContext& context) -- saves/loads HaveSpoken by name (default false)
// Leaf: ?OnPersist@CMI_InfoKeeper@CV_MayorsInvitationScript@NScript@@EAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB9950, 33 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<bool> 0x004045C0 (??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z, already bound).
// Identity (naming wave 35, family op): reg 00eb6f2e "MI_InfoKeeper" in 00eb6d70; Alloc 00eb8810; vtable 012f0720 slot 4; PDB ?OnPersist@CMI_InfoKeeper@CV_MayorsInvitationScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was _unattributed::PersistTransfer_HaveSpoken_00eb9950.
//   ?OnPersist@CMI_InfoKeeper@CV_MayorsInvitationScript@NScript@@EAEXAAVCPersistContext@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00eb9950::PersistTransfer_HaveSpoken_00eb9950(CPersistContext* context) -- persists the "HaveSpoken" flag at +0x20 Retail: Fable.exe (TLC, VC7.1) function @ 0x00EB9950, oracle length 33 bytes. (catalog stand-in _unattributed).
// Class: the class model of the landed Alloc row 0x00EB8810 (rebuild/src/compiled/00/eb/NScriptCVMayorsInvitationScriptCMIInfoKeeper_Alloc_00eb8810.cpp); the PDB OnPersist declaration added; its bool HaveSpoken. CPersistContext: engine/CPersistContext.h. Only this member is graded.
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
class CV_MayorsInvitationScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CMI_InfoKeeper : public CEntityScriptBase {
    public:
        CV_MayorsInvitationScript* ParentClass;         // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        long ChainNumber;                               // +0x1C
        bool HaveSpoken;                                // +0x20
        CMI_InfoKeeper(CV_MayorsInvitationScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CMI_InfoKeeper();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_MayorsInvitationScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
private:
    virtual void OnPersist(CPersistContext& context);   // PDB ?OnPersist@CMI_InfoKeeper@CV_MayorsInvitationScript@NScript@@EAEXAAVCPersistContext@@@Z (slot 4)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CV_MayorsInvitationScript::CMI_InfoKeeper::Alloc(CV_MayorsInvitationScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CMI_InfoKeeper(parent, data, me));
}
}

void NScript::CV_MayorsInvitationScript::CMI_InfoKeeper::OnPersist(CPersistContext& context)
{
    context.Transfer("HaveSpoken", HaveSpoken, false);
}
