// _STL::map::operator[](const EClothingSuitPart& k) -- STLport subscript of map<EClothingSuitPart, bool> (size TU): lower_bound, insert (k, false) when absent, return the flag
// Leaf: ??A?$map
// Retail: Fable.exe (TLC, VC7.1) @ 0x005BBC4E, 69 bytes, __fastcall `ret 4`, frame-pointer size TU.
// Identity: four calls from CTCInventoryClothing vfunc 77 (0x005B76D1); the mapped value is a byte (`mov byte [ebp-4],0`),
//   the key an enum (signed `jge` compare). Ego_r.pdb's CTCInventoryClothing members are later-build maps of long.
// Callees: _M_lower_bound @ 0x005BACEC (node pointer in eax), insert(iterator, const value_type&) @ 0x005BBB7D (iterator
//   built in its slot, result through the dead `k` slot).
// Shape: the landed map<NUISystem::CComponent*, long>::operator[] 0x0042D1B7 template (STLport 4.x, optimize("s", on)).
//   Chunk-422 recipe (precedent 0x009913D0): a same-TU noinline stand-in for insert that reads v but never keeps its address
//   lets VC7.1 store the pair's `false` after the hidden-result push, as retail (declared only, the byte store moves up
//   ahead of the iterator store: 69v69).
#pragma optimize("s", on)
enum EClothingSuitPart { CLOTHING_SUIT_PART_NONE };
namespace _STL {
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
    _Rb_tree_node_base* _M_lower_bound(const K& k) const;         // 0x005BACEC
    iterator insert(iterator position, const value_type& v);      // 0x005BBB7D
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
#pragma auto_inline(off)
// Same-TU stand-in for insert(hint, v) 0x005BBB7D (reads v, never keeps its address).
template <class K, class T, class C>
__declspec(noinline) typename map<K, T, C>::iterator map<K, T, C>::insert(iterator position, const value_type& v)
{
    if (v.second)
        ++_M_node_count;
    return position;
}
#pragma auto_inline(on)

template bool& map<EClothingSuitPart, bool>::operator[](const EClothingSuitPart&);
}