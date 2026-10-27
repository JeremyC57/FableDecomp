// std::_Rb_tree<CCharString, std::pair<const CCharString, bool>, ...>::equal_range(const key_type& k) -- STLport: the [lower_bound(k), upper_bound(k)) iterator pair
// Leaf: ?equal_range@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@_N@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@_N@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@_N@std@@@3@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@_N@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@_N@std@@@2@@std@@U12@@2@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007adbc0, 42 bytes, thiscall, pair result through the hidden pointer, `ret 8`.
// Identity (proven): its callees are bound ?_M_upper_bound@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@_N@std@@... 0x007ACD30 and ?_M_lower_bound@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@_N@std@@... 0x007ACBB0; no caller (unreferenced).
// Callees: _M_upper_bound(const key_type&) const 0x007ACD30 and _M_lower_bound(const key_type&) const 0x007ACBB0 (thiscall).
// Shape (family L057): `return pair<iterator, iterator>(lower_bound(k), upper_bound(k));` -- the upper bound is computed
//   first (right-to-left arguments) and kept in ebx.
class CCharString {
public:
    void* PStringData;                                    // +0x00
};
namespace std {
template <class _Tp> class allocator { };
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
template <class _Pair> struct _Select1st { };
struct _Rb_tree_node_base {
    bool _M_color;                                        // +0x00
    _Rb_tree_node_base* _M_parent;                        // +0x04
    _Rb_tree_node_base* _M_left;                          // +0x08
    _Rb_tree_node_base* _M_right;                         // +0x0C
};
template <class _Tp> struct _Nonconst_traits { };
template <class _Value, class _Traits> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) { }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) : _M_node(__it._M_node) { }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Key key_type;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    _Rb_tree_node_base* _M_header;                        // +0x00
    unsigned int _M_node_count;                           // +0x04
    _Compare _M_key_compare;                              // +0x08
    _Rb_tree_node_base* _M_lower_bound(const key_type& __k) const;   // 0x007ACBB0
    _Rb_tree_node_base* _M_upper_bound(const key_type& __k) const;   // 0x007ACD30
    iterator lower_bound(const key_type& __x) { return iterator(_M_lower_bound(__x)); }
    iterator upper_bound(const key_type& __x) { return iterator(_M_upper_bound(__x)); }
    pair<iterator, iterator> equal_range(const key_type& __x)
    {
        return pair<iterator, iterator>(lower_bound(__x), upper_bound(__x));
    }
};
}
typedef std::pair<const CCharString, bool > TValue;
typedef std::_Rb_tree<CCharString, TValue, std::_Select1st<TValue>, std::less<CCharString >, std::allocator<TValue> > TTree;
template std::pair<TTree::iterator, TTree::iterator> TTree::equal_range(const CCharString&);