// std::map<long, EEngine2DLayer>::operator[](const long& k) -- STLport subscript: lower_bound, insert (k, EEngine2DLayer()) when absent, return the value
// Leaf: ??A?$map
// Retail: Fable.exe @ 0x0042D246, 67 bytes, `ret 4`; callers 0x0041E205.. (`Layers[-3] = 0x40`, ...: NUISystem::CManager Layers, retail +0x70); catalog label map<EUsedKeys,long>::operator[] is wrong: this is the second, distinct instantiation (own lower-bound 0x0042AE33 / insert 0x0042CE46), used by the Layers map<long, EEngine2DLayer>.
// Body: i = lower_bound(k) (_M_lower_bound 0x0042AE33, node pointer in eax); if (i == end() || k < (*i).first) (signed (`jge`) key compare)
// i = insert(i, value_type(k, EEngine2DLayer())) (0x0042CE46; the value_type temporary in the frame, the iterator argument built in its
// slot by the STLport iterator copy constructor, the returned iterator in the dead parameter slot); return (*i).second
// (node +0x14).
// Opt: size TU with frame pointer (`push ebp / mov ebp,esp`, `leave`, unaligned entry).
#pragma optimize("s", on)
enum EEngine2DLayer { ENGINE_2D_LAYER_NONE };
namespace std {
template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
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
template <class K, class T, class C = less<K> > class map {
public:
    typedef pair<const K, T> value_type;
    typedef _Rb_tree_iterator<value_type> iterator;
    _Rb_tree_node_base* _M_header;
    unsigned int        _M_node_count;
    C                   _M_key_compare;
    _Rb_tree_node_base* _M_lower_bound(const K& k) const;
    iterator insert(iterator position, const value_type& v);
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
template EEngine2DLayer& map<long, EEngine2DLayer>::operator[](const long&);
}