// std::_Rb_tree::_M_find(const CWideString& k) const -- map<CWideString, CChecksumCacheFile::CEntry> find: lower-bound walk from the root, then the header unless the found node's key equals k (node pointer in eax)
// Leaf: ?_M_find@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A608F0, 83 bytes (+13 int3), `ret 4`.
// Callees: CWideString::operator<(const CWideString&) const @ 0x0099AF70 (less<CWideString> inline).
// Caller: CChecksumCacheFile::GetChecksum (map::find).
// Label: the manifest's `BinaryTree_FindByKey_LeftBias` is a guess; same STLport body as the landed size-TU
//   _M_find<CWideString, bool> @ 0x004856CD, here in an /O2 TU (rotated while loop).
class CWideString {
public:
    void* PStringData;
    bool operator<(const CWideString& other) const;         // 0x0099AF70
};
class CChecksumCacheFile {
public:
    class CEntry {                                          // 0x20 bytes: checksum + CDateAndTime
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
    _Link_type _M_find(const _Key& __k) const;
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
_Rb_tree_node<_Value>* _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_find(const _Key& __k) const
{
    _Link_type __y = this->_M_header._M_data;               // last node which is not less than __k
    _Link_type __x = _M_root();                             // current node
    while (__x != 0)
        if (!_M_key_compare(_S_key(__x), __k))
            __y = __x, __x = _S_left(__x);
        else
            __x = _S_right(__x);
    if (__y == this->_M_header._M_data || _M_key_compare(__k, _S_key(__y)))
        __y = this->_M_header._M_data;
    return __y;
}

template class _Rb_tree<CWideString, pair<const CWideString, CChecksumCacheFile::CEntry>,
                        _Select1st<pair<const CWideString, CChecksumCacheFile::CEntry> >, less<CWideString>,
                        allocator<pair<const CWideString, CChecksumCacheFile::CEntry> > >;

}   // namespace std