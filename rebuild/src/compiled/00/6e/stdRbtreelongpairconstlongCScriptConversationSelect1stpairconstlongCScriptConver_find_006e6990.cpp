// std::_Rb_tree<long, pair<const long, CScriptConversation>, _Select1st<pair<const long, CScriptConversation> >, less<long>, allocator<pair<const long, CScriptConversation> > >::find(const long& k) -- the script conversation map's lookup: lower-bound walk from the root, end() unless the found node's id equals k
// Leaf: ?find@?$_Rb_tree@JU?$pair@$$CBJVCScriptConversation@@@std@@U?$_Select1st@U?$pair@$$CBJVCScriptConversation@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJVCScriptConversation@@@std@@@2@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBJVCScriptConversation@@@std@@U?$_Nonconst_traits@U?$pair@$$CBJVCScriptConversation@@@std@@@2@@2@ABJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E6990, 71 bytes, thiscall, `ret 8` (hidden iterator result + key reference).
// Identity (sweep1, 2026-10-25): FINDINGS (near-miss pass 2, hand902): "CRetailTreelongkey_find_006e6990 ... use invented
//   classes (STLport ... _Rb_tree::find)". Its four callers are CScriptConversationManager members: the landed
//   AddPersonToConversation 0x006E5800, AddLineToConversation 0x006E5950 and RemoveConversation 0x006E5990 (whose reloc
//   binding already names this Leaf -> 006e6990) and 0x006E6A90; Ego_r.pdb CScriptConversationManager::Conversations +0 is
//   map<long, CScriptConversation>. The key compares signed at node +0x10 (`jl` / `jge`). The catalog name
//   CRetailTree_longkey::find is a generated stand-in shared by 27 bodies of this shape.
// Shape: STLport 4.6 `iterator find(const key_type& k) { return iterator(_M_find(k)); }` with _M_find inlined: walk
//   keeping the last node not less than k, then the header when nothing was found or k < key(y).
class CScriptConversation { int m_Opaque; };   // mapped value; find never reads it

namespace std {
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> class allocator { };
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    _T1 first;
    _T2 second;
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                                     // +0x00
    _Base_ptr _M_parent;                                    // +0x04
    _Base_ptr _M_left;                                      // +0x08
    _Base_ptr _M_right;                                     // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};

struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};
template <class _Tp> struct _Nonconst_traits { };
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_iterator(_Rb_tree_node<_Value>* __x) { _M_node = __x; }
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Key key_type;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;

    _STLP_alloc_proxy<_Link_type, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                    // +0x04
    _Compare     _M_key_compare;                                   // +0x08

    _Link_type& _M_root() const { return (_Link_type&)_M_header._M_data->_M_parent; }
    static _Link_type& _S_left(_Base_ptr __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& _S_right(_Base_ptr __x) { return (_Link_type&)(__x->_M_right); }
    static const _Key& _S_key(_Base_ptr __x) { return _KeyOfValue()(((_Link_type)__x)->_M_value_field); }

    _Link_type _M_find(const key_type& __k) const;
    iterator find(const key_type& __x) { return iterator(_M_find(__x)); }
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_Link_type
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_find(const key_type& __k) const
{
    _Link_type __y = _M_header._M_data;                 // last node which is not less than k
    _Link_type __x = _M_root();                         // current node
    while (__x != 0)
        if (!_M_key_compare(_S_key(__x), __k))
            __y = __x, __x = _S_left(__x);
        else
            __x = _S_right(__x);
    if (__y == _M_header._M_data || _M_key_compare(__k, _S_key(__y)))
        __y = _M_header._M_data;
    return __y;
}

typedef pair<const long, CScriptConversation> _ConversationValue;
template _Rb_tree<long, _ConversationValue, _Select1st<_ConversationValue>, less<long>, allocator<_ConversationValue> >::iterator
_Rb_tree<long, _ConversationValue, _Select1st<_ConversationValue>, less<long>, allocator<_ConversationValue> >::find(const long&);
}
