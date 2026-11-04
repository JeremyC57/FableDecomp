// NScript::CGlobal_GiveHeroItemsFromRewardChestScript::Init() -- empty: retail is a lone `ret`
// Leaf: ?Init@CGlobal_GiveHeroItemsFromRewardChestScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EEC400, 1 bytes, thiscall, plain `ret`.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00eec750 at 00cda0dd with name "Global_GiveHeroItemsFromRewardChest"; Alloc news + stores script vtable 012f5730; PDB ?Alloc@CGlobal_GiveHeroItemsFromRewardChestScript@NScript@@SIPAVCScriptBase@@; script vtable slot 3 (slot 1 pushes "Main" 0x012C2F94); PDB Init; refs=1; was sub_00eec400 (wave3/labels_alloc.tsv); PDB ?Init@CGlobal_GiveHeroItemsFromRewardChestScript@NScript@@UAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Hook() on a `struct T` pad model (00/ee/gapscan_sub00eec400_00eec400.cpp, `?Hook@T@@QAEXXZ`); same bytes.
// Model: engine/CScriptBase.h + Game +0x40 (PDB ctor CGlobal_GiveHeroItemsFromRewardChestScript(CGameScriptInterfaceBase&): no master data); Init declared public virtual as the PDB spells it.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;
namespace NScript {
class CGlobal_GiveHeroItemsFromRewardChestScript : public CScriptBase {
public:
    virtual void Init();                          // slot 3
    CGameScriptInterfaceBase* Game;         // +0x40 (the ctor's CGameScriptInterfaceBase&)
};
void CGlobal_GiveHeroItemsFromRewardChestScript::Init()
{
}
}
