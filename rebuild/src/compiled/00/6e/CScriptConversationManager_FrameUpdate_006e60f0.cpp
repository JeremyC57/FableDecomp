// CScriptConversationManager::FrameUpdate() -- update every running conversation in index order, stepping the map iterator past each one before its update runs (an update may remove its own conversation)
// Leaf: ?FrameUpdate@CScriptConversationManager@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E60F0, 83 bytes (96 with int3 padding), thiscall, plain `ret`.
// Identity: Ego_r.pdb name set `?FrameUpdate@CScriptConversationManager@@QAEXXZ` (next to CScriptConversation::FrameUpdate);
//   the callee 0x006E5A00 is the per-conversation update on node+0x14 (the pair's value, as the landed manager members
//   use it). The catalog's "UpdateFrame_IterateConversations" is descriptive.
// Callees: CScriptConversation::FrameUpdate() @ 0x006E5A00.
// Layout: STLport map<long, CScriptConversation> at +0 (header node pointer; begin = header->_M_left +8); nodes
//   {color, parent +4, left +8, right +0xC, key +0x10, value +0x14}.
// Shape: the reference to the value is taken (`lea ecx,[esi+14h]`) before STLport's inline
//   _Rb_tree_base_iterator::_M_increment runs on esi, and the call comes after; end() (the header) is re-read after
//   each call.
struct _Rb_tree_node_base {
    long                _M_color;                       // +0x00
    _Rb_tree_node_base* _M_parent;                      // +0x04
    _Rb_tree_node_base* _M_left;                        // +0x08
    _Rb_tree_node_base* _M_right;                       // +0x0c
};

class CScriptConversation {
public:
    unsigned char _data[0x2c];                          // retail STLport layout
    void FrameUpdate();                                 // 0x006E5A00
};

struct SConversationPair {
    long                first;
    CScriptConversation second;
};
struct SConversationNode : _Rb_tree_node_base {
    SConversationPair _M_value_field;                   // +0x10
};

struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    void _M_increment()
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        }
        else {
            _Rb_tree_node_base* __y = _M_node->_M_parent;
            while (_M_node == __y->_M_right) {
                _M_node = __y;
                __y = __y->_M_parent;
            }
            if (_M_node->_M_right != __y)
                _M_node = __y;
        }
    }
};
struct _Rb_tree_iterator : _Rb_tree_base_iterator {
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    SConversationPair& operator*() const { return ((SConversationNode*)_M_node)->_M_value_field; }
    SConversationPair* operator->() const { return &operator*(); }
    _Rb_tree_iterator& operator++() { _M_increment(); return *this; }
};
inline bool operator!=(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y)
{
    return __x._M_node != __y._M_node;
}

struct CConversationMap {                               // STLport map<long, CScriptConversation>
    typedef _Rb_tree_iterator iterator;
    _Rb_tree_node_base* _M_header;                      // +0x00
    unsigned long       _M_node_count;                  // +0x04
    unsigned long       _M_key_compare;                 // +0x08
    iterator begin() { return iterator(_M_header->_M_left); }
    iterator end() { return iterator(_M_header); }
};

class CScriptConversationManager {
public:
    CConversationMap Conversations;                     // +0x00
    long             NextConversationIndex;             // +0x0c
    bool             AlwaysShowText;                    // +0x10
    void FrameUpdate();
};

void CScriptConversationManager::FrameUpdate()
{
    CConversationMap::iterator it = Conversations.begin();
    while (it != Conversations.end()) {
        CScriptConversation& conversation = it->second;
        ++it;
        conversation.FrameUpdate();
    }
}