// std::map<CComponent*, pair<CWideString, long> >::operator[](const key_type& k) -- STLport subscript on the trade screen's per-component count labels: lower_bound, insert (k, pair<CWideString, long>()) when absent, return the (text, count) pair
// Leaf: ??A?$map
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B3100, 103 bytes, `ret 4`.
// Map: (line 1 short-spells the NUISystem::CComponent* key and std::pair value) CTCInventoryTrade NumBuyableThings / NumSellableThings / NumWantedThings (+0x1B0 / +0x1BC / +0x1C8, Ego_r.pdb
// `map<NUISystem::CComponent *, std::pair<CWideString, long> >`; the landed ctor 0x0059EB0C builds them with 0x005B2E6C).
// Callers: 38 sites in the CTCInventoryTrade TU (0x005A0863 .. 0x005AFDF5), e.g. 0x005A0863 on NumSellableThings
// (`[ebx+1BCh]`), which writes the returned pair's count (`mov [eax+4],ecx`).
// Callees: _M_lower_bound(k) @ 0x005B1DFE (node pointer; the catalog "RBTree_FindNode_Uint3"), CWideString() @ 0x0099AED0
// and ~CWideString @ 0x0099B510, pair<NUISystem::CComponent* const, pair<CWideString, long> >::pair(const K&, const T&)
// @ 0x005B17C6 (out of line), map::insert(iterator, const value_type&) @ 0x005B2EC4 (the hinted tree insert).
// Label: the catalog "StdMap_LowerBound" is descriptive: the body is STLport 4.x `map::operator[]` verbatim, the
// 0x005812DF template with the key a pointer: less<CComponent*> inline (`mov eax,[ebx] ; cmp eax,[esi+10h] ; jae`), the
// `_Tp()` default (pair's inline default ctor: CWideString() then `and [ebp-4],0`) at the top of the frame, the
// value_type temporary (12 bytes) below it, the iterator argument rebuilt in its slot and the insert result in the dead
// `k` slot; the value is node + 0x14.
// Shape: frame-pointer size TU: `optimize("y", off)` + `optimize("s", on)`; `_Tp()` as a named local of the insert block
// (the 0x005812DF form).
#pragma optimize("y", off)
#pragma optimize("s", on)

class CWideString
{
public:
    void* PData;                                         // +0x00
    CWideString();                                       // 0x0099AED0
    ~CWideString();                                      // 0x0099B510
};

namespace NUISystem { class CComponent; }

namespace std {
template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair() : first(T1()), second(T2()) {}
    pair(const T1& a, const T2& b);                      // 0x005B17C6 for the map's value_type
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
template <class K, class T, class C = less<K> > class map {
public:
    typedef pair<const K, T> value_type;
    typedef _Rb_tree_iterator<value_type> iterator;
    _Rb_tree_node_base* _M_header;
    unsigned int        _M_node_count;
    C                   _M_key_compare;
    _Rb_tree_node_base* _M_lower_bound(const K& k) const;              // 0x005B1DFE
    iterator insert(iterator position, const value_type& v);          // 0x005B2EC4
    iterator lower_bound(const K& k) { return iterator(_M_lower_bound(k)); }
    iterator end() { return iterator(_M_header); }
    C key_comp() const { return _M_key_compare; }
    T& operator[](const K& k)
    {
        iterator i = lower_bound(k);
        if (i == end() || key_comp()(k, (*i).first)) {
            T def;
            i = insert(i, value_type(k, def));
        }
        return (*i).second;
    }
};
template pair<CWideString, long>& map<NUISystem::CComponent*, pair<CWideString, long> >::operator[](NUISystem::CComponent* const&);
}