// NScript::CV_StatueMasterScript::Init() -- empty: retail is a lone `ret`
// Leaf: ?Init@CV_StatueMasterScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ED3A80, 1 bytes, thiscall, plain `ret`.
// Identity: outer script vtable 012f2e6c slot 3 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00ed3b30 (slot 2) registers 3 entity scripts whose PDB Alloc outer is CV_StatueMasterScript; PDB ?Init@CV_StatueMasterScript@NScript@@; refs=1; was sub_00ed3a80 (wave3/labels_alloc.tsv); PDB ?Init@CV_StatueMasterScript@NScript@@UAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Hook() on a `struct T` pad model (00/ed/gapscan_sub00ed3a80_00ed3a80.cpp, `?Hook@T@@QAEXXZ`); same bytes.
// Model: engine/CLevelScriptBase.h (Game +0x40, MasterScriptData +0x44); Init declared public virtual as the PDB spells it.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_StatueMasterScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void Init();                          // slot 3
};
void CV_StatueMasterScript::Init()
{
}
}
