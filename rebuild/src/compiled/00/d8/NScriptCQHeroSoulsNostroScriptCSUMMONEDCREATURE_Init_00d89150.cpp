// NScript::CQ_HeroSoulsNostroScript::CSUMMONED_CREATURE::Init() -- counts the entity in: ++ParentClass->UndeadAlive (CQ_HeroSoulsNostroScript +0x50)
// Leaf: ?Init@CSUMMONED_CREATURE@CQ_HeroSoulsNostroScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D89150, 7 bytes, thiscall, plain `ret`.
// Identity: reg 00d87eb7 "SUMMONED_CREATURE" in 00d87cb0; Alloc 00d890a0; vtable 012d4f34 slot 2; PDB ?Init@CSUMMONED_CREATURE@CQ_HeroSoulsNostroScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00d89150 (wave3/labels_alloc.tsv); PDB ?Init@CSUMMONED_CREATURE@CQ_HeroSoulsNostroScript@NScript@@EAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Bump() on a `struct T` pad model (00/d8/gapscan_sub00d89150_00d89150.cpp, `?Bump@T@@QAEXXZ`); same bytes.
// Class model: the landed Alloc row 0x00D890A0 (rebuild/src/compiled/00/d8/NScriptCQHeroSoulsNostroScriptCSUMMONEDCREATURE_Alloc_00d890a0.cpp) as wave 33 D1 emits from it: its user-declared
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
class CQ_HeroSoulsNostroScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    unsigned char _members48[0x50 - 0x48];    // +0x48 (the script's members before UndeadAlive; not modelled here)
    long UndeadAlive;                         // +0x50 (Ego_r.pdb CQ_HeroSoulsNostroScript, retail = Ego_r - 0x14 less the STLport container deltas)
    class CSUMMONED_CREATURE : public CEntityScriptBase {
    public:
        CQ_HeroSoulsNostroScript* ParentClass;          // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18

        CSUMMONED_CREATURE(CQ_HeroSoulsNostroScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_HeroSoulsNostroScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
    virtual void Init();                                 // PDB ?Init@CSUMMONED_CREATURE@CQ_HeroSoulsNostroScript@NScript@@EAEXXZ (slot 2)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

void CQ_HeroSoulsNostroScript::CSUMMONED_CREATURE::Init()
{
    ++ParentClass->UndeadAlive;
}
}
