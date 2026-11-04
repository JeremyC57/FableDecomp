// NScript::CQS_MeetSisterScript::Init() -- empty: retail is a lone `ret`
// Leaf: ?Init@CQS_MeetSisterScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E28D30, 1 bytes, thiscall, plain `ret`.
// Identity: outer script vtable 012e2194 slot 3 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00e28de0 (slot 2) registers 2 entity scripts whose PDB Alloc outer is CQS_MeetSisterScript; PDB ?Init@CQS_MeetSisterScript@NScript@@; refs=1; was sub_00e28d30 (wave3/labels_alloc.tsv); PDB ?Init@CQS_MeetSisterScript@NScript@@UAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Hook() on a `struct T` pad model (00/e2/gapscan_sub00e28d30_00e28d30.cpp, `?Hook@T@@QAEXXZ`); same bytes.
// Model: engine/CLevelScriptBase.h (Game +0x40, MasterScriptData +0x44); Init declared public virtual as the PDB spells it.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQS_MeetSisterScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void Init();                          // slot 3
};
void CQS_MeetSisterScript::Init()
{
}
}
