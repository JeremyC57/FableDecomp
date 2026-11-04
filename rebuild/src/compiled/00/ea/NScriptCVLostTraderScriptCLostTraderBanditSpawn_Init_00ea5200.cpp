// NScript::CV_LostTraderScript::CLostTraderBanditSpawn::Init() -- empty: retail is a lone `ret` (the entity overrides Init with nothing)
// Leaf: ?Init@CLostTraderBanditSpawn@CV_LostTraderScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EA5200, 1 bytes, thiscall, plain `ret`.
// Identity: reg 00ea3313 "LostTraderBanditSpawn" in 00ea2e80; Alloc 00ea5150; vtable 012ee050 slot 2; PDB ?Init@CLostTraderBanditSpawn@CV_LostTraderScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00ea5200 (wave3/labels_alloc.tsv); PDB ?Init@CLostTraderBanditSpawn@CV_LostTraderScript@NScript@@EAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Hook() on a `struct T` pad model (00/ea/gapscan_sub00ea5200_00ea5200.cpp, `?Hook@T@@QAEXXZ`); same bytes.
// Class model: the landed Alloc row 0x00EA5150 (rebuild/src/compiled/00/ea/NScriptCVLostTraderScriptCLostTraderBanditSpawn_Alloc_00ea5150.cpp) as wave 33 D1 emits from it: its user-declared
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
class CV_LostTraderScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CLostTraderBanditSpawn : public CEntityScriptBase {
    public:
        CV_LostTraderScript* ParentClass;               // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18

        CLostTraderBanditSpawn(CV_LostTraderScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_LostTraderScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
    virtual void Init();                                 // PDB ?Init@CLostTraderBanditSpawn@CV_LostTraderScript@NScript@@EAEXXZ (slot 2)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

void CV_LostTraderScript::CLostTraderBanditSpawn::Init()
{
}
}
