// NScript::CV_PicnicAreaAfterWaspBossScript::CPAAWB_Guard::Main() -- empty: retail is a lone `ret` (the entity overrides Main with nothing)
// Leaf: ?Main@CPAAWB_Guard@CV_PicnicAreaAfterWaspBossScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EC15A0, 1 bytes, thiscall, plain `ret`.
// Identity: reg 00ec138e "PAAWB_Guard" in 00ec12e0; Alloc 00ec14e0; vtable 012f12a0 slot 1; PDB ?Main@CPAAWB_Guard@CV_PicnicAreaAfterWaspBossScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00ec15a0 (wave3/labels_alloc.tsv); PDB ?Main@CPAAWB_Guard@CV_PicnicAreaAfterWaspBossScript@NScript@@EAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Hook() on a `struct T` pad model (00/ec/gapscan_sub00ec15a0_00ec15a0.cpp, `?Hook@T@@QAEXXZ`); same bytes.
// Class model: the landed Alloc row 0x00EC14E0 (rebuild/src/compiled/00/ec/NScriptCVPicnicAreaAfterWaspBossScriptCPAAWBGuard_Alloc_00ec14e0.cpp) as wave 33 D1 emits from it: its user-declared
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
class CQ_SunnyvaleMasterData;
class CV_PicnicAreaAfterWaspBossScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CPAAWB_Guard : public CEntityScriptBase {
    public:
        CV_PicnicAreaAfterWaspBossScript* ParentClass;  // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18

        CPAAWB_Guard(CV_PicnicAreaAfterWaspBossScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_PicnicAreaAfterWaspBossScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

void CV_PicnicAreaAfterWaspBossScript::CPAAWB_Guard::Main()
{
}
}
