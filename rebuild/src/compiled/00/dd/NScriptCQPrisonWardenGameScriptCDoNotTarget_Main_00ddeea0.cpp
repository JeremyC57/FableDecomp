// NScript::CQ_PrisonWardenGameScript::CDoNotTarget::Main() -- Game->EntitySetTargetable(Me, false) (CGameScriptInterfaceBase slot 469, +0x754, 0x008A05A0)
// Leaf: ?Main@CDoNotTarget@CQ_PrisonWardenGameScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DDEEA0, 20 bytes, thiscall, plain `ret`.
// Identity: reg 00dddfb7 "DoNotTarget" in 00ddde20; Alloc 00ddedf0; vtable 012dcd10 slot 1; PDB ?Main@CDoNotTarget@CQ_PrisonWardenGameScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00ddeea0 (wave3/labels_alloc.tsv); PDB ?Main@CDoNotTarget@CQ_PrisonWardenGameScript@NScript@@EAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Run() on a `struct T` pad model (00/dd/gapscan_sub00ddeea0_00ddeea0.cpp, `?Run@T@@QAEXXZ`); same bytes.
// Class model: the landed Alloc row 0x00DDEDF0 (rebuild/src/compiled/00/dd/NScriptCQPrisonWardenGameScriptCDoNotTarget_Alloc_00ddedf0.cpp) as wave 33 D1 emits from it: its user-declared
//   destructor dropped (the entity destructor is implicit), Main declared private virtual as the PDB spells it; the Alloc body is not part of this TU.
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
class CQ_PrisonWardenGameScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CDoNotTarget : public CEntityScriptBase {
    public:
        CQ_PrisonWardenGameScript* ParentClass;         // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18

        CDoNotTarget(CQ_PrisonWardenGameScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_PrisonWardenGameScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

void CQ_PrisonWardenGameScript::CDoNotTarget::Main()
{
    Game->EntitySetTargetable(Me, false);
}
}
