// std::_Rb_tree<CCharString, pair<const CCharString, long>, _Select1st, less<CCharString> >::erase(const CCharString& k) -- erase every node keyed k: equal_range, count the range, erase it, return the count
// Retail: Fable.exe (TLC, VC7.1) @ 0x00475BF6, 69 bytes, `ret 4`.
// Callers: CPersistContext::TransferMap<CCharString, long> @ 0x0047550F (call 0x004756A2; landed, names this
// "erase(key)" of its map<CCharString, long>).
// Callees: equal_range(const key&) @ 0x00475CDD (pair<iterator, iterator> through a hidden pointer),
// distance(const iterator&, const iterator&) @ 0x00475C3B (__cdecl: STLport `_STLP_CALL`, the __distance loop over
// _Rb_global<bool>::_M_increment 0x004292C0), erase(iterator first, iterator last) @ 0x00475C5E.
// Shape: STLport 4.x `pair<iterator,iterator> p = equal_range(x); size_type n = distance(p.first, p.second);
// erase(p.first, p.second); return n;` -- the iterators have user copy constructors, so the by-value erase arguments
// are built in their slots, the first one in the distance call's not-yet-popped argument slot (`add esp,4`).
// Tree TU settings: frame pointer kept, size-optimised (`optimize("y",off)` + `("s",on)`, as 0x0047550F).
// Label correction: catalog "RBTree_TraverseAndCompare_Copy" is a guess.
// Leaf: erase
#pragma optimize("y", off)
#pragma optimize("s", on)

class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
};

namespace std {
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const;
};

template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x00
    _T2 second;                                             // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};

template <class _Value, class _Tp> class _STLP_alloc_proxy {
public:
    _Value _M_data;
};

struct _Rb_tree_node_base {
    bool                _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0c
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};

template <class _Dummy> struct _Rb_global {
    static _Rb_tree_node_base* __cdecl _M_increment(_Rb_tree_node_base* __x);   // 0x004292C0
};

template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) { }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) : _M_node(__it._M_node) { }
    _Rb_tree_iterator& operator++() { _M_node = _Rb_global<bool>::_M_increment(_M_node); return *this; }
    bool operator!=(const _Rb_tree_iterator& __y) const { return _M_node != __y._M_node; }
};

template <class _InputIterator>
int __cdecl distance(const _InputIterator& __first, const _InputIterator& __last)   // 0x00475C3B
{
    int __n = 0;
    _InputIterator __it = __first;
    while (__it != __last) {
        ++__it;
        ++__n;
    }
    return __n;
}

template <class _Key, class _Value, class _Compare> class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value> iterator;

    _STLP_alloc_proxy<_Link_type, _Rb_tree_node<_Value> > _M_header;   // +0x00
    unsigned int _M_node_count;                                         // +0x04
    _Compare _M_key_compare;                                            // +0x08

    pair<iterator, iterator> equal_range(const _Key& __x);              // 0x00475CDD
    void erase(iterator __first, iterator __last);                      // 0x00475C5E
    unsigned int erase(const _Key& __x);
};

template <class _Key, class _Value, class _Compare>
unsigned int _Rb_tree<_Key, _Value, _Compare>::erase(const _Key& __x)
{
    pair<iterator, iterator> __p = equal_range(__x);
    unsigned int __n = distance(__p.first, __p.second);
    erase(__p.first, __p.second);
    return __n;
}

template unsigned int
_Rb_tree<CCharString, pair<const CCharString, long>, less<CCharString> >::erase(const CCharString&);
}