// NScript::CV_RockTrollFirstEncounterScript::CM_RTFERockTrollTrigger::Init() -- empty: retail is a lone `ret` (the entity overrides Init with nothing)
// Leaf: ?Init@CM_RTFERockTrollTrigger@CV_RockTrollFirstEncounterScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EC42A0, 1 bytes, thiscall, plain `ret`.
// Identity: reg 00ec3e6c "M_RTFERockTrollTrigger" in 00ec3d40; Alloc 00ec41f0; vtable 012f166c slot 2; PDB ?Init@CM_RTFERockTrollTrigger@CV_RockTrollFirstEncounterScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00ec42a0 (wave3/labels_alloc.tsv); PDB ?Init@CM_RTFERockTrollTrigger@CV_RockTrollFirstEncounterScript@NScript@@EAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Hook() on a `struct T` pad model (00/ec/gapscan_sub00ec42a0_00ec42a0.cpp, `?Hook@T@@QAEXXZ`); same bytes.
// Class model: the landed Alloc row 0x00EC41F0 (rebuild/src/compiled/00/ec/NScriptCVRockTrollFirstEncounterScriptCMRTFERockTrollTrigger_Alloc_00ec41f0.cpp) as wave 33 D1 emits from it: its user-declared
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
class CV_RockTrollFirstEncounterScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CM_RTFERockTrollTrigger : public CEntityScriptBase {
    public:
        CV_RockTrollFirstEncounterScript* ParentClass;  // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18

        CM_RTFERockTrollTrigger(CV_RockTrollFirstEncounterScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_RockTrollFirstEncounterScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
    virtual void Init();                                 // PDB ?Init@CM_RTFERockTrollTrigger@CV_RockTrollFirstEncounterScript@NScript@@EAEXXZ (slot 2)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

void CV_RockTrollFirstEncounterScript::CM_RTFERockTrollTrigger::Init()
{
}
}
