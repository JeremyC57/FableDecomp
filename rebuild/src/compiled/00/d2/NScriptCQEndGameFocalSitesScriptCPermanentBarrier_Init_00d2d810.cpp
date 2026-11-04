// NScript::CQ_EndGameFocalSitesScript::CPermanentBarrier::Init() -- Game->EntitySetAsDrawable(Me, false) (CGameScriptInterfaceBase slot 511, +0x7fc, 0x0088E9F0)
// Leaf: ?Init@CPermanentBarrier@CQ_EndGameFocalSitesScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D2D810, 20 bytes, thiscall, plain `ret`.
// Identity: reg 00d2af3b "PermanentBarrier" in 00d2aa40; Alloc 00d2d730; vtable 012cc48c slot 2; PDB ?Init@CPermanentBarrier@CQ_EndGameFocalSitesScript@NScript@@; refs=1; was sub_00d2d810 (wave3/labels_alloc.tsv); PDB ?Init@CPermanentBarrier@CQ_EndGameFocalSitesScript@NScript@@EAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Run() on a `struct T` pad model (00/d2/gapscan_sub00d2d810_00d2d810.cpp, `?Run@T@@QAEXXZ`); same bytes.
// Model: engine/CEntityScriptBase.h (vptr, Game +0x04, Me +0x08); the entity has no landed PDB-named Alloc TU to take the
//   class model from, so only the base this body touches is modelled; Init private virtual as the PDB spells it.
#include "engine/CCountedPointer.h"                     // CCountedPointer<T> for CScriptThing::PImp (header contract)
#define FABLE_RETAIL_NSCRIPT
#include "engine/CEntityScriptBase.h"
// CGameScriptInterfaceBase: the retail interface (vtable slots from engine/CGameScriptInterfaceBase.h).
#include "engine/CGameScriptInterfaceBase.h"
namespace NScript {
class CQ_EndGameFocalSitesScript {    // the PDB owner script; only its nested entity class is modelled here
public:
    class CPermanentBarrier;
};
class CQ_EndGameFocalSitesScript::CPermanentBarrier : public CEntityScriptBase {
private:
    virtual void Init();                          // PDB ?Init@CPermanentBarrier@CQ_EndGameFocalSitesScript@NScript@@EAEXXZ (slot 2)
};
void CQ_EndGameFocalSitesScript::CPermanentBarrier::Init()
{
    Game->EntitySetAsDrawable(Me, false);
}
}
