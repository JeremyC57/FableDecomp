// CScriptConversationManager::AddPersonToConversation(long conversation, CThing* person) -- forwards the person to the conversation found under that index (nothing when it is missing)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E5800, length 49 bytes.
// Leaf: ?AddPersonToConversation@CScriptConversationManager
// Retail bytes: push ecx ; push esi ; mov esi,ecx ; lea eax,[esp+0Ch] ; push eax ; lea ecx,[esp+8] ; push ecx ; mov ecx,esi ; call find ; mov ecx,[esi] ; mov eax,[esp+4] ;
//               cmp eax,ecx ; pop esi ; je E ; mov edx,[esp+0Ch] ; push edx ; lea ecx,[eax+14h] ; call AddPersonToConversation ; E: pop ecx ; ret 8
// Callees: Conversations.find @ 0x006E6990 (STLport map<long, CScriptConversation>, the iterator through the hidden result;
// the manifest label "BinaryTreeNode_FindClosestLessEqual" is generic) and the conversation's own member @ 0x006E5790 on
// &it->second (node + 0x14: STLport node {color, parent, left, right, key +0x10, value +0x14}).
// Caller: CGameScriptInterface::AddPersonToConversation (landed) through the world's conversation manager.
// Layout (PDB): Conversations (map) +0, NextConversationIndex, AlwaysShowText; the retail STLport map is 12 bytes. The
// engine header CScriptConversation.h gives the value type (only its address is used here).
// Shape: `iterator it = find(conversation)` constructs straight into the frame slot, `end()` is the header node.
// Name fix (NF185, 2026-11-20, q4388): the invented CConversationMap / SConversationNode / CScriptConversationCalls stand-ins are
//   replaced in place by the STLport map<long, CScriptConversation> (stlport/fable_map.h; its member-template find<long> 0x006E6990
//   declared only, FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE_DECL) and the PDB members of engine/CScriptConversation.h
//   (FABLE_RETAIL_CSCRIPTCONVERSATION_ADDTOCONVERSATION: they take the thing / line by reference).
#define FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE_DECL
#define FABLE_RETAIL_CSCRIPTCONVERSATION_ADDTOCONVERSATION
#include "stlport/fable_map.h"
#include "engine/CScriptConversation.h"

class CThing;
class CCharString;

class CScriptConversationManager { public:
    std::map<long, CScriptConversation> Conversations;   // +0x00
    long             NextConversationIndex;    // +0x0c
    bool             AlwaysShowText;           // +0x10
    void AddPersonToConversation(long conversation, CThing* person);
    void AddLineToConversation(long conversation, CThing* speaker, CThing* listener, const CCharString* line,
                               bool playDuringCutScenes);
};

void CScriptConversationManager::AddPersonToConversation(long conversation, CThing* person)
{
    std::map<long, CScriptConversation>::iterator it = Conversations.find(conversation);
    if (it != Conversations.end())
        it->second.AddPersonToConversation(*person);
}