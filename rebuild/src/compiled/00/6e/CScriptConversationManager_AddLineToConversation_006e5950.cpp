// CScriptConversationManager::AddLineToConversation(long conversation, CThing* speaker, CThing* listener, const CCharString* line, bool playDuringCutScenes) -- forwards the line to the conversation found under that index (nothing when it is missing)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E5950, length 64 bytes.
// Leaf: ?AddLineToConversation@CScriptConversationManager
// Retail bytes: (the find prologue of 0x006E5800) ; je E ; mov edx,[esp+18h] ; mov ecx,[esp+14h] ; push edx ; mov edx,[esp+14h] ; push ecx ; mov ecx,[esp+14h] ; push edx ; push ecx ;
//               lea ecx,[eax+14h] ; call AddLineToConversation ; E: pop ecx ; ret 14h
// Callees: Conversations.find @ 0x006E6990 (STLport map<long, CScriptConversation>, the iterator through the hidden result;
// the manifest label "BinaryTreeNode_FindClosestLessEqual" is generic) and the conversation's own member @ 0x006E58A0 on
// &it->second (node + 0x14: STLport node {color, parent, left, right, key +0x10, value +0x14}).
// Caller: CGameScriptInterface::AddLineToConversation (landed) through the world's conversation manager.
// Layout (PDB): Conversations (map) +0, NextConversationIndex, AlwaysShowText; the retail STLport map is 12 bytes. The
// engine header CScriptConversation.h gives the value type (only its address is used here).
// Shape: `iterator it = find(conversation)` constructs straight into the frame slot, `end()` is the header node.
#include "engine/CScriptConversation.h"

class CThing;
class CCharString;

// The conversation itself: the engine header's layout plus the two members this forwarder calls.
struct CScriptConversationCalls : CScriptConversation {
    void AddPersonToConversation(CThing* person);                                    // 0x006E5790
    void AddLineToConversation(CThing* speaker, CThing* listener, const CCharString* line,
                               bool playDuringCutScenes);                            // 0x006E58A0
};

struct SConversationNode {                     // STLport _Rb_tree_node<pair<const long, CScriptConversation> >
    long                     _M_color;         // +0x00
    SConversationNode*       _M_parent;        // +0x04
    SConversationNode*       _M_left;          // +0x08
    SConversationNode*       _M_right;         // +0x0c
    long                     first;            // +0x10
    CScriptConversationCalls second;           // +0x14
};
struct CConversationMap {                      // STLport map<long, CScriptConversation>: header node, count, comparator
    struct iterator {
        SConversationNode* _M_node;
        iterator() {}
        iterator(SConversationNode* node) : _M_node(node) {}
        SConversationNode* operator->() const { return _M_node; }
        bool operator!=(const iterator& other) const { return _M_node != other._M_node; }
    };
    SConversationNode* _M_header;              // +0x00
    unsigned long      _M_node_count;          // +0x04
    unsigned long      _M_key_compare;         // +0x08
    iterator find(const long& key);            // 0x006E6990
    iterator end() { return iterator(_M_header); }
};

class CScriptConversationManager { public:
    CConversationMap Conversations;            // +0x00
    long             NextConversationIndex;    // +0x0c
    bool             AlwaysShowText;           // +0x10
    void AddPersonToConversation(long conversation, CThing* person);
    void AddLineToConversation(long conversation, CThing* speaker, CThing* listener, const CCharString* line,
                               bool playDuringCutScenes);
};

void CScriptConversationManager::AddLineToConversation(long conversation, CThing* speaker, CThing* listener,
                                                       const CCharString* line, bool playDuringCutScenes)
{
    CConversationMap::iterator it = Conversations.find(conversation);
    if (it != Conversations.end())
        it->second.AddLineToConversation(speaker, listener, line, playDuringCutScenes);
}