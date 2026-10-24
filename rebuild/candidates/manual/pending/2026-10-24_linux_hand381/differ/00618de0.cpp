// std::map<long, pair<long, long> >::operator[](const long& k) -- STLport map subscript: lower-bound walk, insert (k, pair()) through insert_unique(hint, v) when the key is absent, return a reference to the mapped pair
// Leaf: ??A?$map@
// Grade: DIFFER(ours v retail) -- 123v123, scheduling only: the two `mov dword ptr [esp+18h/1Ch],0` stores of the
//   pair() mapped half sit right after `mov eax,esp` (before the iterator store `mov [eax],edx` and the hidden-result
//   `lea edx,[esp+24h] ; push edx`); retail sinks them below `push edx`, immediately before the call. Tried: pair
//   without a user copy constructor (119v123: zero register, key stored after the pushes), this form (the STLport
//   explicit pair copy constructor), a direct `_M_t.insert_unique` call (121v123), STLport's body-assigning
//   _Rb_tree_base_iterator constructors (123v123, same order).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00618DE0, 123 bytes (+5 int3), thiscall `ret 4`.
// Caller: CTCInventoryMap::CreateMapMarkers 0x005FF190 (call at 0x005FF7CA: `counts[key] = pair(1, 0)` on a miss of
//   its own inline find, `++it->second.first` on a hit) on a local map.
// Callee: _Rb_tree::insert_unique(iterator hint, const value_type& v) 0x00617D50 (iterator returned through the hidden
//   pointer; the catalog's `RBTree_Insert_WithSuccessor_Variant`).
// Label correction: the catalog `RBTree_FindAndInsert` is behavioural; FableWin.pdb and Ego_r.pdb both name
//   `??A?$map@JU?$pair@JJ@std@@...` (map<long, pair<long,long> >::operator[]); the body is STLport
//   `iterator i = lower_bound(k); if (i == end() || key_comp()(k, (*i).first)) i = insert(i, value_type(k, T()));
//   return (*i).second;`.
// Layout: the tree header pointer +0; node: color +0, parent +4, left +8, right +0xC, value (key +0x10, mapped pair
//   +0x14) -- the returned reference is node + 0x14.
// Shape: speed TU (no pragma); the lower-bound walk is the 0x00614A10 loop with the header in esi; on a miss the key is
//   re-read into the value_type local, the hint iterator is copy-constructed into its argument slot (`push ecx ;
//   mov eax,esp ; mov [eax],edx`), the result iterator lands in the dead k slot, and the pair() zero stores are sunk
//   below the pushes into the value_type's mapped half.
namespace std {

template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};

template <class T> struct allocator { };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair& __p) : first(__p.first), second(__p.second) { }
};

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                            // +0x00
    _Base_ptr _M_parent;                           // +0x04
    _Base_ptr _M_left;                             // +0x08
    _Base_ptr _M_right;                            // +0x0C
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                         // +0x10
};

template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node<_Value>* __x) : _M_node(__x) { }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) : _M_node(__it._M_node) { }
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    bool operator==(const _Rb_tree_iterator& __y) const { return _M_node == __y._M_node; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
struct _STLP_alloc_proxy : public _MaybeReboundAlloc {
    _Value _M_data;
};

template <class _Key, class _Value, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value> iterator;

    _STLP_alloc_proxy<_Link_type, _Rb_tree_node<_Value>, _Alloc> _M_header;   // +0x00
    unsigned int _M_node_count;                                              // +0x04
    _Compare _M_key_compare;                                                 // +0x08

    _Link_type _M_root() const { return (_Link_type)_M_header._M_data->_M_parent; }
    static _Link_type _S_left(_Link_type x) { return (_Link_type)x->_M_left; }
    static _Link_type _S_right(_Link_type x) { return (_Link_type)x->_M_right; }
    static const _Key& _S_key(_Link_type x) { return x->_M_value_field.first; }

    iterator end() { return iterator(_M_header._M_data); }
    _Compare key_comp() const { return _M_key_compare; }

    _Link_type _M_lower_bound(const _Key& __k) const
    {
        _Link_type __y = _M_header._M_data;        // Last node which is not less than __k.
        _Link_type __x = _M_root();                // Current node.
        while (__x != 0)
            if (!_M_key_compare(_S_key(__x), __k))
                __y = __x, __x = _S_left(__x);
            else
                __x = _S_right(__x);
        return __y;
    }
    iterator lower_bound(const _Key& __k) { return iterator(_M_lower_bound(__k)); }

    iterator insert_unique(iterator __position, const _Value& __v);          // 0x00617D50
};

template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef _Key key_type;
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;

    _Rep_type _M_t;

    iterator end() { return _M_t.end(); }
    _Compare key_comp() const { return _M_t.key_comp(); }
    iterator lower_bound(const key_type& __x) { return _M_t.lower_bound(__x); }
    iterator insert(iterator __position, const value_type& __x) { return _M_t.insert_unique(__position, __x); }

    _Tp& operator[](const key_type& __k);
};

template <class _Key, class _Tp, class _Compare, class _Alloc>
_Tp& map<_Key, _Tp, _Compare, _Alloc>::operator[](const key_type& __k)
{
    iterator __i = lower_bound(__k);
    // __i->first is greater than or equivalent to __k.
    if (__i == end() || key_comp()(__k, (*__i).first))
        __i = insert(__i, value_type(__k, _Tp()));
    return (*__i).second;
}

template pair<long, long>& map<long, pair<long, long> >::operator[](const long&);

}
