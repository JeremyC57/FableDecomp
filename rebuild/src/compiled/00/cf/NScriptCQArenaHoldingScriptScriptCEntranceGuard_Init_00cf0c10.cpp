// NScript::CQ_ArenaHoldingScriptScript::CEntranceGuard::Init() -- resets the entity state: IntroDone = false, DoneArena = false
// Leaf: ?Init@CEntranceGuard@CQ_ArenaHoldingScriptScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CF0C10, 9 bytes, thiscall, plain `ret`.
// Identity: reg 00cf08a2 "EntranceGuard" in 00cf0790; Alloc 00cf0b60; vtable 012c6158 slot 2; PDB ?Init@CEntranceGuard@CQ_ArenaHoldingScriptScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00cf0c10 (wave3/labels_alloc.tsv); PDB ?Init@CEntranceGuard@CQ_ArenaHoldingScriptScript@NScript@@EAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Clear() on a `struct T` pad model (00/cf/gapscan_sub00cf0c10_00cf0c10.cpp, `?Clear@T@@QAEXXZ`); same bytes.
// Class model: the landed Alloc row 0x00CF0B60 (rebuild/src/compiled/00/cf/NScriptCQArenaHoldingScriptScriptCEntranceGuard_Alloc_00cf0b60.cpp) as wave 33 D1 emits from it: its user-declared
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
class CQ_ArenaHoldingScriptScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CEntranceGuard : public CEntityScriptBase {
    public:
        CQ_ArenaHoldingScriptScript* ParentClass;       // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        bool IntroDone;                                 // +0x1C
        bool DoneArena;                                 // +0x1D
        CEntranceGuard(CQ_ArenaHoldingScriptScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_ArenaHoldingScriptScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
    virtual void Init();                                 // PDB ?Init@CEntranceGuard@CQ_ArenaHoldingScriptScript@NScript@@EAEXXZ (slot 2)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

void CQ_ArenaHoldingScriptScript::CEntranceGuard::Init()
{
    IntroDone = false;
    DoneArena = false;
}
}
