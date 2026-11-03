OLD1 = """template <class _Key, class _Tp> class map {                       // STLport map, 12 bytes
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree_iterator<value_type> const_iterator;
    _Rb_tree_node_base* _M_header;
    unsigned long       _M_node_count;
    unsigned long       _M_key_compare;
    _Rb_tree_node_base* _M_find(const _Key& __k) const;           // 0x00A2B240
    const_iterator find(const _Key& __k) const { return const_iterator(_M_find(__k)); }
    const_iterator end() const { return const_iterator(_M_header); }
};"""
NEW1 = """template <class _Tp> struct less { };                              // STLport 4.6 _function.h
template <class _Pair> struct _Select1st { };                      // (map's KeyOfValue)
template <class _Tp> class allocator { };
// STLport 4.6.1 _tree.h: find / _M_find are member templates (owner decision 2026-11-03), _M_find private.
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc> class _Rb_tree {
public:
    typedef _Rb_tree_iterator<_Value> const_iterator;
    _Rb_tree_node_base* _M_header;
    unsigned long       _M_node_count;
    _Compare            _M_key_compare;
    template <class _KT> const_iterator find(const _KT& __k) const { return const_iterator(_M_find(__k)); }
    const_iterator end() const { return const_iterator(_M_header); }
private:
    template <class _KT> _Rb_tree_node<_Value>* _M_find(const _KT& __k) const;   // 0x00A2B240
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > > class map {   // STLport map, 12 bytes
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::const_iterator const_iterator;
    _Rep_type _M_t;
    const_iterator find(const _Key& __k) const { return _M_t.find(__k); }
    const_iterator end() const { return _M_t.end(); }
};"""
OLD2 = """template <class K, class T, class C = less<K>, class A = allocator<pair<const K, T> > >
class map {
public:
    typedef pair<const K, T> value_type;
    typedef _Rb_tree_node<value_type> _Node;
    struct iterator {
        _Node* _M_node;
        iterator(_Node* node) : _M_node(node) { }
        bool operator==(const iterator& other) const { return _M_node == other._M_node; }
        bool operator!=(const iterator& other) const { return _M_node != other._M_node; }
        value_type* operator->() const { return &_M_node->_M_value_field; }
    };
    _Node*       _M_header;                             // +0x00
    unsigned int _M_node_count;                         // +0x04
    C            _M_key_compare;                        // +0x08
    _Node* _M_find(const K& key) const;                 // 0x00CD2B86 (map<CCharString, CScriptGameResourceObjectScriptedThingBase>)
    iterator find(const K& key) { return iterator(_M_find(key)); }
    iterator end() { return iterator(_M_header); }
};"""
NEW2 = """template <class P> struct _Select1st { };              // STLport 4.6 _function.h (map's KeyOfValue)
// STLport 4.6.1 _tree.h: find / _M_find are member templates (owner decision 2026-11-03), _M_find private.
template <class K, class V, class KoV, class C, class A>
class _Rb_tree {
public:
    typedef _Rb_tree_node<V> _Node;
    struct iterator {
        _Node* _M_node;
        iterator(_Node* node) : _M_node(node) { }
        bool operator==(const iterator& other) const { return _M_node == other._M_node; }
        bool operator!=(const iterator& other) const { return _M_node != other._M_node; }
        V* operator->() const { return &_M_node->_M_value_field; }
    };
    _Node*       _M_header;                             // +0x00
    unsigned int _M_node_count;                         // +0x04
    C            _M_key_compare;                        // +0x08
    template <class _KT> iterator find(const _KT& key) { return iterator(_M_find(key)); }
    iterator end() { return iterator(_M_header); }
private:
    template <class _KT> _Node* _M_find(const _KT& key) const;   // 0x00CD2B86 (map<CCharString, CScriptGameResourceObjectScriptedThingBase>)
};
template <class K, class T, class C = less<K>, class A = allocator<pair<const K, T> > >
class map {
public:
    typedef pair<const K, T> value_type;
    typedef _Rb_tree<K, value_type, _Select1st<value_type>, C, A> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator find(const K& key) { return _M_t.find(key); }
    iterator end() { return _M_t.end(); }
};"""
EDITS = [("rebuild/src/compiled/00/a2/CMeshDataBank_FindLipSyncAnimation_00a26530.cpp", OLD1, NEW1),
         ("rebuild/src/compiled/00/cb/global_CSFindThing_00cbf9de.cpp", OLD2, NEW2)]
