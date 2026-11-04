// NScript::CV_TravellingHeroesScript::CTH_Trigger_Oakvale_Gossip::Init() -- empty: retail is a lone `ret` (the entity overrides Init with nothing)
// Leaf: ?Init@CTH_Trigger_Oakvale_Gossip@CV_TravellingHeroesScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE0670, 1 bytes, thiscall, plain `ret`.
// Identity: reg 00edf155 "TH_Trigger_Oakvale_Gossip" in 00eded10; Alloc 00ee05c0; vtable 012f4304 slot 2; PDB ?Init@CTH_Trigger_Oakvale_Gossip@CV_TravellingHeroesScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00ee0670 (wave3/labels_alloc.tsv); PDB ?Init@CTH_Trigger_Oakvale_Gossip@CV_TravellingHeroesScript@NScript@@EAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Hook() on a `struct T` pad model (00/ee/gapscan_sub00ee0670_00ee0670.cpp, `?Hook@T@@QAEXXZ`); same bytes.
// Class model: the landed Alloc row 0x00EE05C0 (rebuild/src/compiled/00/ee/NScriptCVTravellingHeroesScriptCTHTriggerOakvaleGossip_Alloc_00ee05c0.cpp) as wave 33 D1 emits from it: its user-declared
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
class CV_TravellingHeroesScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CTH_Trigger_Oakvale_Gossip : public CEntityScriptBase {
    public:
        CV_TravellingHeroesScript* ParentClass;         // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18

        CTH_Trigger_Oakvale_Gossip(CV_TravellingHeroesScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_TravellingHeroesScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
    virtual void Init();                                 // PDB ?Init@CTH_Trigger_Oakvale_Gossip@CV_TravellingHeroesScript@NScript@@EAEXXZ (slot 2)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

void CV_TravellingHeroesScript::CTH_Trigger_Oakvale_Gossip::Init()
{
}
}
