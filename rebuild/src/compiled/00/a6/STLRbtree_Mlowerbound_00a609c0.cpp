// std::_Rb_tree::_M_lower_bound(const CWideString& k) const -- map<CWideString, CChecksumCacheFile::CEntry> lower bound: the first node whose key does not sort before k, the header when none (node pointer in eax)
// Leaf: ?_M_lower_bound@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A609C0, 51 bytes (+13 int3), `ret 4`.
// Callees: CWideString::operator<(const CWideString&) const @ 0x0099AF70 (less<CWideString> inline).
// Caller: map<CWideString, CEntry>::operator[] @ 0x00A617E0 (lower_bound).
// Label: the manifest's `BinaryTree_FindByKey_RBTree` is a guess; STLport 4.6 `_M_lower_bound`.
class CWideString {
public:
    void* PStringData;
    bool operator<(const CWideString& other) const;         // 0x0099AF70
};
class CChecksumCacheFile {
public:
    struct CEntry {                                          // 0x20 bytes: checksum + CDateAndTime
    public:
        unsigned char Data[0x20];
    };
};

namespace std {
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _T1, class _T2> class pair {
public:
    _T1 first;                                              // +0x00
    _T2 second;                                             // +0x04
};
template <class _Pair> struct _Select1st {};
template <class _Tp> class allocator {
};
template <class _Value, class _Tp> class _STLP_alloc_proxy {
public:
    _Value _M_data;
};
struct _Rb_tree_node_base {
    bool                _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0c
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc> class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;

    _STLP_alloc_proxy<_Link_type, _Rb_tree_node<_Value> > _M_header;   // +0x00
    unsigned int _M_node_count;                                         // +0x04
    _Compare _M_key_compare;                                            // +0x08

    static const _Key& _S_key(_Link_type __x) { return __x->_M_value_field.first; }
    static _Link_type _S_left(_Link_type __x) { return (_Link_type)__x->_M_left; }
    static _Link_type _S_right(_Link_type __x) { return (_Link_type)__x->_M_right; }
    _Link_type _M_root() const { return (_Link_type)this->_M_header._M_data->_M_parent; }
    _Link_type _M_lower_bound(const _Key& __k) const;
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
_Rb_tree_node<_Value>* _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_lower_bound(const _Key& __k) const
{
    _Link_type __y = this->_M_header._M_data;               // last node which is not less than __k
    _Link_type __x = _M_root();                             // current node
    while (__x != 0)
        if (!_M_key_compare(_S_key(__x), __k))
            __y = __x, __x = _S_left(__x);
        else
            __x = _S_right(__x);
    return __y;
}

template class _Rb_tree<CWideString, pair<const CWideString, CChecksumCacheFile::CEntry>,
                        _Select1st<pair<const CWideString, CChecksumCacheFile::CEntry> >, less<CWideString>,
                        allocator<pair<const CWideString, CChecksumCacheFile::CEntry> > >;

}   // namespace std