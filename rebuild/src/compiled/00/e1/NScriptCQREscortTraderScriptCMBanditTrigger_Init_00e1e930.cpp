// NScript::CQR_EscortTraderScript::CM_BanditTrigger::Init() -- resets the entity state: BanditTriggered = false
// Leaf: ?Init@CM_BanditTrigger@CQR_EscortTraderScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E1E930, 5 bytes, thiscall, plain `ret`.
// Identity: reg 00e1e159 "M_BanditTrigger" in 00e1e110; Alloc 00e1e880; vtable 012e1538 slot 2; PDB ?Init@CM_BanditTrigger@CQR_EscortTraderScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00e1e930 (wave3/labels_alloc.tsv); PDB ?Init@CM_BanditTrigger@CQR_EscortTraderScript@NScript@@EAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Clear() on a `struct T` pad model (00/e1/gapscan_sub00e1e930_00e1e930.cpp, `?Clear@T@@QAEXXZ`); same bytes.
// Class model: the landed Alloc row 0x00E1E880 (rebuild/src/compiled/00/e1/NScriptCQREscortTraderScriptCMBanditTrigger_Alloc_00e1e880.cpp) as wave 33 D1 emits from it: its user-declared
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
class CQR_EscortTraderScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CM_BanditTrigger : public CEntityScriptBase {
    public:
        CQR_EscortTraderScript* ParentClass;            // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        bool BanditTriggered;                           // +0x1C
        CM_BanditTrigger(CQR_EscortTraderScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQR_EscortTraderScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
    virtual void Init();                                 // PDB ?Init@CM_BanditTrigger@CQR_EscortTraderScript@NScript@@EAEXXZ (slot 2)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

void CQR_EscortTraderScript::CM_BanditTrigger::Init()
{
    BanditTriggered = false;
}
}
