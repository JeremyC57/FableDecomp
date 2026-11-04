// NScript::CExpression_FishScript::CExpression_FishScript(CGameScriptInterfaceBase& game) -- CScriptBase(), Game = &game, then the script vtable 0x012F54E4
// Leaf: ??0CExpression_FishScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE94A0, 27 bytes, thiscall, `ret 4`; calls CScriptBase::CScriptBase 0x00CB8110.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00ee9f00 at 00cd9b7c with name "Expression_Fish"; Alloc news + stores script vtable 012f54e4; PDB ?Alloc@CExpression_FishScript@NScript@@SIPAVCScriptBase@@; out-of-line copy of the ctor the Alloc inlines: base CScriptBase ctor 0x00CB8110 on ecx, stores the script vtable, ret 4 = PDB arity; PDB ??0CExpression_FishScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z; was CVtable_012f54e4::CVtable_012f54e4 (wave3/labels_alloc.tsv); PDB ??0CExpression_FishScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z in FableWin.pdb.
// Naming wave 35: was landed as the stand-in `??0CVtable_012f54e4@@QAE@PAX@Z` (00/ee/CVtable012f54e4_CVtable012f54e4_00ee94a0.cpp) on a CBase_00cb8110 base; same bytes.
// Model: engine/CScriptBase.h; a global script has no master data (PDB ctor takes only the interface): Game +0x40.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;
namespace NScript {
class CExpression_FishScript : public CScriptBase {
public:
    CExpression_FishScript(CGameScriptInterfaceBase& game);
    virtual ~CExpression_FishScript();    // ??1CExpression_FishScript@NScript@@UAE@XZ
    CGameScriptInterfaceBase* Game;         // +0x40
};
CExpression_FishScript::CExpression_FishScript(CGameScriptInterfaceBase& game) : Game(&game)
{
}
}
