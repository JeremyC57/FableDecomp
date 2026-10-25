// std::_Rb_tree<CWideString, CWideString, _Identity<CWideString>, less<CWideString> >::_M_find(const CWideString& k) const -- set<CWideString> lookup: lower-bound walk, then the header unless the found key equals k
// Leaf: ?_M_find@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009AA4C0, 83 bytes, thiscall `ret 4`, the node pointer in eax. Catalog name
// "BinaryTree_FindByComparator" (descriptive). Callers: CBankFileManager 0x009A7D9B (CreatedRetailBanks, the
// set<CWideString> at +0x28) and the two find() wrappers 0x009AA9C0 / 0x009AAF50 (identical bodies for this tree).
// Callee: CWideString::operator<(const CWideString&) const @ 0x0099AF70 (less<CWideString> inlined).
// Shape: STLport 4.6 `_M_find` verbatim (the landed 0x004856CD is the same algorithm in a size TU; this TU is /O2, so
// the while loop is rotated: one test before it, one at the bottom); the header is re-read for both header returns.
// Layout: _M_header._M_data (header node pointer) +0, _M_node_count +4; node = {color, parent +4, left +8, right +0xC,
// value +0x10}.
class CWideString {
public:
    void* PStringData;                                      // +0x00
    bool operator<(const CWideString& other) const;         // 0x0099AF70
};

namespace std {
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
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

template <class _Key, class _Value, class _KeyOfValue, class _Compare> class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;

    _STLP_alloc_proxy<_Link_type, _Rb_tree_node<_Value> > _M_header;   // +0x00
    unsigned int _M_node_count;                                         // +0x04
    _Compare _M_key_compare;                                            // +0x08

    static const _Key& _S_key(_Link_type __x) { return _KeyOfValue()(__x->_M_value_field); }
    static _Link_type _S_left(_Link_type __x) { return (_Link_type)__x->_M_left; }
    static _Link_type _S_right(_Link_type __x) { return (_Link_type)__x->_M_right; }
    _Link_type _M_root() const { return (_Link_type)this->_M_header._M_data->_M_parent; }
    _Link_type _M_find(const _Key& __k) const;
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare>
_Rb_tree_node<_Value>* _Rb_tree<_Key, _Value, _KeyOfValue, _Compare>::_M_find(const _Key& __k) const
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

template _Rb_tree_node<CWideString>*
_Rb_tree<CWideString, CWideString, _Identity<CWideString>, less<CWideString> >::_M_find(const CWideString&) const;
}