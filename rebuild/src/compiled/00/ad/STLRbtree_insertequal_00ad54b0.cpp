// std::_Rb_tree::insert_equal(const value_type& v) -- multimap insert for the optimiser's contraction multimap (multimap<float, CCountedPointer<CVertexPairContraction>>): descend on the float key, link below the last node visited
// Leaf: ?insert_equal@?$_Rb_tree@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD54B0, 0x40 bytes, thiscall, `ret 8` (hidden iterator result + the value
// reference), returns the result pointer.
// Identity: `fld [v] ; fcomp [x+10h] ; test ah,5 ; jp right` is `less<float>(v.first, key(x)) ? left : right` over
// a float key at node +0x10 -- the COptimisedPrimitive::VertexPairContractions multimap (Ego_r.pdb), whose
// erase(iterator) is 0x00AD4CA0. Catalog "BTree_FindAndInsert" is descriptive.
// Callee: _Rb_tree::_M_insert(x, y, v, w) @ 0x00AD4540 (thiscall, iterator returned through the hidden pointer,
// `ret 14h`; manifest "BTreeNode_InsertValue" is descriptive).
// Shape: STLport 4.x insert_equal: y = header, x = root; `while (x) { y = x; x = comp(v, key(x)) ? left : right; }`
// then `return _M_insert(x, y, v)` (w defaults to 0) forwarding the caller's hidden result pointer.
struct SCountInfo;
template <class T> class CCountedPointer {
public:
    T*          Data;                                            // +0x0
    SCountInfo* Info;                                            // +0x4
};
namespace QuadricOptimiserInternals { class CVertexPairContraction; }

namespace std {
template <class T> class allocator { };
template <class T> class less {
public:
    bool operator()(const T& a, const T& b) const { return a < b; }
};
template <class T1, class T2> class pair {
public:
    typedef T1 first_type;
    T1 first;
    T2 second;
};
template <class P> struct _Select1st {
    const typename P::first_type& operator()(const P& x) const { return x.first; }
};
struct _Rb_tree_node_base {
    bool                _M_color;                                // +0x00
    _Rb_tree_node_base* _M_parent;                               // +0x04
    _Rb_tree_node_base* _M_left;                                 // +0x08
    _Rb_tree_node_base* _M_right;                                // +0x0C
};
template <class V> struct _Rb_tree_node : public _Rb_tree_node_base {
    V _M_value_field;                                            // +0x10
};
template <class V> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) { }
    _Rb_tree_iterator(const _Rb_tree_iterator& it) : _M_node(it._M_node) { }
};
template <class V, class A> class _Rb_tree_base {
public:
    typedef _Rb_tree_node<V> _Node;
    struct _Header_proxy : public allocator<_Node> {
        _Node* _M_data;
    } _M_header;
};
template <class K, class V, class KeyOfValue, class Compare, class A> class _Rb_tree : public _Rb_tree_base<V, A> {
public:
    typedef _Rb_tree_node<V>       _Node;
    typedef _Rb_tree_node_base*    _Base_ptr;
    typedef _Rb_tree_iterator<V>   iterator;
    unsigned int _M_node_count;
    Compare      _M_key_compare;

    _Base_ptr& _M_root() const { return this->_M_header._M_data->_M_parent; }
    static _Base_ptr& _S_left(_Base_ptr x) { return x->_M_left; }
    static _Base_ptr& _S_right(_Base_ptr x) { return x->_M_right; }
    static const K& _S_key(_Base_ptr x) { return KeyOfValue()(((_Node*)x)->_M_value_field); }

    iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const V& __v, _Base_ptr __w = 0);   // 0x00AD4540
    iterator insert_equal(const V& __v);
};

template <class K, class V, class KeyOfValue, class Compare, class A>
typename _Rb_tree<K, V, KeyOfValue, Compare, A>::iterator
_Rb_tree<K, V, KeyOfValue, Compare, A>::insert_equal(const V& __v)
{
    _Base_ptr __y = this->_M_header._M_data;
    _Base_ptr __x = _M_root();
    while (__x != 0) {
        __y = __x;
        __x = _M_key_compare(KeyOfValue()(__v), _S_key(__x)) ? _S_left(__x) : _S_right(__x);
    }
    return _M_insert(__x, __y, __v);
}

typedef pair<const float, CCountedPointer<QuadricOptimiserInternals::CVertexPairContraction> > SContractionEntry;
template _Rb_tree<float, SContractionEntry, _Select1st<SContractionEntry>, less<float>, allocator<SContractionEntry> >::iterator
_Rb_tree<float, SContractionEntry, _Select1st<SContractionEntry>, less<float>, allocator<SContractionEntry> >::insert_equal(const SContractionEntry&);
} // namespace std