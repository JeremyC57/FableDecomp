// _STL::set::insert(const value_type& x) -- insert a long into a set<long>; return the (const) iterator and whether it was new
// Leaf: ?insert@?$set@JU?$less@J@_STL@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00514230, 40 bytes, thiscall, `ret 8` (hidden result pointer + value).
// Identity: set<long>: the callee 0x00512DA0 (manifest "BinarySearchTree_Insert") is _Rb_tree::insert_unique with a
//   signed 4-byte key compare (`cmp edx,[esi+10h] ; setl al`). Callers: CTCVillage::ReportCrime (0x0065CE4D, 0x0065CF63:
//   inserts into the village's set at +0x64 -- Ego_r.pdb CTCVillage::PlayersCrimes is set<long>), CTCInventoryTrade::
//   ApplyTattooCard (0x005AD4C6, 0x005AD516) and CMiniMapDisplay::InitialiseRegionExitMarkers (0x0082B11F) -- one
//   ICF-folded body for every 4-byte signed-key set. Catalog "vector<EWeaponClass>::_Umove<EWeaponClass*>" is propagated.
// Callees: _Rb_tree::insert_unique(const value_type&) @ 0x00512DA0.
// Shape: STLport 4.6 `set::insert` (see 0x004AC860): insert_unique into the 8-byte frame temporary, then the node
//   pointer and the bool byte are copied into the caller's result.
namespace _STL {
template <class _Tp> struct less { bool operator()(const _Tp& a, const _Tp& b) const { return a < b; } };
template <class _Tp> class allocator { };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    template <class _U1, class _U2> pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) { }
};
struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;
    _Base_ptr _M_parent;
    _Base_ptr _M_left;
    _Base_ptr _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};
template <class _Tp> struct _Const_traits { };
template <class _Tp> struct _Nonconst_traits { };
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(const iterator& __it) { _M_node = __it._M_node; }
};
template <class _Value, class _Alloc> struct _Rb_tree_base {
    _Rb_tree_node<_Value>* _M_header;                    // +0x00 (alloc proxy data)
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    typedef _Rb_tree_iterator<_Value, _Const_traits<_Value> >    const_iterator;
    unsigned int _M_node_count;                          // +0x04
    _Compare     _M_key_compare;                         // +0x08
    pair<iterator, bool> insert_unique(const _Value& __x);   // out of line (0x00512DA0)
};
template <class _Tp> struct _Identity { };
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set {
public:
    typedef _Key value_type;
    typedef _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::const_iterator iterator;
    _Rep_type _M_t;                                      // 12 bytes
    pair<iterator, bool> insert(const value_type& __x) {
        typedef typename _Rep_type::iterator _Rep_iterator;
        pair<_Rep_iterator, bool> __p = _M_t.insert_unique(__x);
        return pair<iterator, bool>(__p.first, __p.second);
    }
};
}

template class _STL::set<long>;