// std::map<unsigned long, CAllocationNode*>::insert(const value_type& v) -- forward to the tree's insert_unique and return its (iterator, bool) pair
// Leaf: insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042D1A3, length 20 bytes (__fastcall, hidden result pointer).
// Identity: the catalog label GFPredicateAnd<...> is propagated; the body is the STLport map::insert forwarder.
// Callees: _Rb_tree::insert_unique 0x0042CEE3.
// Shape: `#pragma optimize("s", on)`; the pair has user constructors (copy ctor included) so the result is copied through the
//   hidden pointer; the member is defined out of class and the map is explicitly instantiated.
#pragma optimize("s", on)
namespace std {
template <class T1, class T2> struct pair {
    T1 first; T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    pair(const pair& o) : first(o.first), second(o.second) { }
};
template <class T> struct less { };
struct _Rb_tree_node_base;
struct _Rb_tree_iterator { _Rb_tree_node_base* _M_node; };
template <class K, class V, class C> class _Rb_tree {
public:
    typedef pair<const K, V> value_type;
    _Rb_tree_node_base* _M_header; unsigned long _M_node_count; C _M_key_compare;
    pair<_Rb_tree_iterator, bool> insert_unique(const value_type& v);   // 0x0042CEE3
};
template <class K, class V, class C> class map {
public:
    typedef pair<const K, V> value_type;
    _Rb_tree<K, V, C> _M_t;
    pair<_Rb_tree_iterator, bool> insert(const value_type& v);
};
template <class K, class V, class C> pair<_Rb_tree_iterator, bool> map<K, V, C>::insert(const value_type& v) { return _M_t.insert_unique(v); }
}
struct CAllocationNode;
template class std::map<unsigned long, CAllocationNode*, std::less<unsigned long> >;