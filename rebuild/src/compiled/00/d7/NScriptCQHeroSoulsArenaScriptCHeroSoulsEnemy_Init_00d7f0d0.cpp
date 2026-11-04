// NScript::CQ_HeroSoulsArenaScript::CHeroSoulsEnemy::Init() -- counts the entity in: ++ParentClass->CreatureCount (CQ_HeroSoulsArenaScript +0x48)
// Leaf: ?Init@CHeroSoulsEnemy@CQ_HeroSoulsArenaScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D7F0D0, 7 bytes, thiscall, plain `ret`.
// Identity: reg 00d7cb77 "HeroSoulsEnemy" in 00d7c890; Alloc 00d7f020; vtable 012d3ee8 slot 2; PDB ?Init@CHeroSoulsEnemy@CQ_HeroSoulsArenaScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00d7f0d0 (wave3/labels_alloc.tsv); PDB ?Init@CHeroSoulsEnemy@CQ_HeroSoulsArenaScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Bump() on a `struct T` pad model (00/d7/gapscan_sub00d7f0d0_00d7f0d0.cpp, `?Bump@T@@QAEXXZ`); same bytes.
// Class model: the landed Alloc row 0x00D7F020 (rebuild/src/compiled/00/d7/NScriptCQHeroSoulsArenaScriptCHeroSoulsEnemy_Alloc_00d7f020.cpp) as wave 33 D1 emits from it: its user-declared
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
class CQ_HeroSoulsArenaScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    long CreatureCount;                       // +0x48 (Ego_r.pdb CQ_HeroSoulsArenaScript, retail = Ego_r - 0x14 less the STLport container deltas)
    class CHeroSoulsEnemy : public CEntityScriptBase {
    public:
        CQ_HeroSoulsArenaScript* ParentClass;           // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18

        CHeroSoulsEnemy(CQ_HeroSoulsArenaScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_HeroSoulsArenaScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
    virtual void Init();                                 // PDB ?Init@CHeroSoulsEnemy@CQ_HeroSoulsArenaScript@NScript@@EAEXXZ (slot 2)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

void CQ_HeroSoulsArenaScript::CHeroSoulsEnemy::Init()
{
    ++ParentClass->CreatureCount;
}
}
