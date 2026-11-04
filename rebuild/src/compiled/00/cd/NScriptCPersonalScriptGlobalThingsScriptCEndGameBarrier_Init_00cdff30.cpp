// NScript::CPersonalScript_GlobalThingsScript::CEndGameBarrier::Init() -- Game->EntitySetAsDrawable(Me, false) (CGameScriptInterfaceBase slot 511, +0x7fc, 0x0088E9F0)
// Leaf: ?Init@CEndGameBarrier@CPersonalScript_GlobalThingsScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CDFF30, 20 bytes, thiscall, plain `ret`.
// Identity: reg 00cde8cf "EndGameBarrier" in 00cde420; Alloc 00cdfe30; vtable 012c3434 slot 2; PDB ?Init@CEndGameBarrier@CPersonalScript_GlobalThingsScript@NScript@@; refs=1; was sub_00cdff30 (wave3/labels_alloc.tsv); PDB ?Init@CEndGameBarrier@CPersonalScript_GlobalThingsScript@NScript@@EAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Run() on a `struct T` pad model (00/cd/gapscan_sub00cdff30_00cdff30.cpp, `?Run@T@@QAEXXZ`); same bytes.
// Model: engine/CEntityScriptBase.h (vptr, Game +0x04, Me +0x08); the entity has no landed PDB-named Alloc TU to take the
//   class model from, so only the base this body touches is modelled; Init private virtual as the PDB spells it.
#include "engine/CCountedPointer.h"                     // CCountedPointer<T> for CScriptThing::PImp (header contract)
#define FABLE_RETAIL_NSCRIPT
#include "engine/CEntityScriptBase.h"
// CGameScriptInterfaceBase: the retail interface (vtable slots from engine/CGameScriptInterfaceBase.h).
#include "engine/CGameScriptInterfaceBase.h"
namespace NScript {
class CPersonalScript_GlobalThingsScript {    // the PDB owner script; only its nested entity class is modelled here
public:
    class CEndGameBarrier;
};
class CPersonalScript_GlobalThingsScript::CEndGameBarrier : public CEntityScriptBase {
private:
    virtual void Init();                          // PDB ?Init@CEndGameBarrier@CPersonalScript_GlobalThingsScript@NScript@@EAEXXZ (slot 2)
};
void CPersonalScript_GlobalThingsScript::CEndGameBarrier::Init()
{
    Game->EntitySetAsDrawable(Me, false);
}
}
