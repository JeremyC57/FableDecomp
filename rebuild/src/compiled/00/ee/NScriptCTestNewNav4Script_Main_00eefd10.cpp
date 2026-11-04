// NScript::CTestNewNav4Script::Main() -- empty: retail is a lone `ret`
// Leaf: ?Main@CTestNewNav4Script@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EEFD10, 1 bytes, thiscall, plain `ret`.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00eefde0 at 00cdac5b with name "TestNewNav4"; Alloc news + stores script vtable 012f5d1c; PDB ?Alloc@CTestNewNav4Script@NScript@@SIPAVCScriptBase@@; script vtable slot 2 (slot 1 pushes "Main" 0x012C2F94); PDB Main; refs=1; was sub_00eefd10 (wave3/labels_alloc.tsv); PDB ?Main@CTestNewNav4Script@NScript@@UAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Hook() on a `struct T` pad model (00/ee/gapscan_sub00eefd10_00eefd10.cpp, `?Hook@T@@QAEXXZ`); same bytes.
// Model: engine/CLevelScriptBase.h (CLevelScriptBase<CQ_SunnyvaleMasterData>, the quest-script default) (Game +0x40, MasterScriptData +0x44); Main declared public virtual as the PDB spells it.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CTestNewNav4Script : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void Main();                          // slot 2
};
void CTestNewNav4Script::Main()
{
}
}
