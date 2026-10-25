// CScriptConversationManager::RemoveConversation(long conversation, bool flag) -- clean up the conversation stored under that index and erase it from the map (nothing when it is missing)
// Grade: RELOCATION_MATCH (agent902, 2 grades this pass) -- the flat CConversationMap replaced by the real STLport 4.6 map<long, CScriptConversation> (_Rb_tree with the alloc-proxy header, _Rb_tree_iterator<V, _Nonconst_traits<V> > returned by the out-of-line _Rb_tree::find through the hidden result, inline erase(iterator) with _Rb_global<bool>::_Rebalance_for_erase, _Destroy and the null-tested deallocate); agent901's assigned form (`iterator it; it = find(...)`) still needed -- the initialised form gives 102v98.
// Leaf: ?RemoveConversation@CScriptConversationManager@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E5990, 98 bytes (112 with int3 padding), thiscall, `ret 8`.
// Identity: Ego_r.pdb name set `?RemoveConversation@CScriptConversationManager@@QAEXJ_N@Z`; the map find @ 0x006E6990 and
//   the conversation's OnCleanup(bool) @ 0x006E5690 are the ones the landed manager members (0x006E5800/0x006E5950)
//   and CScriptConversation::OnCleanup use. The catalog's "CScriptConversation_RemoveAndCleanup" has the wrong class.
// Callees: map find(const long&) @ 0x006E6990 (iterator through the hidden result), CScriptConversation::OnCleanup(bool)
//   @ 0x006E5690, STLport _Rb_global<bool>::_Rebalance_for_erase(z, root&, leftmost&, rightmost&) @ 0x0042A34E (__cdecl),
//   ~CScriptConversation @ 0x004AE0E0 (the pair's dtor on node+0x14), free @ 0x00BFEA14 (null-tested deallocate).
// Layout: STLport map<long, CScriptConversation> (12 bytes; Ego_r.pdb CScriptConversationManager::Conversations): header node +0, node count +4; node {color, parent, left,
//   right, key +0x10, value +0x14}.
// Shape: STLport map::erase(iterator) inline: rebalance, _Destroy(&node->_M_value_field), deallocate, --_M_node_count.
extern "C" void __cdecl free(void* block);          // 0x00BFEA14

class CScriptConversation {
public:
    unsigned char _data[0x2c];                          // retail STLport layout
    ~CScriptConversation();                             // 0x004AE0E0
    void OnCleanup(bool flag);                          // 0x006E5690
};

namespace _STL {
template <class _Tp> struct less { };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> struct _Const_traits { typedef const _Tp& reference; typedef const _Tp* pointer; };
template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; typedef _Tp* pointer; };

struct _Rb_tree_node_base {
    bool                _M_color;                       // +0x00
    _Rb_tree_node_base* _M_parent;                      // +0x04
    _Rb_tree_node_base* _M_left;                        // +0x08
    _Rb_tree_node_base* _M_right;                       // +0x0c
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                              // +0x10
};
template <class _Dummy> class _Rb_global {
public:
    static _Rb_tree_node_base* __cdecl _Rebalance_for_erase(_Rb_tree_node_base* __z, _Rb_tree_node_base*& __root,
                                                            _Rb_tree_node_base*& __leftmost,
                                                            _Rb_tree_node_base*& __rightmost);   // 0x0042A34E
};
typedef _Rb_global<bool> _Rb_global_inst;

struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};
template <class _Value, class _Traits>
struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    typedef typename _Traits::pointer pointer;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    _Rb_tree_iterator(const iterator& __it) { _M_node = __it._M_node; }
    pointer operator->() const { return &((_Link_type)_M_node)->_M_value_field; }
    bool operator==(const _Rb_tree_iterator& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _Rb_tree_iterator& __y) const { return _M_node != __y._M_node; }
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                       // +0x04
    _Compare _M_key_compare;                                          // +0x08
    iterator end() { return iterator(_M_header._M_data); }
    iterator find(const _Key& __k);                                   // 0x006E6990 (out of line)
    void erase(iterator __position)
    {
        _Link_type __y = (_Link_type)_Rb_global_inst::_Rebalance_for_erase(__position._M_node,
                                                                           this->_M_header._M_data->_M_parent,
                                                                           this->_M_header._M_data->_M_left,
                                                                           this->_M_header._M_data->_M_right);
        _Destroy(&__y->_M_value_field);
        this->_M_header.deallocate(__y, 1);
        --_M_node_count;
    }
};
template <class _Pair> struct _Select1st { };

template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef _Key key_type;
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator end() { return _M_t.end(); }
    iterator find(const key_type& __x) { return _M_t.find(__x); }
    void erase(iterator __position) { _M_t.erase(__position); }
};
}

class CScriptConversationManager {
public:
    _STL::map<long, CScriptConversation> Conversations; // +0x00
    long             NextConversationIndex;             // +0x0c
    bool             AlwaysShowText;                    // +0x10
    void RemoveConversation(long conversation, bool flag);
};

void CScriptConversationManager::RemoveConversation(long conversation, bool flag)
{
    _STL::map<long, CScriptConversation>::iterator it;
    it = Conversations.find(conversation);
    if (it != Conversations.end()) {
        it->second.OnCleanup(flag);
        Conversations.erase(it);
    }
}