// NScript::CQ_HangingTreeGoodScript::CHTG_Audience::Init() -- empty: retail is a lone `ret`
// Leaf: ?Init@CHTG_Audience@CQ_HangingTreeGoodScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D721F0, 1 bytes, thiscall, plain `ret`.
// Identity: reg 00d6f856 "HTG_Audience" in 00d6f4b0; Alloc 00d720e0; vtable 012d292c slot 2; PDB ?Init@CHTG_Audience@CQ_HangingTreeGoodScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00d721f0 (wave3/labels_alloc.tsv); PDB ?Init@CHTG_Audience@CQ_HangingTreeGoodScript@NScript@@EAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Hook() on a `struct T` pad model (00/d7/gapscan_sub00d721f0_00d721f0.cpp, `?Hook@T@@QAEXXZ`); same bytes.
// Model: engine/CEntityScriptBase.h (vptr, Game +0x04, Me +0x08); the entity has no landed PDB-named Alloc TU to take the
//   class model from, so only the base this body touches is modelled; Init private virtual as the PDB spells it.
#include "engine/CCountedPointer.h"                     // CCountedPointer<T> for CScriptThing::PImp (header contract)
#define FABLE_RETAIL_NSCRIPT
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_HangingTreeGoodScript {    // the PDB owner script; only its nested entity class is modelled here
public:
    class CHTG_Audience;
};
class CQ_HangingTreeGoodScript::CHTG_Audience : public CEntityScriptBase {
private:
    virtual void Init();                          // PDB ?Init@CHTG_Audience@CQ_HangingTreeGoodScript@NScript@@EAEXXZ (slot 2)
};
void CQ_HangingTreeGoodScript::CHTG_Audience::Init()
{
}
}
