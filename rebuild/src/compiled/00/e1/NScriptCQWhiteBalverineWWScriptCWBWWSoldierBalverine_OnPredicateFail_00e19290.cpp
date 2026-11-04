// NScript::CQ_WhiteBalverineWWScript::CWBWW_SoldierBalverine::OnPredicateFail() -- empty override (the entity does nothing when its predicate fails)
// Leaf: ?OnPredicateFail@CWBWW_SoldierBalverine@CQ_WhiteBalverineWWScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E19290, 1 bytes: ret.
// Identity (naming wave 35, family ent): reg 00e1875d "WBWW_SoldierBalverine" in 00e18630; Alloc 00e191e0; vtable 012e0e4c slot 5; PDB ?OnPredicateFail@CWBWW_SoldierBalverine@CQ_WhiteBalverineWWScript@NScript@@; refs=1; was sub_00e19290.
//   ?OnPredicateFail@CWBWW_SoldierBalverine@CQ_WhiteBalverineWWScript@NScript@@EAEXXZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Empty member hook. __fastcall this=ecx. (catalog stand-in _gapscan, Leaf none).
// Class: the class model of the landed Alloc row 0x00E191E0 (rebuild/src/compiled/00/e1/NScriptCQWhiteBalverineWWScriptCWBWWSoldierBalverine_Alloc_00e191e0.cpp); the PDB member declaration added; the Alloc body (if any) is not part of this graded body.
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
class CQ_WhiteBalverineWWScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CWBWW_SoldierBalverine : public CEntityScriptBase {
    public:
        CQ_WhiteBalverineWWScript* ParentClass;         // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18

        CWBWW_SoldierBalverine(CQ_WhiteBalverineWWScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CWBWW_SoldierBalverine();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_WhiteBalverineWWScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
private:
    virtual void OnPredicateFail();   // PDB ?OnPredicateFail@CWBWW_SoldierBalverine@CQ_WhiteBalverineWWScript@NScript@@EAEXXZ (slot 5)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CQ_WhiteBalverineWWScript::CWBWW_SoldierBalverine::Alloc(CQ_WhiteBalverineWWScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CWBWW_SoldierBalverine(parent, data, me));
}
}

void NScript::CQ_WhiteBalverineWWScript::CWBWW_SoldierBalverine::OnPredicateFail()
{
}
