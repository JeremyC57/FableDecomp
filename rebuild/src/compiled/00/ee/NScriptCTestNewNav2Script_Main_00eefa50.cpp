// NScript::CTestNewNav2Script::Main() -- empty: retail is a lone `ret`
// Leaf: ?Main@CTestNewNav2Script@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EEFA50, 1 bytes, thiscall, plain `ret`.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00eefb20 at 00cdab21 with name "TestNewNav2"; Alloc news + stores script vtable 012f5cbc; PDB ?Alloc@CTestNewNav2Script@NScript@@SIPAVCScriptBase@@; script vtable slot 2 (slot 1 pushes "Main" 0x012C2F94); PDB Main; refs=1; was sub_00eefa50 (wave3/labels_alloc.tsv); PDB ?Main@CTestNewNav2Script@NScript@@UAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Hook() on a `struct T` pad model (00/ee/gapscan_sub00eefa50_00eefa50.cpp, `?Hook@T@@QAEXXZ`); same bytes.
// Model: engine/CLevelScriptBase.h (CLevelScriptBase<CQ_SunnyvaleMasterData>, the quest-script default) (Game +0x40, MasterScriptData +0x44); Main declared public virtual as the PDB spells it.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CTestNewNav2Script : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void Main();                          // slot 2
};
void CTestNewNav2Script::Main()
{
}
}
