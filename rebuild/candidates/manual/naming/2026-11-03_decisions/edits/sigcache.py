OLD = """template <class K, class T, class C = less<K> > class map {
public:
    typedef pair<const K, T> value_type;
    typedef _Rb_tree_iterator<value_type> iterator;
    _Rb_tree_node_base* _M_header;
    unsigned int        _M_node_count;
    C                   _M_key_compare;
    _Rb_tree_node_base* _M_find(const K& k) const;       // 0x004856CD
    pair<iterator, bool> insert_unique(const value_type& v);   // 0x00485E5C
    iterator find(const K& k) { return iterator(_M_find(k)); }
    iterator end() { return iterator(_M_header); }
    pair<iterator, bool> insert(const value_type& v) { return insert_unique(v); }
};"""
NEW = """template <class _Pair> struct _Select1st { };          // STLport 4.6 _function.h (map's KeyOfValue)
template <class _Tp> class allocator { };
// STLport 4.6.1 _tree.h: find / _M_find are member templates (owner decision 2026-11-03), _M_find private.
template <class K, class V, class KoV, class C, class A> class _Rb_tree {
public:
    typedef _Rb_tree_iterator<V> iterator;
    _Rb_tree_node_base* _M_header;
    unsigned int        _M_node_count;
    C                   _M_key_compare;
    template <class _KT> iterator find(const _KT& k) { return iterator(_M_find(k)); }
    iterator end() { return iterator(_M_header); }
private:
    template <class _KT> _Rb_tree_node<V>* _M_find(const _KT& k) const;   // 0x004856CD
};
template <class K, class T, class C = less<K> > class map {
public:
    typedef pair<const K, T> value_type;
    typedef _Rb_tree<K, value_type, _Select1st<value_type>, C, allocator<value_type> > _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    pair<iterator, bool> insert_unique(const value_type& v);   // 0x00485E5C
    iterator find(const K& k) { return _M_t.find(k); }
    iterator end() { return _M_t.end(); }
    pair<iterator, bool> insert(const value_type& v) { return insert_unique(v); }
};"""
EDITS = [("rebuild/src/compiled/00/47/CTCInGameMenu_VerifySignatureCached_0047ed0a.cpp", OLD, NEW),
         ("rebuild/src/compiled/00/59/NUISystemCFrontEndManager_VerifySignatureCached_005957d9.cpp", OLD, NEW)]
