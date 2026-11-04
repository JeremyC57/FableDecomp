// NScript::CQ_EndGameFocalSitesScript::CBattleGuard::Init() -- Game->EntitySetOpinionReactionsEnabled(Me, false) (CGameScriptInterfaceBase slot 594, +0x948, 0x0088F810)
// Leaf: ?Init@CBattleGuard@CQ_EndGameFocalSitesScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D2C4E0, 20 bytes, thiscall, plain `ret`.
// Identity: reg 00d2b006 "BattleGuard" in 00d2aa40; Alloc 00d2c430; vtable 012cc320 slot 2; PDB ?Init@CBattleGuard@CQ_EndGameFocalSitesScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00d2c4e0 (wave3/labels_alloc.tsv); PDB ?Init@CBattleGuard@CQ_EndGameFocalSitesScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Run() on a `struct T` pad model (00/d2/gapscan_sub00d2c4e0_00d2c4e0.cpp, `?Run@T@@QAEXXZ`); same bytes.
// Class model: the landed Alloc row 0x00D2C430 (rebuild/src/compiled/00/d2/NScriptCQEndGameFocalSitesScriptCBattleGuard_Alloc_00d2c430.cpp) as wave 33 D1 emits from it: its user-declared
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
// CGameScriptInterfaceBase: the retail interface (vtable slots from engine/CGameScriptInterfaceBase.h).
#include "engine/CGameScriptInterfaceBase.h"

// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_EndGameFocalSitesScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CBattleGuard : public CEntityScriptBase {
    public:
        CQ_EndGameFocalSitesScript* ParentClass;        // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18

        CBattleGuard(CQ_EndGameFocalSitesScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_EndGameFocalSitesScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
    virtual void Init();                                 // PDB ?Init@CBattleGuard@CQ_EndGameFocalSitesScript@NScript@@EAEXXZ (slot 2)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

void CQ_EndGameFocalSitesScript::CBattleGuard::Init()
{
    Game->EntitySetOpinionReactionsEnabled(Me, false);
}
}
