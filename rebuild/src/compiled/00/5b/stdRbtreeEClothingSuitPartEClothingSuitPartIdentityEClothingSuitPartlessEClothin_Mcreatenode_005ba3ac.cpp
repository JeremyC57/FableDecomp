// std::_Rb_tree<EClothingSuitPart, EClothingSuitPart, _Identity<EClothingSuitPart>, less<EClothingSuitPart>, allocator<EClothingSuitPart> >::_M_create_node(const EClothingSuitPart& x) -- malloc a 0x14-byte tree node and placement-copy the suit part into its value field (the links are set by the caller)
// Leaf: ?_M_create_node@?$_Rb_tree@W4EClothingSuitPart@@W41@U?$_Identity@W4EClothingSuitPart@@@std@@U?$less@W4EClothingSuitPart@@@3@V?$allocator@W4EClothingSuitPart@@@3@@std@@QAEPAU?$_Rb_tree_node@W4EClothingSuitPart@@@2@ABW4EClothingSuitPart@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005BA3AC, 26 bytes, thiscall `ret 4` (`this` unused), size TU.
// Identity (sweep1, 2026-10-25): both callers (0x005BA8F7, 0x005BA910) are the two arms of the set<EClothingSuitPart>
//   tree's _M_insert 0x005BA8CF (whose only caller is insert_unique 0x005BAD5B, FINDINGS chunk 454); the 0x14-byte
//   node is the 0x10-byte _Rb_tree_node_base plus the 4-byte enum value at +0x10, and STLport's malloc allocator
//   (malloc 0x00BFEA0E) gives `push 14h ; call malloc ; pop ecx` with the null-tested placement copy of _Construct.
//   The catalog name CRetailList_node0x14_dword::Buynode is a generated stand-in shared by ten bodies of this shape.
//   Access: public (QAE), as every _Rb_tree::_M_create_node reloc binding in the tree spells it.
#pragma optimize("s", on)
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
inline void* operator new(unsigned int, void* __p) { return __p; }

enum EClothingSuitPart { CLOTHING_SUIT_PART_NONE };

namespace std {
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> class allocator { };
template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
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
    bool operator==(const _Rb_tree_base_iterator& __y) const { return _M_node == __y._M_node; }
};
template <class _Tp> struct _Nonconst_traits { };
template <class _Tp> struct _Const_traits { };
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    _Rb_tree_iterator(_Rb_tree_node<_Value>* __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Self& __it) { _M_node = __it._M_node; }
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Value value_type;
    typedef _Rb_tree_iterator<value_type, _Nonconst_traits<value_type> > iterator;

    _STLP_alloc_proxy<_Link_type, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                    // +0x04
    _Compare     _M_key_compare;                                   // +0x08

    _Link_type& _M_root() const { return (_Link_type&)_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)_M_header._M_data->_M_left; }
    static _Link_type& _S_left(_Base_ptr __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& _S_right(_Base_ptr __x) { return (_Link_type&)(__x->_M_right); }
    static const _Key& _S_key(_Base_ptr __x) { return _KeyOfValue()(((_Link_type)__x)->_M_value_field); }

    _Link_type _M_allocate_node() { return (_Link_type)malloc(sizeof(_Node)); }
    _Link_type _M_create_node(const value_type& __x);
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_Link_type
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_create_node(const value_type& __x)
{
    _Link_type __tmp = _M_allocate_node();
    new (&__tmp->_M_value_field) value_type(__x);
    return __tmp;
}

template class _Rb_tree<EClothingSuitPart, EClothingSuitPart, _Identity<EClothingSuitPart>, less<EClothingSuitPart>, allocator<EClothingSuitPart> >;
}
