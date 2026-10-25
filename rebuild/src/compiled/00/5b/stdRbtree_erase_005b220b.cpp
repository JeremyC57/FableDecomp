// std::_Rb_tree::erase(iterator first, iterator last) -- (map<NUISystem::CComponent*, pair<long, long> >) STLport range erase: clear() when the range is the whole tree (first == leftmost, last == header), else erase(first++) node by node
// Leaf: ?erase@?$_Rb_tree@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005B220B, 68 bytes (unaligned entry, size-optimised TU with an ebp frame),
//   thiscall, `ret 8` (two 4-byte iterators by value).
// Callees: clear() @ 0x005B1C18 and erase(iterator) @ 0x005B1D09 (this tree's own out-of-line COMDATs),
//   _Rb_global<bool>::_M_increment @ 0x004292C0 (static __cdecl successor step of the post-increment).
// Caller: the tree's erase(const key_type&) @ 0x005B2779 (equal_range, distance, erase(first, last), return the count).
// Identity: map<NUISystem::CComponent*, pair<long, long> > -- proven; evidence: its erase(const key&) 0x005B2779 is called on CTCInventoryTrade this+0x150 (CTCInventoryTrade::SellItemPC 0x005AF10C / 0x005B0621), ItemsToSell = map<NUISystem::CComponent*, pair<long, long> > (struct_layouts_egor +0x178; retail +0x150 as the landed CTCInventoryTrade::AddSellItemToList 0x0059ED42 / RefreshDescriptions 0x005AE6AF model it).
// Label: the manifest name "Container_EraseRange_RB" is propagated across byte-identical bodies; this is the STLport 4.6
//   `_Rb_tree::erase(iterator, iterator)` template.
// Shape: the iterator argument for erase(iterator) is built in its slot by the iterator's user copy constructor
//   (`push ecx ; mov edi,esp`, stored after the increment returns); clear() and erase(iterator) are declared only, so
//   they stay calls as in retail.
#pragma optimize("s", on)
namespace NUISystem { class CComponent; class CList; }
namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
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
template <class _Tp> struct less { };
template <class _Pair> struct _Select1st { };
template <class _Tp> struct _Identity { };
template <class _Tp> struct _Nonconst_traits { };

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;
    _Base_ptr _M_parent;
    _Base_ptr _M_left;
    _Base_ptr _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static _Base_ptr __cdecl _M_increment(_Base_ptr);          // 0x004292C0
};
typedef _Rb_global<bool> _Rb_global_inst;

struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
    bool operator==(const _Rb_tree_base_iterator& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _Rb_tree_base_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    _Rb_tree_iterator() { _M_node = 0; }
    _Rb_tree_iterator(_Rb_tree_node<_Value>* __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Self& __it) { _M_node = __it._M_node; }
    _Self& operator++() { _M_node = _Rb_global_inst::_M_increment(_M_node); return *this; }
    _Self operator++(int)
    {
        _Self __tmp = *this;
        _M_node = _Rb_global_inst::_M_increment(_M_node);
        return __tmp;
    }
};
template <class _Tp, class _Alloc> class _Rb_tree_base {
public:
    typedef _Rb_tree_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
    _Base_ptr& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    void clear();                                      // 0x005B1C18 (own COMDAT)
    void erase(iterator __position);                   // 0x005B1D09 (own COMDAT)
    void erase(iterator __first, iterator __last);
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::erase(iterator __first, iterator __last)
{
    if (__first._M_node == _M_leftmost() && __last._M_node == this->_M_header._M_data)
        clear();
    else
        while (__first != __last) erase(__first++);
}
typedef NUISystem::CComponent* _TreeKey;
typedef pair<NUISystem::CComponent* const, pair<long, long> > _TreeValue;
template void _Rb_tree<_TreeKey, _TreeValue, _Select1st<_TreeValue>, less<_TreeKey> >::erase(iterator, iterator);
}