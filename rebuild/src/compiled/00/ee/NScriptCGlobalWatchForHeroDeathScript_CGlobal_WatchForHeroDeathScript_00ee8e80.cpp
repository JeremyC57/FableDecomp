// NScript::CGlobal_WatchForHeroDeathScript::CGlobal_WatchForHeroDeathScript(CGameScriptInterfaceBase& game) -- CScriptBase(), Game = &game, then the script vtable 0x012F5470
// Leaf: ??0CGlobal_WatchForHeroDeathScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE8E80, 27 bytes, thiscall, `ret 4`; calls CScriptBase::CScriptBase 0x00CB8110.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00ee90a0 at 00cd9a4a with name "Global_WatchForHeroDeath"; Alloc news + stores script vtable 012f5470; PDB ?Alloc@CGlobal_WatchForHeroDeathScript@NScript@@SIPAVCScriptBase@@; out-of-line copy of the ctor the Alloc inlines: base CScriptBase ctor 0x00CB8110 on ecx, stores the script vtable, ret 4 = PDB arity; PDB ??0CGlobal_WatchForHeroDeathScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z; was CVtable_012f5470::CVtable_012f5470 (wave3/labels_alloc.tsv); PDB ??0CGlobal_WatchForHeroDeathScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z in FableWin.pdb.
// Naming wave 35: was landed as the stand-in `??0CVtable_012f5470@@QAE@PAX@Z` (00/ee/CVtable012f5470_CVtable012f5470_00ee8e80.cpp) on a CBase_00cb8110 base; same bytes.
// Model: engine/CScriptBase.h; a global script has no master data (PDB ctor takes only the interface): Game +0x40.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;
namespace NScript {
class CGlobal_WatchForHeroDeathScript : public CScriptBase {
public:
    CGlobal_WatchForHeroDeathScript(CGameScriptInterfaceBase& game);
    virtual ~CGlobal_WatchForHeroDeathScript();    // ??1CGlobal_WatchForHeroDeathScript@NScript@@UAE@XZ
    CGameScriptInterfaceBase* Game;         // +0x40
};
CGlobal_WatchForHeroDeathScript::CGlobal_WatchForHeroDeathScript(CGameScriptInterfaceBase& game) : Game(&game)
{
}
}
