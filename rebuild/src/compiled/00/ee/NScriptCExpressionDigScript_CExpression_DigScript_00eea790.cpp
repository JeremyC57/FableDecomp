// NScript::CExpression_DigScript::CExpression_DigScript(CGameScriptInterfaceBase& game) -- CScriptBase(), Game = &game, then the script vtable 0x012F5628
// Leaf: ??0CExpression_DigScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EEA790, 27 bytes, thiscall, `ret 4`; calls CScriptBase::CScriptBase 0x00CB8110.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00eeae70 at 00cd9d47 with name "Expression_Dig"; Alloc news + stores script vtable 012f5628; PDB ?Alloc@CExpression_DigScript@NScript@@SIPAVCScriptBase@@; out-of-line copy of the ctor the Alloc inlines: base CScriptBase ctor 0x00CB8110 on ecx, stores the script vtable, ret 4 = PDB arity; PDB ??0CExpression_DigScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z; was CVtable_012f5628::CVtable_012f5628 (wave3/labels_alloc.tsv); PDB ??0CExpression_DigScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z in FableWin.pdb.
// Naming wave 35: was landed as the stand-in `??0CVtable_012f5628@@QAE@PAX@Z` (00/ee/CVtable012f5628_CVtable012f5628_00eea790.cpp) on a CBase_00cb8110 base; same bytes.
// Model: engine/CScriptBase.h; a global script has no master data (PDB ctor takes only the interface): Game +0x40.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;
namespace NScript {
class CExpression_DigScript : public CScriptBase {
public:
    CExpression_DigScript(CGameScriptInterfaceBase& game);
    virtual ~CExpression_DigScript();    // ??1CExpression_DigScript@NScript@@UAE@XZ
    CGameScriptInterfaceBase* Game;         // +0x40
};
CExpression_DigScript::CExpression_DigScript(CGameScriptInterfaceBase& game) : Game(&game)
{
}
}
