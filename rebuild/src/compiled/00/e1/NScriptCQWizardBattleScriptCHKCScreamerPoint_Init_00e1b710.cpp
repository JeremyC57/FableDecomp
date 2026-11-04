// NScript::CQ_WizardBattleScript::CHKC_ScreamerPoint::Init() -- resets the entity state: CurrentAlpha = 0
// Leaf: ?Init@CHKC_ScreamerPoint@CQ_WizardBattleScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E1B710, 8 bytes, thiscall, plain `ret`.
// Identity: reg 00e1aa4a "HKC_ScreamerPoint" in 00e1a990; Alloc 00e1b660; vtable 012e1078 slot 2; PDB ?Init@CHKC_ScreamerPoint@CQ_WizardBattleScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00e1b710 (wave3/labels_alloc.tsv); PDB ?Init@CHKC_ScreamerPoint@CQ_WizardBattleScript@NScript@@EAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Clear() on a `struct T` pad model (00/e1/gapscan_sub00e1b710_00e1b710.cpp, `?Clear@T@@QAEXXZ`); same bytes.
// Class model: the landed Alloc row 0x00E1B660 (rebuild/src/compiled/00/e1/NScriptCQWizardBattleScriptCHKCScreamerPoint_Alloc_00e1b660.cpp) as wave 33 D1 emits from it: its user-declared
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
class CQ_WizardBattleScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CHKC_ScreamerPoint : public CEntityScriptBase {
    public:
        CQ_WizardBattleScript* ParentClass;             // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        long CurrentAlpha;                              // +0x1C
        CHKC_ScreamerPoint(CQ_WizardBattleScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_WizardBattleScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
    virtual void Init();                                 // PDB ?Init@CHKC_ScreamerPoint@CQ_WizardBattleScript@NScript@@EAEXXZ (slot 2)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

void CQ_WizardBattleScript::CHKC_ScreamerPoint::Init()
{
    CurrentAlpha = 0;
}
}
