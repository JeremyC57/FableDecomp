// NScript::CExpression_FollowScript::CExpression_FollowScript(CGameScriptInterfaceBase& game) -- CScriptBase(), Game = &game, then the script vtable 0x012F55B8
// Leaf: ??0CExpression_FollowScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE9F50, 27 bytes, thiscall, `ret 4`; calls CScriptBase::CScriptBase 0x00CB8110.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00eea3e0 at 00cd9c15 with name "Expression_Follow"; Alloc news + stores script vtable 012f55b8; PDB ?Alloc@CExpression_FollowScript@NScript@@SIPAVCScriptBase@@; out-of-line copy of the ctor the Alloc inlines: base CScriptBase ctor 0x00CB8110 on ecx, stores the script vtable, ret 4 = PDB arity; PDB ??0CExpression_FollowScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z; was CVtable_012f55b8::CVtable_012f55b8 (wave3/labels_alloc.tsv); PDB ??0CExpression_FollowScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z in FableWin.pdb.
// Naming wave 35: was landed as the stand-in `??0CVtable_012f55b8@@QAE@PAX@Z` (00/ee/CVtable012f55b8_CVtable012f55b8_00ee9f50.cpp) on a CBase_00cb8110 base; same bytes.
// Model: engine/CScriptBase.h; a global script has no master data (PDB ctor takes only the interface): Game +0x40.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;
namespace NScript {
class CExpression_FollowScript : public CScriptBase {
public:
    CExpression_FollowScript(CGameScriptInterfaceBase& game);
    virtual ~CExpression_FollowScript();    // ??1CExpression_FollowScript@NScript@@UAE@XZ
    CGameScriptInterfaceBase* Game;         // +0x40
};
CExpression_FollowScript::CExpression_FollowScript(CGameScriptInterfaceBase& game) : Game(&game)
{
}
}
