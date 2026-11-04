// NScript::CGlobal_TeleportToHeroGuildScript::Init() -- empty: retail is a lone `ret`
// Leaf: ?Init@CGlobal_TeleportToHeroGuildScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CDD5E0, 1 bytes, thiscall, plain `ret`.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00cddb60 at 00cd9fab with name "Global_TeleportToHeroGuild"; Alloc news + stores script vtable 012c2fb8; PDB ?Alloc@CGlobal_TeleportToHeroGuildScript@NScript@@SIPAVCScriptBase@@; script vtable slot 3 (slot 1 pushes "Main" 0x012C2F94); PDB Init; refs=1; was sub_00cdd5e0 (wave3/labels_alloc.tsv); PDB ?Init@CGlobal_TeleportToHeroGuildScript@NScript@@UAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Hook() on a `struct T` pad model (00/cd/gapscan_sub00cdd5e0_00cdd5e0.cpp, `?Hook@T@@QAEXXZ`); same bytes.
// Model: engine/CLevelScriptBase.h (CLevelScriptBase<CQ_SunnyvaleMasterData>, the quest-script default) (Game +0x40, MasterScriptData +0x44); Init declared public virtual as the PDB spells it.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CGlobal_TeleportToHeroGuildScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void Init();                          // slot 3
};
void CGlobal_TeleportToHeroGuildScript::Init()
{
}
}
