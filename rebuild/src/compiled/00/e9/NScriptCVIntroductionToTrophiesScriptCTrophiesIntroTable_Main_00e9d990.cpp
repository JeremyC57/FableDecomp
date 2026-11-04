// NScript::CV_IntroductionToTrophiesScript::CTrophiesIntroTable::Main() -- Game->SetTavernTableAvailableForUse(Me, false) (CGameScriptInterfaceBase slot 743, +0xb9c, 0x008A1890)
// Leaf: ?Main@CTrophiesIntroTable@CV_IntroductionToTrophiesScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E9D990, 20 bytes, thiscall, plain `ret`.
// Identity: reg 00e9c627 "TrophiesIntroTable" in 00e9c4b0; Alloc 00e9d8e0; vtable 012ed660 slot 1; PDB ?Main@CTrophiesIntroTable@CV_IntroductionToTrophiesScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00e9d990 (wave3/labels_alloc.tsv); PDB ?Main@CTrophiesIntroTable@CV_IntroductionToTrophiesScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Run() on a `struct T` pad model (00/e9/gapscan_sub00e9d990_00e9d990.cpp, `?Run@T@@QAEXXZ`); same bytes.
// Class model: the landed Alloc row 0x00E9D8E0 (rebuild/src/compiled/00/e9/NScriptCVIntroductionToTrophiesScriptCTrophiesIntroTable_Alloc_00e9d8e0.cpp) as wave 33 D1 emits from it: its user-declared
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
// CGameScriptInterfaceBase: the retail interface (vtable slots from engine/CGameScriptInterfaceBase.h).
#include "engine/CGameScriptInterfaceBase.h"

// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_IntroductionToTrophiesScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CTrophiesIntroTable : public CEntityScriptBase {
    public:
        CV_IntroductionToTrophiesScript* ParentClass;   // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18

        CTrophiesIntroTable(CV_IntroductionToTrophiesScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_IntroductionToTrophiesScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

void CV_IntroductionToTrophiesScript::CTrophiesIntroTable::Main()
{
    Game->SetTavernTableAvailableForUse(Me, false);
}
}
