// NScript::CPersonalScript_GlobalThingsScript::CClickableDoor::Init() -- resets the entity state: IsDoorOpen = false
// Leaf: ?Init@CClickableDoor@CPersonalScript_GlobalThingsScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CDEEC0, 5 bytes, thiscall, plain `ret`.
// Identity: reg 00cde46a "ClickableDoor" in 00cde420; Alloc 00cdee10; vtable 012c32a4 slot 2; PDB ?Init@CClickableDoor@CPersonalScript_GlobalThingsScript@NScript@@; refs=1; was sub_00cdeec0 (wave3/labels_alloc.tsv); PDB ?Init@CClickableDoor@CPersonalScript_GlobalThingsScript@NScript@@EAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Clear() on a `struct T` pad model (00/cd/gapscan_sub00cdeec0_00cdeec0.cpp, `?Clear@T@@QAEXXZ`); same bytes.
// Class model: the landed Alloc row 0x00CDEE10 (rebuild/src/compiled/00/cd/NScriptCPersonalScriptGlobalThingsScriptCClickableDoor_Alloc_00cdee10.cpp) as wave 33 D1 emits from it: its user-declared
//   destructor dropped (the entity destructor is implicit), Init declared private virtual as the PDB spells it; the Alloc body is not part of this TU.
class CGameScriptInterfaceBase;
class CScriptDataBase;

// CCountedPointer<T>: the shared retail model (engine/CCountedPointer.h; 0x00CDEE00 is CCountedPointer<CEntityScriptBase>::DeleteData);
//   the Alloc row's local stand-in (deleter Destroy_00cdee00) is dropped here as wave 33 D4 did.
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
class CPersonalScript_GlobalThingsScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CClickableDoor : public CEntityScriptBase {
    public:
        CPersonalScript_GlobalThingsScript* ParentClass; // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        bool IsDoorOpen;                                // +0x1C
        CClickableDoor(CPersonalScript_GlobalThingsScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CPersonalScript_GlobalThingsScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
    virtual void Init();                                 // PDB ?Init@CClickableDoor@CPersonalScript_GlobalThingsScript@NScript@@EAEXXZ (slot 2)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

void CPersonalScript_GlobalThingsScript::CClickableDoor::Init()
{
    IsDoorOpen = false;
}
}
