// CScriptConversationManager::RemoveConversation(long conversation, bool flag) -- clean up the conversation stored under that index and erase it from the map (nothing when it is missing)
// Grade: DIFFER(102v98) -- retail copies the found node into esi before the end() test and keeps it across OnCleanup (this in edi); ours keeps the iterator in its frame slot (find's hidden result) and reloads it after the OnCleanup call, pushing esi late
// Leaf: ?RemoveConversation@CScriptConversationManager@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E5990, 98 bytes (112 with int3 padding), thiscall, `ret 8`.
// Identity: Ego_r.pdb name set `?RemoveConversation@CScriptConversationManager@@QAEXJ_N@Z`; the map find @ 0x006E6990 and
//   the conversation's OnCleanup(bool) @ 0x006E5690 are the ones the landed manager members (0x006E5800/0x006E5950)
//   and CScriptConversation::OnCleanup use. The catalog's "CScriptConversation_RemoveAndCleanup" has the wrong class.
// Callees: map find(const long&) @ 0x006E6990 (iterator through the hidden result), CScriptConversation::OnCleanup(bool)
//   @ 0x006E5690, STLport _Rb_global<bool>::_Rebalance_for_erase(z, root&, leftmost&, rightmost&) @ 0x0042A34E (__cdecl),
//   ~CScriptConversation @ 0x004AE0E0 (the pair's dtor on node+0x14), free @ 0x00BFEA14 (null-tested deallocate).
// Layout: STLport map<long, CScriptConversation> (12 bytes): header node +0, node count +4; node {color, parent, left,
//   right, key +0x10, value +0x14}.
// Shape: STLport map::erase(iterator) inline: rebalance, _Destroy(&node->_M_value_field), deallocate, --_M_node_count.
extern "C" void __cdecl free(void* block);

struct _Rb_tree_node_base {
    long                _M_color;                       // +0x00
    _Rb_tree_node_base* _M_parent;                      // +0x04
    _Rb_tree_node_base* _M_left;                        // +0x08
    _Rb_tree_node_base* _M_right;                       // +0x0c
};
_Rb_tree_node_base* __cdecl _Rebalance_for_erase(_Rb_tree_node_base* __z, _Rb_tree_node_base*& __root,
                                                 _Rb_tree_node_base*& __leftmost,
                                                 _Rb_tree_node_base*& __rightmost);   // 0x0042A34E

class CScriptConversation {
public:
    unsigned char _data[0x2c];                          // retail STLport layout
    ~CScriptConversation();                             // 0x004AE0E0
    void OnCleanup(bool flag);                          // 0x006E5690
};

struct SConversationPair {
    long                first;                          // +0x10 in the node
    CScriptConversation second;                         // +0x14 in the node
};
struct SConversationNode : _Rb_tree_node_base {
    SConversationPair _M_value_field;                   // +0x10
};

struct CConversationMap {                               // STLport map<long, CScriptConversation>
    struct iterator {
        SConversationNode* _M_node;
        iterator() { }
        iterator(SConversationNode* node) : _M_node(node) { }
        SConversationPair* operator->() const { return &_M_node->_M_value_field; }
        bool operator!=(const iterator& other) const { return _M_node != other._M_node; }
    };
    SConversationNode* _M_header;                       // +0x00
    unsigned long      _M_node_count;                   // +0x04
    unsigned long      _M_key_compare;                  // +0x08

    iterator find(const long& key);                     // 0x006E6990
    iterator end() { return iterator(_M_header); }
    void erase(iterator __position)
    {
        SConversationNode* __y = (SConversationNode*)_Rebalance_for_erase(__position._M_node, _M_header->_M_parent,
                                                                          _M_header->_M_left, _M_header->_M_right);
        __y->_M_value_field.~SConversationPair();
        if (__y)
            free(__y);
        --_M_node_count;
    }
};

class CScriptConversationManager {
public:
    CConversationMap Conversations;                     // +0x00
    long             NextConversationIndex;             // +0x0c
    bool             AlwaysShowText;                    // +0x10
    void RemoveConversation(long conversation, bool flag);
};

void CScriptConversationManager::RemoveConversation(long conversation, bool flag)
{
    CConversationMap::iterator it = Conversations.find(conversation);
    if (it != Conversations.end()) {
        it->second.OnCleanup(flag);
        Conversations.erase(it);
    }
}
