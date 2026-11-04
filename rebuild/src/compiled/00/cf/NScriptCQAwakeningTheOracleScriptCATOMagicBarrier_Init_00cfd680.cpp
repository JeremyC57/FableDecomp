// NScript::CQ_AwakeningTheOracleScript::CATO_MagicBarrier::Init() -- Game->EntitySetAsDrawable(Me, false) (CGameScriptInterfaceBase slot 511, +0x7fc, 0x0088E9F0)
// Leaf: ?Init@CATO_MagicBarrier@CQ_AwakeningTheOracleScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CFD680, 20 bytes, thiscall, plain `ret`.
// Identity: reg 00cfb3f3 "ATO_MagicBarrier" in 00cfb080; Alloc 00cfd5d0; vtable 012c7d38 slot 2; PDB ?Init@CATO_MagicBarrier@CQ_AwakeningTheOracleScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00cfd680 (wave3/labels_alloc.tsv); PDB ?Init@CATO_MagicBarrier@CQ_AwakeningTheOracleScript@NScript@@EAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Run() on a `struct T` pad model (00/cf/gapscan_sub00cfd680_00cfd680.cpp, `?Run@T@@QAEXXZ`); same bytes.
// Class model: the landed Alloc row 0x00CFD5D0 (rebuild/src/compiled/00/cf/NScriptCQAwakeningTheOracleScriptCATOMagicBarrier_Alloc_00cfd5d0.cpp) as wave 33 D1 emits from it: its user-declared
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
// CGameScriptInterfaceBase: the retail interface (vtable slots from engine/CGameScriptInterfaceBase.h).
#include "engine/CGameScriptInterfaceBase.h"

// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_AwakeningTheOracleScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CATO_MagicBarrier : public CEntityScriptBase {
    public:
        CQ_AwakeningTheOracleScript* ParentClass;       // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18

        CATO_MagicBarrier(CQ_AwakeningTheOracleScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_AwakeningTheOracleScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
    virtual void Init();                                 // PDB ?Init@CATO_MagicBarrier@CQ_AwakeningTheOracleScript@NScript@@EAEXXZ (slot 2)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

void CQ_AwakeningTheOracleScript::CATO_MagicBarrier::Init()
{
    Game->EntitySetAsDrawable(Me, false);
}
}
