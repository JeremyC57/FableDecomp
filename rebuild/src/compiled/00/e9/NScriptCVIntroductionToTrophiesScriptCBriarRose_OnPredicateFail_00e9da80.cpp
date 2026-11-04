// NScript::CV_IntroductionToTrophiesScript::CBriarRose::OnPredicateFail() -- empty override (the entity does nothing when its predicate fails)
// Leaf: ?OnPredicateFail@CBriarRose@CV_IntroductionToTrophiesScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E9DA80, 1 bytes: ret.
// Identity (naming wave 35, family ent): reg 00e9c4fc "BriarRose" in 00e9c4b0; Alloc 00e9d9d0; vtable 012ed67c slot 5; PDB ?OnPredicateFail@CBriarRose@CV_IntroductionToTrophiesScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00e9da80.
//   ?OnPredicateFail@CBriarRose@CV_IntroductionToTrophiesScript@NScript@@EAEXXZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Empty member hook. __fastcall this=ecx. (catalog stand-in _gapscan, Leaf none).
// Class: the class model of the landed Alloc row 0x00E9D9D0 (rebuild/src/compiled/00/e9/NScriptCVIntroductionToTrophiesScriptCBriarRose_Alloc_00e9d9d0.cpp); the PDB member declaration added; the Alloc body (if any) is not part of this graded body.
class CGameScriptInterfaceBase;
class CScriptDataBase;

// 0x00CDEE00 is CCountedPointer<T>::DeleteData (engine/CCountedPointer.h; reloc_bindings binds both spellings)

#include "engine/CCountedPointer.h"


// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_IntroductionToTrophiesScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CBriarRose : public CEntityScriptBase {
    public:
        CV_IntroductionToTrophiesScript* ParentClass;   // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        CTimer AnimTimer;                               // +0x1C (Ego_r)
        CBriarRose(CV_IntroductionToTrophiesScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CBriarRose();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_IntroductionToTrophiesScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
private:
    virtual void OnPredicateFail();   // PDB ?OnPredicateFail@CBriarRose@CV_IntroductionToTrophiesScript@NScript@@EAEXXZ (slot 5)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CV_IntroductionToTrophiesScript::CBriarRose::Alloc(CV_IntroductionToTrophiesScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CBriarRose(parent, data, me));
}
}

void NScript::CV_IntroductionToTrophiesScript::CBriarRose::OnPredicateFail()
{
}
