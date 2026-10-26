// std::_Rb_tree<unsigned long, pair<const unsigned long, long>, _Select1st<...>, less<unsigned long>, allocator<...> >
//   ::_M_lower_bound(const unsigned long& k) const -- STLport: walk down from the root keeping the last node whose key is
//   not less than k (the header when none)
// Leaf: ?_M_lower_bound@?$_Rb_tree@KU?$pair@$$CBKJ@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005D84F2, 35 bytes, thiscall `ret 4` (the key by reference; returns the node).
// Identity: the tree of CVertexGroupBlock::GroupToIndexMap (Ego_r.pdb: map<unsigned long, long>). Its equal_range
//   0x005D8676 calls 0x005D8446 then this (right-to-left evaluation of `pair(lower_bound(k), upper_bound(k))`), and that
//   equal_range is called by the landed CVertexGroupBlock::RemoveLast 0x00AD9470 with Groups.back() as the key; the other
//   callers are the tree's two lower_bound(k) wrappers 0x005D8662 / 0x005D86D1 (`iterator(_M_lower_bound(k))` through a
//   hidden pointer, `ret 8`) and one more call at 0x005D8940. The
//   compare is unsigned (`cmp [node+10h], key ; jb` = key(x) < k goes right). The byte-identical 0x0042AE90 is the
//   map<NUISystem::CComponent*, NUISystem::CComponent*> instance (CTCInventoryTrade); the reloc table bound this tree's
//   name there. Catalog "CMemoryAllocatorVariableSize_FindNodeLinear" (manifest "_global") is descriptive and wrong.
// Shape: STLport 4.6 `_M_lower_bound` returning _Rb_tree_node_base*; size TU (unaligned entry), no frame.
#pragma optimize("s", on)

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};

template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;                                      // +0x00
    _T2 second;                                     // +0x04
};

template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

struct _Rb_tree_node_base {
    bool                _M_color;                   // +0x00
    _Rb_tree_node_base* _M_parent;                  // +0x04
    _Rb_tree_node_base* _M_left;                    // +0x08
    _Rb_tree_node_base* _M_right;                   // +0x0C
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                          // +0x10
};

template <class _Tp, class _Alloc> struct _Rb_tree_base {
    _STLP_alloc_proxy<_Rb_tree_node<_Tp>*, _Alloc> _M_header;      // +0x00
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value>* _Link_type;
    unsigned int _M_node_count;                     // +0x04
    _Compare     _M_key_compare;                    // +0x08
    _Base_ptr _M_root() const { return this->_M_header._M_data->_M_parent; }
    static const _Key& _S_key(_Base_ptr __x) { return _KeyOfValue()(((_Link_type)__x)->_M_value_field); }
    _Base_ptr _M_lower_bound(const _Key& __k) const;
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
_Rb_tree_node_base* _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_lower_bound(const _Key& __k) const
{
    _Base_ptr __y = this->_M_header._M_data;        // last node which is not less than __k
    _Base_ptr __x = _M_root();                      // current node
    while (__x != 0)
        if (!_M_key_compare(_S_key(__x), __k))
            __y = __x, __x = __x->_M_left;
        else
            __x = __x->_M_right;
    return __y;
}

typedef pair<const unsigned long, long> _ULongLongPair;
template _Rb_tree_node_base* _Rb_tree<unsigned long, _ULongLongPair, _Select1st<_ULongLongPair>, less<unsigned long>,
                                      allocator<_ULongLongPair> >::_M_lower_bound(const unsigned long&) const;

} // namespace std
