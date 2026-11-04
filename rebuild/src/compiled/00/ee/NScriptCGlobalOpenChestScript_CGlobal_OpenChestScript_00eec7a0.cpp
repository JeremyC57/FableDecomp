// NScript::CGlobal_OpenChestScript::CGlobal_OpenChestScript(CGameScriptInterfaceBase& game) -- CScriptBase(), Game = &game, then the script vtable 0x012F5760
// Leaf: ??0CGlobal_OpenChestScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EEC7A0, 27 bytes, thiscall, `ret 4`; calls CScriptBase::CScriptBase 0x00CB8110.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00eecea0 at 00cda176 with name "Global_OpenChest"; Alloc news + stores script vtable 012f5760; PDB ?Alloc@CGlobal_OpenChestScript@NScript@@SIPAVCScriptBase@@; out-of-line copy of the ctor the Alloc inlines: base CScriptBase ctor 0x00CB8110 on ecx, stores the script vtable, ret 4 = PDB arity; PDB ??0CGlobal_OpenChestScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z; was CVtable_012f5760::CVtable_012f5760 (wave3/labels_alloc.tsv); PDB ??0CGlobal_OpenChestScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z in FableWin.pdb.
// Naming wave 35: was landed as the stand-in `??0CVtable_012f5760@@QAE@PAX@Z` (00/ee/CVtable012f5760_CVtable012f5760_00eec7a0.cpp) on a CBase_00cb8110 base; same bytes.
// Model: engine/CScriptBase.h; a global script has no master data (PDB ctor takes only the interface): Game +0x40.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;
namespace NScript {
class CGlobal_OpenChestScript : public CScriptBase {
public:
    CGlobal_OpenChestScript(CGameScriptInterfaceBase& game);
    virtual ~CGlobal_OpenChestScript();    // ??1CGlobal_OpenChestScript@NScript@@UAE@XZ
    CGameScriptInterfaceBase* Game;         // +0x40
};
CGlobal_OpenChestScript::CGlobal_OpenChestScript(CGameScriptInterfaceBase& game) : Game(&game)
{
}
}
