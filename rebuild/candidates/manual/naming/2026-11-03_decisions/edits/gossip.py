OLD = """template <class _Key, class _Tp, class _Compare = less<_Key> > class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree_iterator<value_type> iterator;
    _Rb_tree_node_base* _M_header;                       // +0x00
    unsigned int        _M_node_count;                   // +0x04
    _Compare            _M_key_compare;                  // +0x08

    _Rb_tree_node_base* _M_find(const _Key& __k) const;  // 0x008AC200 (_Rb_tree::_M_find)
    iterator find(const _Key& __k) { return iterator(_M_find(__k)); }
    iterator end() { return iterator(_M_header); }
    _Tp& operator[](const _Key& __k);                    // 0x007AEBE0
};"""
NEW = """template <class _Pair> struct _Select1st { };          // STLport 4.6 _function.h (map's KeyOfValue)
// STLport 4.6.1 _tree.h: find / _M_find are member templates (owner decision 2026-11-03), _M_find private.
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc> class _Rb_tree {
public:
    typedef _Rb_tree_iterator<_Value> iterator;
    _Rb_tree_node_base* _M_header;                       // +0x00
    unsigned int        _M_node_count;                   // +0x04
    _Compare            _M_key_compare;                  // +0x08
    template <class _KT> iterator find(const _KT& __k) { return iterator(_M_find(__k)); }
    iterator end() { return iterator(_M_header); }
private:
    template <class _KT> _Rb_tree_node<_Value>* _M_find(const _KT& __k) const;  // 0x008AC200
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > > class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator find(const _Key& __k) { return _M_t.find(__k); }
    iterator end() { return _M_t.end(); }
    _Tp& operator[](const _Key& __k);                    // 0x007AEBE0
};"""
EDITS = [("rebuild/src/compiled/00/8a/CTCAIScratchpad_GetGossip_008ae510.cpp", OLD, NEW),
         ("rebuild/src/compiled/00/8a/CTCAIScratchpad_GetGossipFactions_008ae970.cpp", OLD, NEW)]
