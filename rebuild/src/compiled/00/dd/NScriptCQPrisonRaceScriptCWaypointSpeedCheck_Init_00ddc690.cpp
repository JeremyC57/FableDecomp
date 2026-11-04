// NScript::CQ_PrisonRaceScript::CWaypointSpeedCheck::Init() -- empty: retail is a lone `ret` (the entity overrides Init with nothing)
// Leaf: ?Init@CWaypointSpeedCheck@CQ_PrisonRaceScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DDC690, 1 bytes, thiscall, plain `ret`.
// Identity: reg 00ddb0af "WaypointSpeedCheck" in 00ddae40; Alloc 00ddc5e0; vtable 012dca64 slot 2; PDB ?Init@CWaypointSpeedCheck@CQ_PrisonRaceScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00ddc690 (wave3/labels_alloc.tsv); PDB ?Init@CWaypointSpeedCheck@CQ_PrisonRaceScript@NScript@@EAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Hook() on a `struct T` pad model (00/dd/gapscan_sub00ddc690_00ddc690.cpp, `?Hook@T@@QAEXXZ`); same bytes.
// Class model: the landed Alloc row 0x00DDC5E0 (rebuild/src/compiled/00/dd/NScriptCQPrisonRaceScriptCWaypointSpeedCheck_Alloc_00ddc5e0.cpp) as wave 33 D1 emits from it: its user-declared
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
class CQ_PrisonRaceScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CWaypointSpeedCheck : public CEntityScriptBase {
    public:
        CQ_PrisonRaceScript* ParentClass;               // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18

        CWaypointSpeedCheck(CQ_PrisonRaceScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_PrisonRaceScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
    virtual void Init();                                 // PDB ?Init@CWaypointSpeedCheck@CQ_PrisonRaceScript@NScript@@EAEXXZ (slot 2)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

void CQ_PrisonRaceScript::CWaypointSpeedCheck::Init()
{
}
}
