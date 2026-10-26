// std::map::operator[](const key_type& k) -- (map<long, long>) STLport subscript: lower_bound, insert (k, T()) when absent, return the value
// Leaf: ??A?$map@JJU?$less@J@std@@V?$allocator@U?$pair@$$CBJJ@std@@@2@@std@@QAEAAJABJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005B30BB, 69 bytes, thiscall `ret 4`; size-optimised TU with a frame pointer
//   (`push ebp / mov ebp,esp`, `leave`, unaligned entry).
// Identity (proven): map<long, long>::operator[]; evidence: its callers are CTCInventoryTrade::InitialiseBuyableItems 0x005A30A1 (a local map) and CGameScriptInterface::RegisterTimer 0x006E79E0 -- Ego_r.pdb's CGameScriptInterface::ScriptTimers and CTCInventoryTrade::SpecialItemsToDelete are map<long, long>, one COMDAT for every user; the PDB name sets hold this operator[]
// Label: the manifest's "operator[]" is propagated (family H083: six byte-identical subscripts over signed
//   4-byte keys and 4-byte values; retail has no identical-code folding, so each address is one instantiation).
// Callees: _M_lower_bound 0x005B1DD9 (node pointer in eax), insert(iterator, const value_type&) 0x005B2EA7 (the returned
//   iterator in the dead parameter slot).
// Model: the landed map<NUISystem::CComponent*, long>::operator[] 0x0042D1B7 with a signed key (`jge` where the pointer key
//   gives `jae`) and STLport's allocator parameter (the PDB symbol carries it): i = lower_bound(k); if (i == end() ||
//   k < (*i).first) i = insert(i, value_type(k, T())); return (*i).second (node +0x14).
#pragma optimize("s", on)

namespace std {
template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};
template <class T> class allocator {
public:
    allocator() { }
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) {}
};
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                    // +0x10
};
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    bool operator==(const _Rb_tree_base_iterator& y) const { return _M_node == y._M_node; }
};
template <class V> struct _Rb_tree_iterator : _Rb_tree_base_iterator {
    _Rb_tree_iterator(_Rb_tree_node_base* x) { _M_node = x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& it) { _M_node = it._M_node; }
    V& operator*() const { return static_cast<_Rb_tree_node<V>*>(_M_node)->_M_value_field; }
};
template <class K, class T, class C = less<K>, class A = allocator<pair<const K, T> > > class map {
public:
    typedef pair<const K, T> value_type;
    typedef _Rb_tree_iterator<value_type> iterator;
    _Rb_tree_node_base* _M_header;                       // +0x00
    unsigned int        _M_node_count;                   // +0x04
    C                   _M_key_compare;                  // +0x08
    _Rb_tree_node_base* _M_lower_bound(const K& k) const;              // 0x005B1DD9
    iterator insert(iterator position, const value_type& v);           // 0x005B2EA7
    iterator lower_bound(const K& k) { return iterator(_M_lower_bound(k)); }
    iterator end() { return iterator(_M_header); }
    C key_comp() const { return _M_key_compare; }
    T& operator[](const K& k)
    {
        iterator i = lower_bound(k);
        if (i == end() || key_comp()(k, (*i).first))
            i = insert(i, value_type(k, T()));
        return (*i).second;
    }
};
}
template long& std::map<long, long >::operator[](long const&);