// std::_Rb_tree::_M_upper_bound(const key_type& k) const -- (map<NUISystem::CComponent*, NUISystem::CComponent* >) STLport upper bound: the first node whose key is greater than k, the header when none; the key compare (an unsigned 4-byte less) is inline
// Leaf: ?_M_upper_bound@?$_Rb_tree@PAVCComponent@NUISystem@@U?$pair@QAVCComponent@NUISystem@@PAV12@@std@@U?$_Select1st@U?$pair@QAVCComponent@NUISystem@@PAV12@@std@@@4@U?$less@PAVCComponent@NUISystem@@@4@V?$allocator@U?$pair@QAVCComponent@NUISystem@@PAV12@@std@@@4@@std@@QBEPAU_Rb_tree_node_base@2@ABQAVCComponent@NUISystem@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042AEB5, 37 bytes, thiscall `ret 4`, the node pointer in eax (a size-optimised /Os module: unaligned entry).
// Identity (proven): std::_Rb_tree<NUISystem::CComponent*, pair<NUISystem::CComponent* const, NUISystem::CComponent*>, _Select1st<...>, less<NUISystem::CComponent*>, allocator<...> >::_M_upper_bound(const key_type&) const; evidence: its callers are the landed equal_range 0x0042C058 (0x0042C060, `pair(lower_bound(k), upper_bound(k))`, upper first) and the upper_bound(k) wrapper 0x0042B8E8 (0x0042B8EC) of the tree behind CTCInventoryTrade::ItemBuyLists / ItemSellLists (Ego_r.pdb map<NUISystem::CComponent*, NUISystem::CComponent*>, per that landed equal_range: erase(const key&) 0x0042CDE0 <- the thunks on this+0x20C / this+0x200)
// Label: the manifest's "BinarySearchTree_FindByUintComparator" is a descriptive placeholder (family J043: five byte-identical upper bounds over 4-byte unsigned keys,
//   each its own COMDAT; retail has no identical-code folding, so each address is one tree type).
// Callees: none (less<NUISystem::CComponent*> inline: `cmp k, key; jae right`).
// Shape: STLport 4.6 `y = header; x = root; while (x) if (comp(k, key(x))) y = x, x = left(x); else x = right(x); return y;`;
//   the key is loaded once into edx, node key at +0x10.
#pragma optimize("s", on)                                 // size-optimised TU (unaligned start)
#include <stddef.h>
namespace NUISystem { class CComponent; class CList; }
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
    _T1 first;
    _T2 second;
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
struct _Rb_tree_node_base {
    bool                _M_color;                          // +0x00
    _Rb_tree_node_base* _M_parent;                         // +0x04
    _Rb_tree_node_base* _M_left;                           // +0x08
    _Rb_tree_node_base* _M_right;                          // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                 // +0x10
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    _STLP_alloc_proxy<_Rb_tree_node<_Tp>*, _Alloc> _M_header;   // +0x00
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value>* _Link_type;
    unsigned int _M_node_count;                            // +0x04
    _Compare     _M_key_compare;                           // +0x08
    _Link_type _M_root() const { return (_Link_type)this->_M_header._M_data->_M_parent; }
    static _Link_type _S_left(_Base_ptr __x) { return (_Link_type)__x->_M_left; }
    static _Link_type _S_right(_Base_ptr __x) { return (_Link_type)__x->_M_right; }
    static _Value& _S_value(_Base_ptr __x) { return ((_Link_type)__x)->_M_value_field; }
    static const _Key& _S_key(_Base_ptr __x) { return _KeyOfValue()(_S_value(__x)); }
    _Base_ptr _M_upper_bound(const _Key& __k) const;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
_Rb_tree_node_base* _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_upper_bound(const _Key& __k) const
{
    _Link_type __y = this->_M_header._M_data;              // last node which is greater than __k
    _Link_type __x = _M_root();                            // current node
    while (__x != 0)
        if (_M_key_compare(__k, _S_key(__x)))
            __y = __x, __x = _S_left(__x);
        else
            __x = _S_right(__x);
    return __y;
}
typedef NUISystem::CComponent* _MapKey;
typedef pair<const _MapKey, NUISystem::CComponent* > _MapValue;
typedef _Rb_tree<_MapKey, _MapValue, _Select1st<_MapValue>, less<_MapKey>, allocator<_MapValue> > _MapTree;
template _Rb_tree_node_base* _MapTree::_M_upper_bound(const _MapKey&) const;
} // namespace std