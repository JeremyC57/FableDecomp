// std::_Rb_tree<CCharString, pair<const CCharString, long>, _Select1st, less<CCharString> >::erase(const CCharString& k) -- STLport: find the equal range of k, count it, erase it, return the count
// Leaf: ?erase@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0047536C, 69 bytes, `ret 4` (thiscall: the key by reference; returns the count).
// Identity: the catalog's "RBTree_TraverseAndCompare" is behavioural. Called from CPersistContext::TransferMap<CCharString,
//   long> @ 0x00475026 (landed; "map<CCharString,long> erase(key) 0x0047536C" there) for CPlayerGuiDef's two
//   map<CCharString, long> members (Ego_r.pdb ScriptSprites / MiniMapGraphics).
// Callees: equal_range(k) @ 0x00475453 (the pair of iterators through the hidden pointer, straight into the local pair),
//   distance(const iterator&, const iterator&) @ 0x004753B1 (__cdecl: the _M_increment walk), erase(iterator first,
//   iterator last) @ 0x004753D4 (both iterators by value).
// Model: STLport 4.6 `size_type erase(const key_type& x) { pair<iterator,iterator> p = equal_range(x); size_type n =
//   distance(p.first, p.second); erase(p.first, p.second); return n; }`. The iterator has a user copy constructor, so the
//   two by-value arguments are copy-constructed in place (`mov eax,esp ; mov [eax],ecx`), the first into the slot the
//   cdecl distance call left behind (`add esp,4`).
// Shape: size TU (#pragma optimize("s", on)), ebp frame for the local pair.
#pragma optimize("s", on)

class CCharString;

namespace std {
template <class T1, class T2> struct pair {
    T1 first;                                       // +0x00
    T2 second;                                      // +0x04
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
struct _Rb_tree_node_base {
    bool                _M_color;                   // +0x00
    _Rb_tree_node_base* _M_parent;                  // +0x04
    _Rb_tree_node_base* _M_left;                    // +0x08
    _Rb_tree_node_base* _M_right;                   // +0x0C
};
template <class Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) { }
    _Rb_tree_iterator(const _Rb_tree_iterator<Value>& it) : _M_node(it._M_node) { }
};

template <class InputIter> int __cdecl distance(const InputIter& first, const InputIter& last);   // 0x004753B1

template <class Key, class Value> class _Rb_tree {
public:
    typedef _Rb_tree_iterator<Value> iterator;
    _Rb_tree_node_base* _M_header;                  // +0x00
    unsigned int        _M_node_count;              // +0x04
    pair<iterator, iterator> equal_range(const Key& k);   // 0x00475453
    void erase(iterator first, iterator last);            // 0x004753D4
    unsigned int erase(const Key& x)
    {
        pair<iterator, iterator> p = equal_range(x);
        unsigned int n = distance(p.first, p.second);
        erase(p.first, p.second);
        return n;
    }
};

typedef pair<const CCharString, long> _MapValue;
template unsigned int _Rb_tree<CCharString, _MapValue>::erase(const CCharString& x);
}