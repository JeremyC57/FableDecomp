// NScript::CPersonalScript_GlobalThingsScript::CBSGateGuard::OnPredicateFail() -- empty override (the entity does nothing when its predicate fails)
// Leaf: ?OnPredicateFail@CBSGateGuard@CPersonalScript_GlobalThingsScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CDFAD0, 1 bytes: ret.
// Identity (naming wave 35, family ent): reg 00cde7fb "BSGateGuard" in 00cde420; Alloc 00cdfa10; vtable 012c33fc slot 5; PDB ?OnPredicateFail@CBSGateGuard@CPersonalScript_GlobalThingsScript@NScript@@; refs=1; was sub_00cdfad0.
//   ?OnPredicateFail@CBSGateGuard@CPersonalScript_GlobalThingsScript@NScript@@EAEXXZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Empty member hook. __fastcall this=ecx. (catalog stand-in _gapscan, Leaf none).
// Class: the class model of the landed Alloc row 0x00CDFA10 (rebuild/src/compiled/00/cd/NScriptCPersonalScriptGlobalThingsScriptCBSGateGuard_Alloc_00cdfa10.cpp); the PDB member declaration added; its local CCountedPointer stand-in replaced by engine/CCountedPointer.h (wave 33 D4); the Alloc body (if any) is not part of this graded body.
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
    class CBSGateGuard : public CEntityScriptBase {
    public:
        CPersonalScript_GlobalThingsScript* ParentClass; // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18

        CBSGateGuard(CPersonalScript_GlobalThingsScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CBSGateGuard();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CPersonalScript_GlobalThingsScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
private:
    virtual void OnPredicateFail();   // PDB ?OnPredicateFail@CBSGateGuard@CPersonalScript_GlobalThingsScript@NScript@@EAEXXZ (slot 5)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CPersonalScript_GlobalThingsScript::CBSGateGuard::Alloc(CPersonalScript_GlobalThingsScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CBSGateGuard(parent, data, me));
}
}

void NScript::CPersonalScript_GlobalThingsScript::CBSGateGuard::OnPredicateFail()
{
}
