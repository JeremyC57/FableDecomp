// NScript::CQ_TraderConflictEvilScript::CTC_BanditFollower::Init() -- resets the entity state: HitWarning = false, SetAgainstHero = false
// Leaf: ?Init@CTC_BanditFollower@CQ_TraderConflictEvilScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DF8040, 9 bytes, thiscall, plain `ret`.
// Identity: reg 00df60d0 "TC_BanditFollower" in 00df6010; Alloc 00df7f90; vtable 012de92c slot 2; PDB ?Init@CTC_BanditFollower@CQ_TraderConflictEvilScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00df8040 (wave3/labels_alloc.tsv); PDB ?Init@CTC_BanditFollower@CQ_TraderConflictEvilScript@NScript@@EAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Clear() on a `struct T` pad model (00/df/gapscan_sub00df8040_00df8040.cpp, `?Clear@T@@QAEXXZ`); same bytes.
// Class model: the landed Alloc row 0x00DF7F90 (rebuild/src/compiled/00/df/NScriptCQTraderConflictEvilScriptCTCBanditFollower_Alloc_00df7f90.cpp) as wave 33 D1 emits from it: its user-declared
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
class CQ_TraderConflictEvilScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CTC_BanditFollower : public CEntityScriptBase {
    public:
        CQ_TraderConflictEvilScript* ParentClass;       // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        bool HitWarning;                                // +0x1C
        bool SetAgainstHero;                            // +0x1D
        CTC_BanditFollower(CQ_TraderConflictEvilScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_TraderConflictEvilScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
    virtual void Init();                                 // PDB ?Init@CTC_BanditFollower@CQ_TraderConflictEvilScript@NScript@@EAEXXZ (slot 2)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

void CQ_TraderConflictEvilScript::CTC_BanditFollower::Init()
{
    HitWarning = false;
    SetAgainstHero = false;
}
}
