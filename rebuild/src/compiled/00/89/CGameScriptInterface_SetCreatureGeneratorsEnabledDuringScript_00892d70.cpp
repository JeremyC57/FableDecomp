// CGameScriptInterface::SetCreatureGeneratorsEnabledDuringScript(const CCharString& script_name, bool enabled) const -- the quest manager singleton's SetCreatureGenerationEnabledDuringScript, a tail jump
// Leaf: ?SetCreatureGeneratorsEnabledDuringScript@CGameScriptInterface@@UBEXABVCCharString@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00892D70. Identity (NF433, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body: ?Get@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@SIAAVCQuestManager@@XZ (inline in retail: the PClass word
//   0x013B89FC), then ?SetCreatureGenerationEnabledDuringScript@CQuestManager@@QAEXABVCCharString@@_N@Z (retail 0x004B3810, the jmp target).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_CREATUREGENERATIONDURINGSCRIPT
#include "engine/CGameScriptInterface.h"
#include "engine/CQuestManager.h"

void CGameScriptInterface::SetCreatureGeneratorsEnabledDuringScript(const CCharString& script_name, bool enabled) const
{
    CQuestManager::Get().SetCreatureGenerationEnabledDuringScript(script_name, enabled);
}
