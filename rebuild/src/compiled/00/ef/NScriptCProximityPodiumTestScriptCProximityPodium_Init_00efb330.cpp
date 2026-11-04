// NScript::CProximityPodiumTestScript::CProximityPodium::Init() -- empty: retail is a lone `ret` (the entity overrides Init with nothing)
// Leaf: ?Init@CProximityPodium@CProximityPodiumTestScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EFB330, 1 bytes, thiscall, plain `ret`.
// Identity: reg 00efb227 "ProximityPodium" in 00efb1e0; Alloc 00efb280; vtable 012f6b20 slot 2; PDB ?Init@CProximityPodium@CProximityPodiumTestScript@NScript@@; refs=1; was sub_00efb330 (wave3/labels_alloc.tsv); PDB ?Init@CProximityPodium@CProximityPodiumTestScript@NScript@@EAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Hook() on a `struct T` pad model (00/ef/gapscan_sub00efb330_00efb330.cpp, `?Hook@T@@QAEXXZ`); same bytes.
// Class model: the landed Alloc row 0x00EFB280 (rebuild/src/compiled/00/ef/NScriptCProximityPodiumTestScriptCProximityPodium_Alloc_00efb280.cpp) as wave 33 D1 emits from it: its user-declared
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
class CDanyalTestScriptsData;
class CProximityPodiumTestScript : public CLevelScriptBase<CDanyalTestScriptsData> {
public:
    class CProximityPodium : public CEntityScriptBase {
    public:
        CProximityPodiumTestScript* ParentClass;        // +0x14
        CDanyalTestScriptsData* PMasterData;            // +0x18

        CProximityPodium(CProximityPodiumTestScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CDanyalTestScriptsData*)data;
        }
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CProximityPodiumTestScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
    virtual void Init();                                 // PDB ?Init@CProximityPodium@CProximityPodiumTestScript@NScript@@EAEXXZ (slot 2)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

void CProximityPodiumTestScript::CProximityPodium::Init()
{
}
}
