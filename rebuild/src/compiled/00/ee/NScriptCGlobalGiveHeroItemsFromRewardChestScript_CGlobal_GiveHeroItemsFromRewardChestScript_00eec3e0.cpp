// NScript::CGlobal_GiveHeroItemsFromRewardChestScript::CGlobal_GiveHeroItemsFromRewardChestScript(CGameScriptInterfaceBase& game) -- CScriptBase(), Game = &game, then the script vtable 0x012F5730
// Leaf: ??0CGlobal_GiveHeroItemsFromRewardChestScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EEC3E0, 27 bytes, thiscall, `ret 4`; calls CScriptBase::CScriptBase 0x00CB8110.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00eec750 at 00cda0dd with name "Global_GiveHeroItemsFromRewardChest"; Alloc news + stores script vtable 012f5730; PDB ?Alloc@CGlobal_GiveHeroItemsFromRewardChestScript@NScript@@SIPAVCScriptBase@@; out-of-line copy of the ctor the Alloc inlines: base CScriptBase ctor 0x00CB8110 on ecx, stores the script vtable, ret 4 = PDB arity; PDB ??0CGlobal_GiveHeroItemsFromRewardChestScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z; was CVtable_012f5730::CVtable_012f5730 (wave3/labels_alloc.tsv); PDB ??0CGlobal_GiveHeroItemsFromRewardChestScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z in FableWin.pdb.
// Naming wave 35: was landed as the stand-in `??0CVtable_012f5730@@QAE@PAX@Z` (00/ee/CVtable012f5730_CVtable012f5730_00eec3e0.cpp) on a CBase_00cb8110 base; same bytes.
// Model: engine/CScriptBase.h; a global script has no master data (PDB ctor takes only the interface): Game +0x40.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;
namespace NScript {
class CGlobal_GiveHeroItemsFromRewardChestScript : public CScriptBase {
public:
    CGlobal_GiveHeroItemsFromRewardChestScript(CGameScriptInterfaceBase& game);
    virtual ~CGlobal_GiveHeroItemsFromRewardChestScript();    // ??1CGlobal_GiveHeroItemsFromRewardChestScript@NScript@@UAE@XZ
    CGameScriptInterfaceBase* Game;         // +0x40
};
CGlobal_GiveHeroItemsFromRewardChestScript::CGlobal_GiveHeroItemsFromRewardChestScript(CGameScriptInterfaceBase& game) : Game(&game)
{
}
}
