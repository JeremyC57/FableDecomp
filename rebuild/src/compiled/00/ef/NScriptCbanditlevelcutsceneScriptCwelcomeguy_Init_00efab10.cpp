// NScript::Cbandit_level_cutsceneScript::Cwelcome_guy::Init() -- empty: retail is a lone `ret` (the entity overrides Init with nothing)
// Leaf: ?Init@Cwelcome_guy@Cbandit_level_cutsceneScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EFAB10, 1 bytes, thiscall, plain `ret`.
// Identity: reg 00efa5d5 "welcome_guy" in 00efa4f0; Alloc 00efaa60; vtable 012f6a70 slot 2; PDB ?Init@Cwelcome_guy@Cbandit_level_cutsceneScript@NScript@@; refs=1; was sub_00efab10 (wave3/labels_alloc.tsv); PDB ?Init@Cwelcome_guy@Cbandit_level_cutsceneScript@NScript@@EAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Hook() on a `struct T` pad model (00/ef/gapscan_sub00efab10_00efab10.cpp, `?Hook@T@@QAEXXZ`); same bytes.
// Class model: the landed Alloc row 0x00EFAA60 (rebuild/src/compiled/00/ef/NScriptCbanditlevelcutsceneScriptCwelcomeguy_Alloc_00efaa60.cpp) as wave 33 D1 emits from it: its user-declared
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
class Cjoss_test_scriptsData;
class Cbandit_level_cutsceneScript : public CLevelScriptBase<Cjoss_test_scriptsData> {
public:
    class Cwelcome_guy : public CEntityScriptBase {
    public:
        Cbandit_level_cutsceneScript* ParentClass;      // +0x14
        Cjoss_test_scriptsData* PMasterData;            // +0x18

        Cwelcome_guy(Cbandit_level_cutsceneScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (Cjoss_test_scriptsData*)data;
        }
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(Cbandit_level_cutsceneScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
    virtual void Init();                                 // PDB ?Init@Cwelcome_guy@Cbandit_level_cutsceneScript@NScript@@EAEXXZ (slot 2)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

void Cbandit_level_cutsceneScript::Cwelcome_guy::Init()
{
}
}
