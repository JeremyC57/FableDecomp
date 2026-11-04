// NScript::CExpression_PicklockScript::CExpression_PicklockScript(CGameScriptInterfaceBase& game) -- CScriptBase(), Game = &game, then the script vtable 0x012F56D0
// Leaf: ??0CExpression_PicklockScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EEB780, 27 bytes, thiscall, `ret 4`; calls CScriptBase::CScriptBase 0x00CB8110.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00eebd70 at 00cd9e79 with name "Expression_Picklock"; Alloc news + stores script vtable 012f56d0; PDB ?Alloc@CExpression_PicklockScript@NScript@@SIPAVCScriptBase@@; out-of-line copy of the ctor the Alloc inlines: base CScriptBase ctor 0x00CB8110 on ecx, stores the script vtable, ret 4 = PDB arity; PDB ??0CExpression_PicklockScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z; was CVtable_012f56d0::CVtable_012f56d0 (wave3/labels_alloc.tsv); PDB ??0CExpression_PicklockScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z in FableWin.pdb.
// Naming wave 35: was landed as the stand-in `??0CVtable_012f56d0@@QAE@PAX@Z` (00/ee/CVtable012f56d0_CVtable012f56d0_00eeb780.cpp) on a CBase_00cb8110 base; same bytes.
// Model: engine/CScriptBase.h; a global script has no master data (PDB ctor takes only the interface): Game +0x40.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;
namespace NScript {
class CExpression_PicklockScript : public CScriptBase {
public:
    CExpression_PicklockScript(CGameScriptInterfaceBase& game);
    virtual ~CExpression_PicklockScript();    // ??1CExpression_PicklockScript@NScript@@UAE@XZ
    CGameScriptInterfaceBase* Game;         // +0x40
};
CExpression_PicklockScript::CExpression_PicklockScript(CGameScriptInterfaceBase& game) : Game(&game)
{
}
}
