// CGameScriptInterface::RemoveConversation(long conversation, bool remove_now) const -- the world's script conversation manager, a tail jump
// Leaf: ?RemoveConversation@CGameScriptInterface@@UBEXJ_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008907C0. Identity (NF433, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body: the inline ?GetScriptConversationManager@CWorld@@QAEAAVCScriptConversationManager@@XZ (World +0x7c), then
//   ?RemoveConversation@CScriptConversationManager@@QAEXJ_N@Z (retail 0x006E5990, the jmp target), arguments in order.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_SCRIPTCONVERSATIONMANAGER
#define FABLE_RETAIL_CSCRIPTCONVERSATIONMANAGER_REMOVECONVERSATION
#include "engine/CGameScriptInterface.h"
#include "engine/CWorld.h"
#include "engine/CScriptConversationManager.h"

void CGameScriptInterface::RemoveConversation(long conversation, bool remove_now) const
{
    World->GetScriptConversationManager().RemoveConversation(conversation, remove_now);
}
