// NScript::CV_ChapelOfEvilScript::CCOE_AcolyteEvil::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CCOE_AcolyteEvil@CV_ChapelOfEvilScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E6A5A0, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00e6943c "COE_AcolyteEvil" in 00e693f0; Alloc 00e6a500; vtable 012e8468 slot 3; PDB ?GetParentScript@CCOE_AcolyteEvil@CV_ChapelOfEvilScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00e6a5a0.
//   ?GetParentScript@CCOE_AcolyteEvil@CV_ChapelOfEvilScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: the class model of the landed Alloc row 0x00E6A500 (rebuild/src/compiled/00/e6/NScriptCVChapelOfEvilScriptCCOEAcolyteEvil_Alloc_00e6a500.cpp); the Alloc body (if any) is not part of this graded body.
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
class CV_ChapelOfEvilScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CCOE_AcolyteEvil : public CEntityScriptBase {
    public:
        CV_ChapelOfEvilScript* ParentClass;             // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        CTimer AcolytePrayerTimer;                      // +0x1C (Ego_r)
        CCOE_AcolyteEvil(CV_ChapelOfEvilScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CCOE_AcolyteEvil();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_ChapelOfEvilScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CV_ChapelOfEvilScript::CCOE_AcolyteEvil::Alloc(CV_ChapelOfEvilScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CCOE_AcolyteEvil(parent, data, me));
}
}

CScriptBase* NScript::CV_ChapelOfEvilScript::CCOE_AcolyteEvil::GetParentScript() const
{
    return ParentClass;
}
