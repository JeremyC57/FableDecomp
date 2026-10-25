// std::_Rb_tree::equal_range(const key_type& k) -- (map<NUISystem::CComponent*, long>) STLport equal_range: the [lower_bound(k), upper_bound(k)) iterator pair from the out-of-line node searches, returned through the hidden result
// Leaf: ?equal_range@?$_Rb_tree@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005CC2D6, 42 bytes, thiscall, `ret 8` (hidden result, key), returns the result.
// Identity: map<NUISystem::CComponent*, long> -- proven; it is called by the tree's erase(const key&) 0x005CC31B (`pair p = equal_range(k);
//   n = distance(p.first, p.second); erase(p.first, p.second)`), whose erase(first, last) 0x005CC292 is this chunk's G014 row
//   (same tree, same TU; that row's NOTES entry carries the member evidence: its erase(const key&) 0x005CC31B is called on CTCInventoryWeapons this+0x170 (RefreshWeaponDescriptions 0x005CAFC3), WeaponMenuItemMap = map<NUISystem::CComponent*, long> (struct_layouts_egor +0x18C)).
// Label: the manifest's "operator()" is propagated across byte-identical bodies.
// Callees: _M_upper_bound(k) @ 0x005CBF72 (first: VC7.1 evaluates the pair constructor's arguments right to left; node in esi
//   -> second) and _M_lower_bound(k) @ 0x0042AD91 (-> first), both `const`, node pointer in eax.
//   The lower bound sits in another TU (0x0042AD91, the CTCInventoryTrade-side copy of the same map<NUISystem::CComponent*,
//   long> instantiation: identical-name COMDATs merge, the catalog binds it as that map's _M_lower_bound).
// Body: STLport 4.6 `pair<iterator, iterator> equal_range(const key_type& __x) { return pair<iterator, iterator>(
//   lower_bound(__x), upper_bound(__x)); }` with `lower_bound(k) = iterator(_M_lower_bound(k))` and the upper twin; the pair
//   has a user constructor, so it is returned through the hidden pointer.
// Shape: a size-optimised TU (the tree's functions sit back to back at odd addresses): `optimize("s", on)`.
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
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
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
struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
};
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    _Rb_tree_iterator() { _M_node = 0; }
    _Rb_tree_iterator(_Base_ptr __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Self& __it) { _M_node = __it._M_node; }
};
template <class _Tp, class _Alloc> class _Rb_tree_base {
public:
    typedef _Rb_tree_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Key key_type;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
    _Rb_tree_node_base* _M_lower_bound(const key_type& __k) const;   // 0x0042AD91 (own COMDAT)
    _Rb_tree_node_base* _M_upper_bound(const key_type& __k) const;   // 0x005CBF72 (own COMDAT)
    iterator lower_bound(const key_type& __x) { return iterator(_M_lower_bound(__x)); }
    iterator upper_bound(const key_type& __x) { return iterator(_M_upper_bound(__x)); }
    pair<iterator, iterator> equal_range(const key_type& __x)
    {
        return pair<iterator, iterator>(lower_bound(__x), upper_bound(__x));
    }
};
typedef NUISystem::CComponent* _TreeKey;
typedef pair<NUISystem::CComponent* const, long> _TreeValue;
template pair<_Rb_tree<_TreeKey, _TreeValue, _Select1st<_TreeValue>, less<_TreeKey> >::iterator,
              _Rb_tree<_TreeKey, _TreeValue, _Select1st<_TreeValue>, less<_TreeKey> >::iterator>
    _Rb_tree<_TreeKey, _TreeValue, _Select1st<_TreeValue>, less<_TreeKey> >::equal_range(const _TreeKey&);
}