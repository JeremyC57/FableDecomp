// NScript::CV_FishermanScript::CFishermanWasp::Init() -- counts the entity in: ++ParentClass->WaspsAlive (CV_FishermanScript +0x4c)
// Leaf: ?Init@CFishermanWasp@CV_FishermanScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E7BAF0, 7 bytes, thiscall, plain `ret`.
// Identity: reg 00e7b655 "FishermanWasp" in 00e7b4e0; Alloc 00e7ba40; vtable 012ea0ec slot 2; PDB ?Init@CFishermanWasp@CV_FishermanScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00e7baf0 (wave3/labels_alloc.tsv); PDB ?Init@CFishermanWasp@CV_FishermanScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Bump() on a `struct T` pad model (00/e7/gapscan_sub00e7baf0_00e7baf0.cpp, `?Bump@T@@QAEXXZ`); same bytes.
// Class model: the landed Alloc row 0x00E7BA40 (rebuild/src/compiled/00/e7/NScriptCVFishermanScriptCFishermanWasp_Alloc_00e7ba40.cpp) as wave 33 D1 emits from it: its user-declared
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
class CV_FishermanScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    unsigned char _members48[0x4c - 0x48];    // +0x48 (the script's members before WaspsAlive; not modelled here)
    long WaspsAlive;                          // +0x4c (Ego_r.pdb CV_FishermanScript, retail = Ego_r - 0x14 less the STLport container deltas)
    class CFishermanWasp : public CEntityScriptBase {
    public:
        CV_FishermanScript* ParentClass;                // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18

        CFishermanWasp(CV_FishermanScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_FishermanScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
    virtual void Init();                                 // PDB ?Init@CFishermanWasp@CV_FishermanScript@NScript@@EAEXXZ (slot 2)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

void CV_FishermanScript::CFishermanWasp::Init()
{
    ++ParentClass->WaspsAlive;
}
}
