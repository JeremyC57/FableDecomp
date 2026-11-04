// NScript::CExpression_PickpocketScript::CExpression_PickpocketScript(CGameScriptInterfaceBase& game) -- CScriptBase(), Game = &game, then the script vtable 0x012F56A0
// Leaf: ??0CExpression_PickpocketScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EEAEC0, 27 bytes, thiscall, `ret 4`; calls CScriptBase::CScriptBase 0x00CB8110.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00eeb730 at 00cd9de0 with name "Expression_Pickpocket"; Alloc news + stores script vtable 012f56a0; PDB ?Alloc@CExpression_PickpocketScript@NScript@@SIPAVCScriptBase@@; out-of-line copy of the ctor the Alloc inlines: base CScriptBase ctor 0x00CB8110 on ecx, stores the script vtable, ret 4 = PDB arity; PDB ??0CExpression_PickpocketScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z; was CVtable_012f56a0::CVtable_012f56a0 (wave3/labels_alloc.tsv); PDB ??0CExpression_PickpocketScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z in FableWin.pdb.
// Naming wave 35: was landed as the stand-in `??0CVtable_012f56a0@@QAE@PAX@Z` (00/ee/CVtable012f56a0_CVtable012f56a0_00eeaec0.cpp) on a CBase_00cb8110 base; same bytes.
// Model: engine/CScriptBase.h; a global script has no master data (PDB ctor takes only the interface): Game +0x40.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;
namespace NScript {
class CExpression_PickpocketScript : public CScriptBase {
public:
    CExpression_PickpocketScript(CGameScriptInterfaceBase& game);
    virtual ~CExpression_PickpocketScript();    // ??1CExpression_PickpocketScript@NScript@@UAE@XZ
    CGameScriptInterfaceBase* Game;         // +0x40
};
CExpression_PickpocketScript::CExpression_PickpocketScript(CGameScriptInterfaceBase& game) : Game(&game)
{
}
}
