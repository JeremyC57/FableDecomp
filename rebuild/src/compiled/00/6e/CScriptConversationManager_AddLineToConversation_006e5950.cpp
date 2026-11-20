// CScriptConversationManager::AddLineToConversation(long conversation, const CThing& speaker, const CThing* listener, const CCharString& line, bool play_during_cut_scenes) -- forwards the line to the conversation found under that index (nothing when it is missing)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E5950, length 64 bytes.
// Leaf: ?AddLineToConversation@CScriptConversationManager@@QAEXJABVCThing@@PBV2@ABVCCharString@@_N@Z
// Retail bytes: (the find prologue of 0x006E5800) ; je E ; mov edx,[esp+18h] ; mov ecx,[esp+14h] ; push edx ; mov edx,[esp+14h] ; push ecx ; mov ecx,[esp+14h] ; push edx ; push ecx ;
//               lea ecx,[eax+14h] ; call AddLineToConversation ; E: pop ecx ; ret 14h
// Callees: Conversations.find @ 0x006E6990 (STLport map<long, CScriptConversation>, the iterator through the hidden result;
// the manifest label "BinaryTreeNode_FindClosestLessEqual" is generic) and the conversation's own member @ 0x006E58A0 on
// &it->second (node + 0x14: STLport node {color, parent, left, right, key +0x10, value +0x14}).
// Caller: CGameScriptInterface::AddLineToConversation (landed) through the world's conversation manager.
// Layout (PDB): Conversations (map) +0, NextConversationIndex, AlwaysShowText; the retail STLport map is 12 bytes. The
// engine header CScriptConversation.h gives the value type (only its address is used here).
// Shape: `iterator it = find(conversation)` constructs straight into the frame slot, `end()` is the header node.
// Name fix (NF185, 2026-11-20, q4388): the invented CConversationMap / SConversationNode / CScriptConversationCalls stand-ins are
//   replaced in place by the STLport map<long, CScriptConversation> (stlport/fable_map.h; its member-template find<long> 0x006E6990
//   declared only, FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE_DECL) and the PDB members of engine/CScriptConversation.h
//   (FABLE_RETAIL_CSCRIPTCONVERSATION_ADDTOCONVERSATION: they take the thing / line by reference).
// Name fix (NF202, 2026-11-20, q12195): the PDB signature (Ego_r.pdb 0x0070EE80 + FableWin.pdb 0x02409E00
//   ?AddLineToConversation@CScriptConversationManager@@QAEXJABVCThing@@PBV2@ABVCCharString@@_N@Z: speaker and line by const
//   reference, listener const*); the local CScriptConversationManager model is replaced by engine/CScriptConversationManager.h
//   (FABLE_RETAIL_CSCRIPTCONVERSATIONMANAGER_ADDLINETOCONVERSATION).
#define FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE_DECL
#define FABLE_RETAIL_CSCRIPTCONVERSATION_ADDTOCONVERSATION
#define FABLE_RETAIL_CSCRIPTCONVERSATIONMANAGER_ADDLINETOCONVERSATION
#include "engine/CScriptConversationManager.h"

void CScriptConversationManager::AddLineToConversation(long conversation, const CThing& speaker, const CThing* listener,
                                                       const CCharString& line, bool play_during_cut_scenes)
{
    std::map<long, CScriptConversation>::iterator it = Conversations.find(conversation);
    if (it != Conversations.end())
        it->second.AddLineToConversation(speaker, listener, line, play_during_cut_scenes);
}
