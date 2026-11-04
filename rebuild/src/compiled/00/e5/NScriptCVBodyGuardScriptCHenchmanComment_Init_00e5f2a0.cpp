// NScript::CV_BodyGuardScript::CHenchmanComment::Init() -- empty: retail is a lone `ret`
// Leaf: ?Init@CHenchmanComment@CV_BodyGuardScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E5F2A0, 1 bytes, thiscall, plain `ret`.
// Identity: reg 00e5d59f "HenchmanComment" in 00e5d4f0; Alloc 00e5f1c0; vtable 012e70c8 slot 2; PDB ?Init@CHenchmanComment@CV_BodyGuardScript@NScript@@; refs=1; was sub_00e5f2a0 (wave3/labels_alloc.tsv); PDB ?Init@CHenchmanComment@CV_BodyGuardScript@NScript@@EAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Hook() on a `struct T` pad model (00/e5/gapscan_sub00e5f2a0_00e5f2a0.cpp, `?Hook@T@@QAEXXZ`); same bytes.
// Model: engine/CEntityScriptBase.h (vptr, Game +0x04, Me +0x08); the entity has no landed PDB-named Alloc TU to take the
//   class model from, so only the base this body touches is modelled; Init private virtual as the PDB spells it.
#include "engine/CCountedPointer.h"                     // CCountedPointer<T> for CScriptThing::PImp (header contract)
#define FABLE_RETAIL_NSCRIPT
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CV_BodyGuardScript {    // the PDB owner script; only its nested entity class is modelled here
public:
    class CHenchmanComment;
};
class CV_BodyGuardScript::CHenchmanComment : public CEntityScriptBase {
private:
    virtual void Init();                          // PDB ?Init@CHenchmanComment@CV_BodyGuardScript@NScript@@EAEXXZ (slot 2)
};
void CV_BodyGuardScript::CHenchmanComment::Init()
{
}
}
