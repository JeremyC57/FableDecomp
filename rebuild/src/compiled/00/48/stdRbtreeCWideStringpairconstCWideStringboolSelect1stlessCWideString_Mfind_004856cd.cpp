// std::_Rb_tree<CWideString, pair<const CWideString, bool>, _Select1st, less<CWideString> >::_M_find(const CWideString& k) const -- lower-bound walk, then the header unless the found key equals k
// Retail: Fable.exe (TLC, VC7.1) @ 0x004856CD, 73 bytes, `ret 4`, the node pointer in eax.
// Callers: CTCInGameMenu::VerifySignatureCached 0x0047ED0A and NUISystem::CFrontEndManager::VerifySignatureCached
// 0x005957D9 (both landed: "cache.find(filename) (STLport _M_find 0x004856CD)", map<CWideString, bool> Signatures),
// plus 0x00485983 / 0x00485A88.
// Callees: CWideString::operator<(const CWideString&) const @ 0x0099AF70 (out of line; less<CWideString> inlined).
// Shape: STLport 4.x `_M_find`: `y = header; x = root; while (x) if (!comp(key(x), k)) y = x, x = left(x); else
// x = right(x); if (y == header || comp(k, key(y))) y = header; return y;` -- the header re-read for the result.
// Size TU (the while loop enters at its bottom test); the functor is `__forceinline` (the size TU calls it otherwise).
// Label correction: catalog "RBTree_FindNode_Iterator" is a guess.
// Leaf: _M_find
#pragma optimize("s", on)

class CWideString {
public:
    void* m_pData;
    bool operator<(const CWideString& other) const;         // 0x0099AF70
};

namespace std {
template <class _Tp> struct less {
    __forceinline bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x00
    _T2 second;                                             // +0x04
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

template <class _Key, class _Value, class _Compare> class _Rb_tree {
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

template <class _Key, class _Value, class _Compare>
_Rb_tree_node<_Value>* _Rb_tree<_Key, _Value, _Compare>::_M_find(const _Key& __k) const
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

template _Rb_tree_node<pair<const CWideString, bool> >*
_Rb_tree<CWideString, pair<const CWideString, bool>, less<CWideString> >::_M_find(const CWideString&) const;
}