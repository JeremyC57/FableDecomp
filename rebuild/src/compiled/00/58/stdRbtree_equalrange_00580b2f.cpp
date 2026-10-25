// std::_Rb_tree::equal_range(const key_type& k) -- (map<void*, float>) STLport equal_range: the [lower_bound(k), upper_bound(k)) iterator pair from the out-of-line node searches, returned through the hidden result
// Leaf: ?equal_range@?$_Rb_tree@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00580B2F, 42 bytes, thiscall, `ret 8` (hidden result, key), returns the result.
// Identity: map<void*, float> -- proven; it is called by the tree's erase(const key&) 0x0058119B (`pair p = equal_range(k);
//   n = distance(p.first, p.second); erase(p.first, p.second)`), whose erase(first, last) 0x00580658 is this chunk's G014 row
//   (same tree, same TU; that row's NOTES entry carries the member evidence: its erase(const key&) 0x0058119B is the map erase that the landed CTCHaste::UnsetActionSpeedMultiplier(void*) 0x0058148D (Ego_r.pdb name) applies to this+0x18, Ego_r.pdb CTCHaste::Multipliers map<void*, float>).
// Label: the manifest's "operator()" is propagated across byte-identical bodies.
// Callees: _M_upper_bound(k) @ 0x0057FED6 (first: VC7.1 evaluates the pair constructor's arguments right to left; node in esi
//   -> second) and _M_lower_bound(k) @ 0x0057FEB1 (-> first), both `const`, node pointer in eax.
// Body: STLport 4.6 `pair<iterator, iterator> equal_range(const key_type& __x) { return pair<iterator, iterator>(
//   lower_bound(__x), upper_bound(__x)); }` with `lower_bound(k) = iterator(_M_lower_bound(k))` and the upper twin; the pair
//   has a user constructor, so it is returned through the hidden pointer.
// Shape: a size-optimised TU (the tree's functions sit back to back at odd addresses): `optimize("s", on)`.
#pragma optimize("s", on)

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
    _Rb_tree_node_base* _M_lower_bound(const key_type& __k) const;   // 0x0057FEB1 (own COMDAT)
    _Rb_tree_node_base* _M_upper_bound(const key_type& __k) const;   // 0x0057FED6 (own COMDAT)
    iterator lower_bound(const key_type& __x) { return iterator(_M_lower_bound(__x)); }
    iterator upper_bound(const key_type& __x) { return iterator(_M_upper_bound(__x)); }
    pair<iterator, iterator> equal_range(const key_type& __x)
    {
        return pair<iterator, iterator>(lower_bound(__x), upper_bound(__x));
    }
};
typedef void* _TreeKey;
typedef pair<void* const, float> _TreeValue;
template pair<_Rb_tree<_TreeKey, _TreeValue, _Select1st<_TreeValue>, less<_TreeKey> >::iterator,
              _Rb_tree<_TreeKey, _TreeValue, _Select1st<_TreeValue>, less<_TreeKey> >::iterator>
    _Rb_tree<_TreeKey, _TreeValue, _Select1st<_TreeValue>, less<_TreeKey> >::equal_range(const _TreeKey&);
}