OLD1 = """template <class _Key> class set {
public:
    typedef _Rb_tree_iterator<_Key> iterator;
    _Rb_tree_node_base* _M_header;                          // +0x00
    unsigned int _M_node_count;                             // +0x04
    unsigned int _M_key_compare;                            // +0x08
    _Rb_tree_node_base* _M_find(const _Key& __k) const;     // 0x009AA4C0
    pair<iterator, bool> insert_unique(const _Key& __v);    // 0x009AAEB0
    iterator end() { return iterator(_M_header); }
    iterator find(const _Key& __x) { return iterator(_M_find(__x)); }
    pair<iterator, bool> insert(const _Key& __x) { return insert_unique(__x); }
};"""
NEW1 = """template <class _Tp> struct less { };                      // STLport 4.6 _function.h
template <class _Tp> struct _Identity { };                 // (set's KeyOfValue)
template <class _Tp> class allocator { };
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};
// STLport 4.6.1 _tree.h: find / _M_find are member templates (owner decision 2026-11-03), _M_find private.
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc> class _Rb_tree {
public:
    typedef _Rb_tree_iterator<_Value> iterator;
    _Rb_tree_node_base* _M_header;                          // +0x00
    unsigned int _M_node_count;                             // +0x04
    _Compare _M_key_compare;                                // +0x08
    iterator end() { return iterator(_M_header); }
    template <class _KT> iterator find(const _KT& __x) { return iterator(_M_find(__x)); }
private:
    template <class _KT> _Rb_tree_node<_Value>* _M_find(const _KT& __k) const;     // 0x009AA4C0
};
template <class _Key> class set {
public:
    typedef _Rb_tree<_Key, _Key, _Identity<_Key>, less<_Key>, allocator<_Key> > _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    pair<iterator, bool> insert_unique(const _Key& __v);    // 0x009AAEB0
    iterator end() { return _M_t.end(); }
    iterator find(const _Key& __x) { return _M_t.find(__x); }
    pair<iterator, bool> insert(const _Key& __x) { return insert_unique(__x); }
};"""
OLD2 = """template <class _Key, class _Compare = less<_Key> >
class set {
public:
    typedef _Rb_tree_const_iterator<_Key> const_iterator;
    const_iterator end() const { return const_iterator(_M_header); }
    const_iterator find(const _Key& k) const { return const_iterator(_M_find(k)); }
    _Rb_tree_node_base* _M_find(const _Key& k) const;   // 0x004B9370
private:
    _Rb_tree_node_base* _M_header;              // +0x00
    unsigned int        _M_node_count;          // +0x04
    _Compare            _M_key_compare;         // +0x08
};"""
NEW2 = """template <class _Tp> struct _Identity { };     // STLport 4.6 _function.h (set's KeyOfValue)
template <class _Tp> class allocator { };
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                      // +0x10
};
// STLport 4.6.1 _tree.h: find / _M_find are member templates (owner decision 2026-11-03), _M_find private.
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_const_iterator<_Value> const_iterator;
    const_iterator end() const { return const_iterator(_M_header); }
    template <class _KT> const_iterator find(const _KT& k) const { return const_iterator(_M_find(k)); }
private:
    template <class _KT> _Rb_tree_node<_Value>* _M_find(const _KT& k) const;   // 0x004B9370
    _Rb_tree_node_base* _M_header;              // +0x00
    unsigned int        _M_node_count;          // +0x04
    _Compare            _M_key_compare;         // +0x08
};
template <class _Key, class _Compare = less<_Key> >
class set {
public:
    typedef _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, allocator<_Key> > _Rep_type;
    typedef typename _Rep_type::const_iterator const_iterator;
    const_iterator end() const { return _M_t.end(); }
    const_iterator find(const _Key& k) const { return _M_t.find(k); }
private:
    _Rep_type _M_t;
};"""
EDITS = [("rebuild/src/compiled/00/9a/CBankFileManager_GetRetailBankFilePath_009a7d40.cpp", OLD1, NEW1),
         ("rebuild/src/compiled/00/9c/CFileInstaller_IsFileCompleted_009c43e0.cpp", OLD2, NEW2)]
