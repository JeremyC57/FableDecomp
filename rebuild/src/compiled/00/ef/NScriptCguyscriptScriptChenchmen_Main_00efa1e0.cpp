// NScript::Cguy_scriptScript::Chenchmen::Main() -- empty: retail is a lone `ret` (the entity overrides Main with nothing)
// Leaf: ?Main@Chenchmen@Cguy_scriptScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EFA1E0, 1 bytes, thiscall, plain `ret`.
// Identity: reg 00efa057 "henchmen" in 00efa010; Alloc 00efa0b0; vtable 012f69b0 slot 1; PDB ?Main@Chenchmen@Cguy_scriptScript@NScript@@; refs=1; was sub_00efa1e0 (wave3/labels_alloc.tsv); PDB ?Main@Chenchmen@Cguy_scriptScript@NScript@@EAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Hook() on a `struct T` pad model (00/ef/gapscan_sub00efa1e0_00efa1e0.cpp, `?Hook@T@@QAEXXZ`); same bytes.
// Class model: the landed Alloc row 0x00EFA0B0 (rebuild/src/compiled/00/ef/NScriptCguyscriptScriptChenchmen_Alloc_00efa0b0.cpp) as wave 33 D1 emits from it: its user-declared
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

// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"

namespace NScript {
class Cjoss_test_scriptsData;
class Cguy_scriptScript : public CLevelScriptBase<Cjoss_test_scriptsData> {
public:
    class Chenchmen : public CEntityScriptBase {
    public:
        Cguy_scriptScript* ParentClass;                 // +0x14
        Cjoss_test_scriptsData* PMasterData;            // +0x18

        Chenchmen(Cguy_scriptScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (Cjoss_test_scriptsData*)data;
        }
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(Cguy_scriptScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

void Cguy_scriptScript::Chenchmen::Main()
{
}
}
