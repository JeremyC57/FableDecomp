OLD = """template <class K, class T> class map {
public:
    typedef pair<const K, T> value_type;
    typedef _Rb_tree_iterator<value_type> iterator;
    _Rb_tree_node_base* _M_header;                      // +0x00
    unsigned int        _M_node_count;                  // +0x04
    _Rb_tree_node_base* _M_find(const K& k);            // 0x00592F92
    iterator find(const K& k) { return iterator(_M_find(k)); }
    iterator begin() { return iterator(_M_header->_M_left); }
    iterator end() { return iterator(_M_header); }
};"""
NEW = """template <class _Tp> struct less { };                  // STLport 4.6 _function.h
template <class _Pair> struct _Select1st { };          // (map's KeyOfValue)
template <class _Tp> class allocator { };
// STLport 4.6.1 _tree.h: find / _M_find are member templates (owner decision 2026-11-03), _M_find private and const.
template <class K, class V, class KoV, class C, class A> class _Rb_tree {
public:
    typedef _Rb_tree_iterator<V> iterator;
    _Rb_tree_node_base* _M_header;                      // +0x00
    unsigned int        _M_node_count;                  // +0x04
    C                   _M_key_compare;                 // +0x08
    template <class _KT> iterator find(const _KT& k) { return iterator(_M_find(k)); }
    iterator begin() { return iterator(_M_header->_M_left); }
    iterator end() { return iterator(_M_header); }
private:
    template <class _KT> _Rb_tree_node<V>* _M_find(const _KT& k) const;   // 0x00592F92
};
template <class K, class T, class C = less<K>, class A = allocator<pair<const K, T> > > class map {
public:
    typedef pair<const K, T> value_type;
    typedef _Rb_tree<K, value_type, _Select1st<value_type>, C, A> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator find(const K& k) { return _M_t.find(k); }
    iterator begin() { return _M_t.begin(); }
    iterator end() { return _M_t.end(); }
};"""
EDITS = [("rebuild/src/compiled/00/58/CTCInventory_ContructItemLists_0058ef79.cpp", OLD, NEW),
         ("rebuild/src/compiled/00/59/CTCInventory_ContructItemListsPC_00590198.cpp", OLD, NEW)]
