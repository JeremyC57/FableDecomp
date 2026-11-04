// NScript::COracleMinigameTestScript::COracleGamesmaster::Init() -- resets the entity state: SaidIntro = false
// Leaf: ?Init@COracleGamesmaster@COracleMinigameTestScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F000B0, 5 bytes, thiscall, plain `ret`.
// Identity: reg 00effe57 "OracleGamesmaster" in 00effe10; Alloc 00f00000; vtable 012f7100 slot 2; PDB ?Init@COracleGamesmaster@COracleMinigameTestScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00f000b0 (wave3/labels_alloc.tsv); PDB ?Init@COracleGamesmaster@COracleMinigameTestScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Clear() on a `struct T` pad model (00/f0/gapscan_sub00f000b0_00f000b0.cpp, `?Clear@T@@QAEXXZ`); same bytes.
// Class model: the landed Alloc row 0x00F00000 (rebuild/src/compiled/00/f0/NScriptCOracleMinigameTestScriptCOracleGamesmaster_Alloc_00f00000.cpp) as wave 33 D1 emits from it: its user-declared
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

// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class COracleMinigameTestScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class COracleGamesmaster : public CEntityScriptBase {
    public:
        COracleMinigameTestScript* ParentClass;         // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        bool SaidIntro;                                 // +0x1C
        COracleGamesmaster(COracleMinigameTestScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(COracleMinigameTestScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
    virtual void Init();                                 // PDB ?Init@COracleGamesmaster@COracleMinigameTestScript@NScript@@EAEXXZ (slot 2)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

void COracleMinigameTestScript::COracleGamesmaster::Init()
{
    SaidIntro = false;
}
}
