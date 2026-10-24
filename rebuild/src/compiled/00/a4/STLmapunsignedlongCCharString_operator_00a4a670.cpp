// _STL::map<unsigned long, CCharString>::operator[](const unsigned long& k) -- find the entry for k (inline lower_bound walk); when absent insert (k, CCharString()) at the hint; return a reference to the mapped string
// Leaf: ??A?$map@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A4A670, length 147 bytes (+13 int3 pad), `ret 4`.
// Callers: 0x00A48C6E (CLipSyncSampleData::WriteToFile 0x00A48BA0), 0x00A496F7 / 0x00A499F7 (CLipSyncSampleData::ReadFromFile
//   0x00A495F0): the phoneme ID -> symbol map of the lip-sync sample files.
// Callees: CCharString default ctor 0x0099E4B0 / copy ctor 0x0099EC30 / dtor 0x0099EAE0, 0x00A49F60
//   _Rb_tree::insert_unique(iterator, const value_type&) (landed; key compared unsigned, string at node +0x14).
// Identity: catalogued as `BST_InsertEntry_EHeroMorphType` (propagated); the key compares are unsigned (`jb` / `jae`), as in
//   the landed insert_unique, so the key is an unsigned long (the phoneme ID).
// Shape: STLport 4.x `map::operator[]` verbatim: `lower_bound` inlined (header in ecx, root at header+4, y kept in esi),
//   `__i == end() || key_comp()(k, __i->first)`, then `insert(__i, value_type(k, __default_constructed((_Tp*)0)))`:
//   the by-value CCharString() result is homed in the dead `k` argument slot, the pair is built in the frame (key word,
//   string copy ctor), the 4-byte iterator (user copy ctor) is built in the argument area and the result iterator comes
//   back through the hidden pointer; both string temporaries are destroyed after the call.
class CCharString {
public:
    void* m_pData;                                   // +0x00
    CCharString();                                   // 0x0099E4B0
    CCharString(const CCharString& other);           // 0x0099EC30
    ~CCharString();                                  // 0x0099EAE0
};

namespace _STL {
struct __false_type { __false_type() { } };

template <class _Tp>
inline _Tp __default_constructed_aux(_Tp*, const __false_type&) { return _Tp(); }
template <class _Tp>
inline _Tp __default_constructed(_Tp* __p) { return __default_constructed_aux(__p, __false_type()); }

template <class _Tp>
class less {
public:
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _T1, class _T2>
class pair {
public:
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};

template <class _Tp>
class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

struct _Rb_tree_node_base {
    char                _M_color;                    // +0x00
    _Rb_tree_node_base* _M_parent;                   // +0x04
    _Rb_tree_node_base* _M_left;                     // +0x08
    _Rb_tree_node_base* _M_right;                    // +0x0C
};

template <class _Value>
struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                           // +0x10
};

struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};

template <class _Value>
struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) { _M_node = __it._M_node; }
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    bool operator==(const _Rb_tree_iterator& __y) const { return _M_node == __y._M_node; }
};

template <class _Key, class _Value, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_iterator<_Value> iterator;
    _STLP_alloc_proxy<_Rb_tree_node_base*, _Rb_tree_node<_Value>, _Alloc> _M_header;   // +0x00
    unsigned int _M_node_count;                      // +0x04
    _Compare     _M_key_compare;                     // +0x08

    _Rb_tree_node_base* _M_root() const { return _M_header._M_data->_M_parent; }
    static const _Key& _S_key(_Rb_tree_node_base* __x) { return ((_Rb_tree_node<_Value>*)__x)->_M_value_field.first; }
    iterator end() { return iterator(_M_header._M_data); }
    iterator lower_bound(const _Key& __k)
    {
        _Rb_tree_node_base* __y = _M_header._M_data;
        _Rb_tree_node_base* __x = _M_root();
        while (__x != 0)
            if (!_M_key_compare(_S_key(__x), __k))
                __y = __x, __x = __x->_M_left;
            else
                __x = __x->_M_right;
        return iterator(__y);
    }
    iterator insert_unique(iterator __position, const _Value& __v);   // 0x00A49F60
};

template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;

    iterator end() { return _M_t.end(); }
    iterator lower_bound(const _Key& __x) { return _M_t.lower_bound(__x); }
    _Compare key_comp() const { return _M_t._M_key_compare; }
    iterator insert(iterator __position, const value_type& __x) { return _M_t.insert_unique(__position, __x); }

    _Tp& operator[](const _Key& __k)
    {
        iterator __i = lower_bound(__k);
        if (__i == end() || key_comp()(__k, (*__i).first))
            __i = insert(__i, value_type(__k, __default_constructed((_Tp*)0)));
        return (*__i).second;
    }
};

template CCharString& map<unsigned long, CCharString>::operator[](const unsigned long&);
}