// NScript::CQ_OrchardFarmRaidEvilScript::Main() -- empty: retail is a lone `ret`
// Leaf: ?Main@CQ_OrchardFarmRaidEvilScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DD2010, 1 bytes, thiscall, plain `ret`.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00dd2300 at 00cd7020 with name "Q_OrchardFarmRaidEvil"; Alloc news + stores script vtable 012db7b0; PDB ?Alloc@CQ_OrchardFarmRaidEvilScript@NScript@@SIPAVCScriptBase@@; script vtable slot 2 (slot 1 pushes "Main" 0x012C2F94); PDB Main; refs=1; was sub_00dd2010 (wave3/labels_alloc.tsv); PDB ?Main@CQ_OrchardFarmRaidEvilScript@NScript@@UAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Hook() on a `struct T` pad model (00/dd/gapscan_sub00dd2010_00dd2010.cpp, `?Hook@T@@QAEXXZ`); same bytes.
// Model: engine/CLevelScriptBase.h (CLevelScriptBase<CQ_SunnyvaleMasterData>, the quest-script default) (Game +0x40, MasterScriptData +0x44); Main declared public virtual as the PDB spells it.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_OrchardFarmRaidEvilScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void Main();                          // slot 2
};
void CQ_OrchardFarmRaidEvilScript::Main()
{
}
}
