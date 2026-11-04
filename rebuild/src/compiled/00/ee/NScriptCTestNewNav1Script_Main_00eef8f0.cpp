// NScript::CTestNewNav1Script::Main() -- empty: retail is a lone `ret`
// Leaf: ?Main@CTestNewNav1Script@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EEF8F0, 1 bytes, thiscall, plain `ret`.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00eef9c0 at 00cdaa84 with name "TestNewNav1"; Alloc news + stores script vtable 012f5c8c; PDB ?Alloc@CTestNewNav1Script@NScript@@SIPAVCScriptBase@@; script vtable slot 2 (slot 1 pushes "Main" 0x012C2F94); PDB Main; refs=1; was sub_00eef8f0 (wave3/labels_alloc.tsv); PDB ?Main@CTestNewNav1Script@NScript@@UAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Hook() on a `struct T` pad model (00/ee/gapscan_sub00eef8f0_00eef8f0.cpp, `?Hook@T@@QAEXXZ`); same bytes.
// Model: engine/CLevelScriptBase.h (CLevelScriptBase<CQ_SunnyvaleMasterData>, the quest-script default) (Game +0x40, MasterScriptData +0x44); Main declared public virtual as the PDB spells it.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CTestNewNav1Script : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void Main();                          // slot 2
};
void CTestNewNav1Script::Main()
{
}
}
