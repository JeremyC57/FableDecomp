OLD = """template <class K, class T> class map {
public:
    typedef pair<const K, T> value_type;
    typedef _Rb_tree_node<value_type> _Node;
    struct const_iterator {
        _Rb_tree_node_base* _M_node;
        const_iterator(_Rb_tree_node_base* n) : _M_node(n) { }
        const value_type* operator->() const { return &static_cast<_Node*>(_M_node)->_M_value_field; }
        bool operator!=(const const_iterator& o) const { return _M_node != o._M_node; }
    };
    _Rb_tree_node_base* _M_header;                      // +0x00
    unsigned int        _M_node_count;                  // +0x04
    unsigned char       _M_key_compare;                 // +0x08 less<K> (empty)
    _Rb_tree_node_base* _M_find(const K& k) const;      // 0x006BCF50
    const_iterator find(const K& k) const { return const_iterator(_M_find(k)); }
    const_iterator end() const { return const_iterator(_M_header); }
};"""
NEW = """template <class _Tp> struct less { };                  // STLport 4.6 _function.h
template <class _Pair> struct _Select1st { };          // (map's KeyOfValue)
template <class _Tp> class allocator { };
// STLport 4.6.1 _tree.h: find / _M_find are member templates (owner decision 2026-11-03), _M_find private.
template <class K, class V, class KoV, class C, class A> class _Rb_tree {
public:
    typedef _Rb_tree_node<V> _Node;
    struct const_iterator {
        _Rb_tree_node_base* _M_node;
        const_iterator(_Rb_tree_node_base* n) : _M_node(n) { }
        const V* operator->() const { return &static_cast<_Node*>(_M_node)->_M_value_field; }
        bool operator!=(const const_iterator& o) const { return _M_node != o._M_node; }
    };
    _Rb_tree_node_base* _M_header;                      // +0x00
    unsigned int        _M_node_count;                  // +0x04
    C                   _M_key_compare;                 // +0x08 less<K> (empty)
    template <class _KT> const_iterator find(const _KT& k) const { return const_iterator(_M_find(k)); }
    const_iterator end() const { return const_iterator(_M_header); }
private:
    template <class _KT> _Rb_tree_node<V>* _M_find(const _KT& k) const;   // 0x006BCF50
};
template <class K, class T, class C = less<K>, class A = allocator<pair<const K, T> > > class map {
public:
    typedef pair<const K, T> value_type;
    typedef _Rb_tree<K, value_type, _Select1st<value_type>, C, A> _Rep_type;
    typedef typename _Rep_type::const_iterator const_iterator;
    _Rep_type _M_t;
    const_iterator find(const K& k) const { return _M_t.find(k); }
    const_iterator end() const { return _M_t.end(); }
};"""
EDITS = [("rebuild/src/compiled/00/6b/CRegion_GetMiniMapRegionExitTextOffset_006bc350.cpp", OLD, NEW)]
