// NScript::CQ_HeroSoulsBriarScript::CBriarRose::Init() -- resets the entity state: FirstSpeakAndWaitDone = false
// Leaf: ?Init@CBriarRose@CQ_HeroSoulsBriarScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D816E0, 5 bytes, thiscall, plain `ret`.
// Identity: reg 00d80e8c "BriarRose" in 00d80e40; Alloc 00d81630; vtable 012d42f8 slot 2; PDB ?Init@CBriarRose@CQ_HeroSoulsBriarScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00d816e0 (wave3/labels_alloc.tsv); PDB ?Init@CBriarRose@CQ_HeroSoulsBriarScript@NScript@@EAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Clear() on a `struct T` pad model (00/d8/gapscan_sub00d816e0_00d816e0.cpp, `?Clear@T@@QAEXXZ`); same bytes.
// Class model: the landed Alloc row 0x00D81630 (rebuild/src/compiled/00/d8/NScriptCQHeroSoulsBriarScriptCBriarRose_Alloc_00d81630.cpp) as wave 33 D1 emits from it: its user-declared
//   destructor dropped (the entity destructor is implicit), Init declared private virtual as the PDB spells it; the Alloc body is not part of this TU.
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
class CQ_HeroSoulsBriarScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CBriarRose : public CEntityScriptBase {
    public:
        CQ_HeroSoulsBriarScript* ParentClass;           // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        bool FirstSpeakAndWaitDone;                     // +0x1C
        CBriarRose(CQ_HeroSoulsBriarScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_HeroSoulsBriarScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
    virtual void Init();                                 // PDB ?Init@CBriarRose@CQ_HeroSoulsBriarScript@NScript@@EAEXXZ (slot 2)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

void CQ_HeroSoulsBriarScript::CBriarRose::Init()
{
    FirstSpeakAndWaitDone = false;
}
}
