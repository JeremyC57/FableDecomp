// NScript::CQ_HeroSoulsGuildmasterScript::CGuardInWoods::Init() -- counts the entity in: ++ParentClass->GuardInWoodsCount (CQ_HeroSoulsGuildmasterScript +0x50)
// Leaf: ?Init@CGuardInWoods@CQ_HeroSoulsGuildmasterScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D86440, 7 bytes, thiscall, plain `ret`.
// Identity: reg 00d84eff "GuardInWoods" in 00d84d60; Alloc 00d86390; vtable 012d4bb8 slot 2; PDB ?Init@CGuardInWoods@CQ_HeroSoulsGuildmasterScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00d86440 (wave3/labels_alloc.tsv); PDB ?Init@CGuardInWoods@CQ_HeroSoulsGuildmasterScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Bump() on a `struct T` pad model (00/d8/gapscan_sub00d86440_00d86440.cpp, `?Bump@T@@QAEXXZ`); same bytes.
// Class model: the landed Alloc row 0x00D86390 (rebuild/src/compiled/00/d8/NScriptCQHeroSoulsGuildmasterScriptCGuardInWoods_Alloc_00d86390.cpp) as wave 33 D1 emits from it: its user-declared
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
class CQ_HeroSoulsGuildmasterScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    unsigned char _members48[0x50 - 0x48];    // +0x48 (the script's members before GuardInWoodsCount; not modelled here)
    long GuardInWoodsCount;                   // +0x50 (Ego_r.pdb CQ_HeroSoulsGuildmasterScript, retail = Ego_r - 0x14 less the STLport container deltas)
    class CGuardInWoods : public CEntityScriptBase {
    public:
        CQ_HeroSoulsGuildmasterScript* ParentClass;     // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18

        CGuardInWoods(CQ_HeroSoulsGuildmasterScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_HeroSoulsGuildmasterScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
    virtual void Init();                                 // PDB ?Init@CGuardInWoods@CQ_HeroSoulsGuildmasterScript@NScript@@EAEXXZ (slot 2)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

void CQ_HeroSoulsGuildmasterScript::CGuardInWoods::Init()
{
    ++ParentClass->GuardInWoodsCount;
}
}
