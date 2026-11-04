// NScript::CPersonalScript_alextestScript::Init() -- empty: retail is a lone `ret`
// Leaf: ?Init@CPersonalScript_alextestScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EED2B0, 1 bytes, thiscall, plain `ret`.
// Identity: outer script vtable 012f5814 slot 3 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00eed360 (slot 2) registers 3 entity scripts whose PDB Alloc outer is CPersonalScript_alextestScript; PDB ?Init@CPersonalScript_alextestScript@NScript@@; refs=1; was sub_00eed2b0 (wave3/labels_alloc.tsv); PDB ?Init@CPersonalScript_alextestScript@NScript@@UAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Hook() on a `struct T` pad model (00/ee/gapscan_sub00eed2b0_00eed2b0.cpp, `?Hook@T@@QAEXXZ`); same bytes.
// Model: engine/CLevelScriptBase.h (Game +0x40, MasterScriptData +0x44); Init declared public virtual as the PDB spells it.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CalextestData;
class CPersonalScript_alextestScript : public CLevelScriptBase<CalextestData> {
public:
    virtual void Init();                          // slot 3
};
void CPersonalScript_alextestScript::Init()
{
}
}
