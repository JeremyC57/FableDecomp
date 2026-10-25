// std::map::operator[](NUISystem::CComponent* const& k) -- STLport subscript of map<NUISystem::CComponent*, pair<long, long> > (size TU): lower_bound, insert (k, (0, 0)) when absent, return the mapped pair
// Leaf: ??A?$map
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B322B, 74 bytes, __fastcall `ret 4`, frame-pointer size TU.
// Identity: Ego_r.pdb CTCInventoryTrade::ItemsToBuy / ItemsToSell are map<NUISystem::CComponent*, pair<long, long> >; callers
//   CTCInventoryTrade::SellItemPC 0x005AECC0 / 0x005B01FE, BuyItem 0x005AF64E, BuyItemPC 0x005B0AAC.
// Callees: _M_lower_bound @ 0x005B1D38 (node pointer in eax), insert(iterator, const value_type&) @ 0x005B31CF (iterator
//   built in its slot, result through the dead `k` slot).
// Shape: the landed map<NUISystem::CComponent*, long>::operator[] 0x0042D1B7 template (STLport 4.x, optimize("s", on));
//   STLport's pair default ctor value-initialises both longs, and its user copy ctor (`first(o.first), second(o.second)`)
//   keeps them as two `and [ebp-x],0` stores ahead of the key (bare or __default_constructed default with the implicit
//   copy: one xor-zeroed register, 76v74; a POD stand-in: 89v74).
#pragma optimize("s", on)
namespace NUISystem { class CComponent; }
namespace std {
template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair() : first(T1()), second(T2()) {}                // STLport 4.6 pair (member templates on)
    pair(const T1& a, const T2& b) : first(a), second(b) {}
    template <class U1, class U2> pair(const pair<U1, U2>& p) : first(p.first), second(p.second) {}
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) {}
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
    _Rb_tree_node_base* _M_lower_bound(const K& k) const;         // 0x005B1D38
    iterator insert(iterator position, const value_type& v);      // 0x005B31CF
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
typedef NUISystem::CComponent* _ComponentKey;
template pair<long, long>& map<_ComponentKey, pair<long, long> >::operator[](const _ComponentKey&);
}