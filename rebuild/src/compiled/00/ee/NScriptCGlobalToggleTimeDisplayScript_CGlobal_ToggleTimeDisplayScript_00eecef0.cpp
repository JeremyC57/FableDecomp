// NScript::CGlobal_ToggleTimeDisplayScript::CGlobal_ToggleTimeDisplayScript(CGameScriptInterfaceBase& game) -- CScriptBase(), Game = &game, then the script vtable 0x012F57A0
// Leaf: ??0CGlobal_ToggleTimeDisplayScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EECEF0, 27 bytes, thiscall, `ret 4`; calls CScriptBase::CScriptBase 0x00CB8110.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00eed060 at 00cda044 with name "Global_ToggleTimeDisplay"; Alloc news + stores script vtable 012f57a0; PDB ?Alloc@CGlobal_ToggleTimeDisplayScript@NScript@@SIPAVCScriptBase@@; out-of-line copy of the ctor the Alloc inlines: base CScriptBase ctor 0x00CB8110 on ecx, stores the script vtable, ret 4 = PDB arity; PDB ??0CGlobal_ToggleTimeDisplayScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z; was CVtable_012f57a0::CVtable_012f57a0 (wave3/labels_alloc.tsv); PDB ??0CGlobal_ToggleTimeDisplayScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z in FableWin.pdb.
// Naming wave 35: was landed as the stand-in `??0CVtable_012f57a0@@QAE@PAX@Z` (00/ee/CVtable012f57a0_CVtable012f57a0_00eecef0.cpp) on a CBase_00cb8110 base; same bytes.
// Model: engine/CScriptBase.h; a global script has no master data (PDB ctor takes only the interface): Game +0x40.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;
namespace NScript {
class CGlobal_ToggleTimeDisplayScript : public CScriptBase {
public:
    CGlobal_ToggleTimeDisplayScript(CGameScriptInterfaceBase& game);
    virtual ~CGlobal_ToggleTimeDisplayScript();    // ??1CGlobal_ToggleTimeDisplayScript@NScript@@UAE@XZ
    CGameScriptInterfaceBase* Game;         // +0x40
};
CGlobal_ToggleTimeDisplayScript::CGlobal_ToggleTimeDisplayScript(CGameScriptInterfaceBase& game) : Game(&game)
{
}
}
