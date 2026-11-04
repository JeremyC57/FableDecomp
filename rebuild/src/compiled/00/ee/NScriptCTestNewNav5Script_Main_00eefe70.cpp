// NScript::CTestNewNav5Script::Main() -- empty: retail is a lone `ret`
// Leaf: ?Main@CTestNewNav5Script@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EEFE70, 1 bytes, thiscall, plain `ret`.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00eeff40 at 00cdacf8 with name "TestNewNav5"; Alloc news + stores script vtable 012f5d4c; PDB ?Alloc@CTestNewNav5Script@NScript@@SIPAVCScriptBase@@; script vtable slot 2 (slot 1 pushes "Main" 0x012C2F94); PDB Main; refs=1; was sub_00eefe70 (wave3/labels_alloc.tsv); PDB ?Main@CTestNewNav5Script@NScript@@UAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Hook() on a `struct T` pad model (00/ee/gapscan_sub00eefe70_00eefe70.cpp, `?Hook@T@@QAEXXZ`); same bytes.
// Model: engine/CLevelScriptBase.h (CLevelScriptBase<CQ_SunnyvaleMasterData>, the quest-script default) (Game +0x40, MasterScriptData +0x44); Main declared public virtual as the PDB spells it.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CTestNewNav5Script : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void Main();                          // slot 2
};
void CTestNewNav5Script::Main()
{
}
}
