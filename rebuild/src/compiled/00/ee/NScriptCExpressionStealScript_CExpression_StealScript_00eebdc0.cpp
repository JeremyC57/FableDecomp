// NScript::CExpression_StealScript::CExpression_StealScript(CGameScriptInterfaceBase& game) -- CScriptBase(), Game = &game, then the script vtable 0x012F5700
// Leaf: ??0CExpression_StealScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EEBDC0, 27 bytes, thiscall, `ret 4`; calls CScriptBase::CScriptBase 0x00CB8110.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00eec390 at 00cd9f12 with name "Expression_Steal"; Alloc news + stores script vtable 012f5700; PDB ?Alloc@CExpression_StealScript@NScript@@SIPAVCScriptBase@@; out-of-line copy of the ctor the Alloc inlines: base CScriptBase ctor 0x00CB8110 on ecx, stores the script vtable, ret 4 = PDB arity; PDB ??0CExpression_StealScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z; was CVtable_012f5700::CVtable_012f5700 (wave3/labels_alloc.tsv); PDB ??0CExpression_StealScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z in FableWin.pdb.
// Naming wave 35: was landed as the stand-in `??0CVtable_012f5700@@QAE@PAX@Z` (00/ee/CVtable012f5700_CVtable012f5700_00eebdc0.cpp) on a CBase_00cb8110 base; same bytes.
// Model: engine/CScriptBase.h; a global script has no master data (PDB ctor takes only the interface): Game +0x40.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;
namespace NScript {
class CExpression_StealScript : public CScriptBase {
public:
    CExpression_StealScript(CGameScriptInterfaceBase& game);
    virtual ~CExpression_StealScript();    // ??1CExpression_StealScript@NScript@@UAE@XZ
    CGameScriptInterfaceBase* Game;         // +0x40
};
CExpression_StealScript::CExpression_StealScript(CGameScriptInterfaceBase& game) : Game(&game)
{
}
}
