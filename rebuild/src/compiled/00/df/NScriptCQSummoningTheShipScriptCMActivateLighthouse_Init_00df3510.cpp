// NScript::CQ_SummoningTheShipScript::CM_ActivateLighthouse::Init() -- empty: retail is a lone `ret` (the entity overrides Init with nothing)
// Leaf: ?Init@CM_ActivateLighthouse@CQ_SummoningTheShipScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DF3510, 1 bytes, thiscall, plain `ret`.
// Identity: reg 00df15df "M_ActivateLighthouse" in 00df13a0; Alloc 00df3460; vtable 012de334 slot 2; PDB ?Init@CM_ActivateLighthouse@CQ_SummoningTheShipScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00df3510 (wave3/labels_alloc.tsv); PDB ?Init@CM_ActivateLighthouse@CQ_SummoningTheShipScript@NScript@@EAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Hook() on a `struct T` pad model (00/df/gapscan_sub00df3510_00df3510.cpp, `?Hook@T@@QAEXXZ`); same bytes.
// Class model: the landed Alloc row 0x00DF3460 (rebuild/src/compiled/00/df/NScriptCQSummoningTheShipScriptCMActivateLighthouse_Alloc_00df3460.cpp) as wave 33 D1 emits from it: its user-declared
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
class CQ_SummoningTheShipScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CM_ActivateLighthouse : public CEntityScriptBase {
    public:
        CQ_SummoningTheShipScript* ParentClass;         // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18

        CM_ActivateLighthouse(CQ_SummoningTheShipScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_SummoningTheShipScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
    virtual void Init();                                 // PDB ?Init@CM_ActivateLighthouse@CQ_SummoningTheShipScript@NScript@@EAEXXZ (slot 2)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

void CQ_SummoningTheShipScript::CM_ActivateLighthouse::Init()
{
}
}
