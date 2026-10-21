// std::map<NUISystem::CComponent*, long>::operator[](const NUISystem::CComponent*& k) -- STLport subscript: lower_bound, insert (k, long()) when absent, return the value
// Leaf: ??A?$map
// Retail: Fable.exe @ 0x0042D1B7, 67 bytes, `ret 4`; callers 0x0042D410, 0x0042D429, 0x005A072D, 0x005A1204, 0x005A1258, ...; label matches.
// Body: i = lower_bound(k) (_M_lower_bound 0x0042AD91, node pointer in eax); if (i == end() || k < (*i).first) (unsigned (`jae`) pointer-key compare)
// i = insert(i, value_type(k, long())) (0x0042CDC3; the value_type temporary in the frame, the iterator argument built in its
// slot by the STLport iterator copy constructor, the returned iterator in the dead parameter slot); return (*i).second
// (node +0x14).
// Opt: size TU with frame pointer (`push ebp / mov ebp,esp`, `leave`, unaligned entry).
#pragma optimize("s", on)
namespace NUISystem { class CComponent; }
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
typedef NUISystem::CComponent* _ComponentKey;
template long& map<_ComponentKey, long>::operator[](const _ComponentKey&);
}