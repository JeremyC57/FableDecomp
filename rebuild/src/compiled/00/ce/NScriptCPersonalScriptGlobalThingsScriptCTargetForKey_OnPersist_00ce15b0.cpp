// NScript::CPersonalScript_GlobalThingsScript::CTargetForKey::OnPersist(CPersistContext& context) -- saves/loads TargetForKeyDone by name (default false)
// Leaf: ?OnPersist@CTargetForKey@CPersonalScript_GlobalThingsScript@NScript@@EAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CE15B0, 33 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<bool> 0x004045C0 (??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z, already bound).
// Identity (naming wave 35, family op): reg 00cde865 "TargetForKey" in 00cde420; Alloc 00cdfd40; vtable 012c3418 slot 4; PDB ?OnPersist@CTargetForKey@CPersonalScript_GlobalThingsScript@NScript@@; refs=1; was _unattributed::PersistTransfer_TargetForKeyDone_00ce15b0.
//   ?OnPersist@CTargetForKey@CPersonalScript_GlobalThingsScript@NScript@@EAEXAAVCPersistContext@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00ce15b0::PersistTransfer_TargetForKeyDone_00ce15b0(CPersistContext* context) -- persists the "TargetForKeyDone" flag at +0x1c Retail: Fable.exe (TLC, VC7.1) function @ 0x00CE15B0, oracle length 33 bytes. (catalog stand-in _unattributed).
// Class: the class model of the landed Alloc row 0x00CDFD40 (rebuild/src/compiled/00/cd/NScriptCPersonalScriptGlobalThingsScriptCTargetForKey_Alloc_00cdfd40.cpp); the PDB OnPersist declaration added; its bool TargetForKeyDone. CPersistContext: engine/CPersistContext.h. Only this member is graded.
#include "engine/CPersistContext.h"
class CGameScriptInterfaceBase;
class CScriptDataBase;

// CCountedPointer<T> / CCPPointerInfo: the shared retail model (engine/CCountedPointer.h; 0x00CDEE00 is
//   CCountedPointer<CEntityScriptBase>::DeleteData), in place of the Alloc row's local stand-in (deleter Destroy_00cdee00; wave 33 D4).
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
class CPersonalScript_GlobalThingsScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CTargetForKey : public CEntityScriptBase {
    public:
        CPersonalScript_GlobalThingsScript* ParentClass; // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        bool TargetForKeyDone;                          // +0x1C
        CTargetForKey(CPersonalScript_GlobalThingsScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CTargetForKey();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CPersonalScript_GlobalThingsScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
private:
    virtual void OnPersist(CPersistContext& context);   // PDB ?OnPersist@CTargetForKey@CPersonalScript_GlobalThingsScript@NScript@@EAEXAAVCPersistContext@@@Z (slot 4)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CPersonalScript_GlobalThingsScript::CTargetForKey::Alloc(CPersonalScript_GlobalThingsScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CTargetForKey(parent, data, me));
}
}

void NScript::CPersonalScript_GlobalThingsScript::CTargetForKey::OnPersist(CPersistContext& context)
{
    context.Transfer("TargetForKeyDone", TargetForKeyDone, false);
}
