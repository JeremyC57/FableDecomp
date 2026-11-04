// NScript::CQ_WhiteBalverineWWScript::CWBWW_WhiteBalverine::Main() -- run script frames until the thread is told to terminate
// Leaf: ?Main@CWBWW_WhiteBalverine@CQ_WhiteBalverineWWScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E191A0, 24 bytes, thiscall, plain `ret`; calls Game (+0x04) slot 7 NewScriptFrame (+0x1c), CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30.
// Identity: reg 00e186f1 "WBWW_WhiteBalverine" in 00e18630; Alloc 00e190a0; vtable 012e0e30 slot 1; PDB ?Main@CWBWW_WhiteBalverine@CQ_WhiteBalverineWWScript@NScript@@; refs=1; was CVtable_012e0e30::vfunc_1 (wave3/labels_alloc.tsv); PDB ?Main@CWBWW_WhiteBalverine@CQ_WhiteBalverineWWScript@NScript@@EAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?CQ_SunnyvaleMasterScript__Main@@YIXPAUCQ_SunnyvaleMasterScript@@@Z` on a pad model (00/e1/CQSunnyvaleMasterScript_Main_00e191a0.cpp); same bytes.
// Class model: the landed Alloc row 0x00E190A0 (rebuild/src/compiled/00/e1/NScriptCQWhiteBalverineWWScriptCWBWWWhiteBalverine_Alloc_00e190a0.cpp) as wave 33 D1 emits from it: its user-declared
//   destructor dropped, Main private virtual as the PDB spells it; the Alloc body is not part of this TU.
class CGameScriptInterfaceBase;
class CScriptDataBase;

// 0x00CDEE00 is CCountedPointer<T>::DeleteData (engine/CCountedPointer.h; reloc_bindings binds both spellings)

#include "engine/CCountedPointer.h"


// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
// CGameScriptInterfaceBase: the retail interface (slot 7 NewScriptFrame, +0x1c).
#include "engine/CGameScriptInterfaceBase.h"

// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_WhiteBalverineWWScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CWBWW_WhiteBalverine : public CEntityScriptBase {
    public:
        CQ_WhiteBalverineWWScript* ParentClass;         // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18

        CWBWW_WhiteBalverine(CQ_WhiteBalverineWWScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_WhiteBalverineWWScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

void CQ_WhiteBalverineWWScript::CWBWW_WhiteBalverine::Main()
{
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}
}
