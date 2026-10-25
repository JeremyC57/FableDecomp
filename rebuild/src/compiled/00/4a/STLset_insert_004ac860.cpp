// _STL::set::insert(const value_type& x) -- insert a string into a CCharString set; return the (const) iterator and whether it was new
// Leaf: ?insert@?$set@VCCharString@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004AC860, 40 bytes, thiscall, `ret 8` (hidden result pointer + value).
// Identity: set<CCharString>: the callee 0x004AC380 (manifest "StdSet_Insert") is _Rb_tree::insert_unique, whose key
//   compare is the inline CCharString `<` (data pointers equal / null tests, then CBasicString<char>::operator< core
//   0x00429950) on the node value at +0x10 (STLport node: color, parent, left, right, value). The only caller is
//   RunCutsceneMacro_Func (0x00CC669B). Catalog "vector<EWeaponClass>::_Umove<EWeaponClass*>" is propagated.
// Callees: _Rb_tree::insert_unique(const value_type&) @ 0x004AC380 (out of line; returns pair<iterator, bool> through a
//   hidden pointer to the 8-byte frame temporary). `this` stays in ecx: the tree is the set's only member.
// Shape: STLport 4.6 `set::insert` (as landed 0x005BAF94, here a speed TU): `pair<_Rep_iterator, bool> __p =
//   _M_t.insert_unique(__x); return pair<iterator, bool>(__p.first, __p.second);` -- the conversion to the set's const
//   iterator copies the node pointer and then the bool byte into the caller's result. A map::insert would forward the
//   hidden pointer instead (same iterator type, no copy).
class CCharStringData;
class CCharString {                                      // 4 bytes: the shared string data pointer
public:
    CCharStringData* PData;
};
bool operator<(const CCharString& a, const CCharString& b);   // CBasicString<char>::operator< (0x00429950 core)
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
    pair<iterator, bool> insert_unique(const _Value& __x);   // out of line (0x004AC380)
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

template class _STL::set<CCharString>;