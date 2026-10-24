// std::_Rb_tree<CWideString, pair<const CWideString, bool>, _Select1st, less<CWideString> >::insert_unique(pair<iterator, bool>* result, const value_type& v) -- find v's insert position; insert it unless an equal key exists, return the iterator and whether it was inserted
// Retail: Fable.exe (TLC, VC7.1) @ 0x00485E5C, 146 bytes, `ret 8` (hidden pair<iterator, bool> result).
// Callers: CTCInGameMenu::VerifySignatureCached 0x0047ED0A (landed: "insert_unique 0x00485E5C", the Signatures cache of
// map<CWideString, bool>) and its CFrontEndManager twin 0x005957D9.
// Callees: CWideString::operator< @ 0x0099AF70, _Rb_global<bool>::_M_decrement @ 0x004294E2 (__cdecl), _M_insert @
// 0x00485BF0 (landed in this chunk; iterator by hidden pointer).
// Shape: STLport 4.x `insert_unique(const value_type&)`: walk down keeping `__comp` (starts `true`: `mov al,1`), `__j =
// iterator(__y)`; when `__comp` and `__j == begin()` insert with `_M_insert(/*__x*/ __y, __y, __v)` (the zero `__w`
// pushed from the exhausted `__x` register), else `--__j`; insert when key(j) < v, otherwise return (j, false).
// `_M_insert`'s iterator result is homed in the dead `v` argument slot. Size TU with a frame pointer.
// Label correction: catalog "RBTree_LowerBound_Iterator" is a guess.
// Leaf: insert_unique
#pragma optimize("y", off)
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
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
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

template <class _Dummy> struct _Rb_global {
    static void __cdecl _Rebalance(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root);   // 0x0042971D
    static _Rb_tree_node_base* __cdecl _M_decrement(_Rb_tree_node_base* __x);             // 0x004294E2
};

template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) { }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) : _M_node(__it._M_node) { }
    bool operator==(const _Rb_tree_iterator& __y) const { return _M_node == __y._M_node; }
    _Rb_tree_iterator& operator--() { _M_node = _Rb_global<bool>::_M_decrement(_M_node); return *this; }
};

template <class _Key, class _Value, class _Compare> class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value> iterator;

    _STLP_alloc_proxy<_Link_type, _Rb_tree_node<_Value> > _M_header;   // +0x00
    unsigned int _M_node_count;                                         // +0x04
    _Compare _M_key_compare;                                            // +0x08

    static const _Key& _S_key(_Rb_tree_node_base* __x) { return ((_Link_type)__x)->_M_value_field.first; }
    _Link_type _M_root() const { return (_Link_type)this->_M_header._M_data->_M_parent; }
    _Link_type _M_leftmost() const { return (_Link_type)this->_M_header._M_data->_M_left; }
    static _Link_type _S_left(_Link_type __x) { return (_Link_type)__x->_M_left; }
    static _Link_type _S_right(_Link_type __x) { return (_Link_type)__x->_M_right; }
    iterator begin() { return iterator(_M_leftmost()); }
    iterator _M_insert(_Rb_tree_node_base* __x, _Rb_tree_node_base* __y, const _Value& __v,
                       _Rb_tree_node_base* __w = 0);                     // 0x00485BF0
    pair<iterator, bool> insert_unique(const _Value& __v);
};

template <class _Key, class _Value, class _Compare>
pair<typename _Rb_tree<_Key, _Value, _Compare>::iterator, bool>
_Rb_tree<_Key, _Value, _Compare>::insert_unique(const _Value& __v)
{
    _Link_type __y = this->_M_header._M_data;
    _Link_type __x = _M_root();
    bool __comp = true;
    while (__x != 0) {
        __y = __x;
        __comp = _M_key_compare(__v.first, _S_key(__x));
        __x = __comp ? _S_left(__x) : _S_right(__x);
    }
    iterator __j = iterator(__y);
    if (__comp)
        if (__j == begin())
            return pair<iterator, bool>(_M_insert(/* __x*/ __y, __y, __v), true);
        else
            --__j;
    if (_M_key_compare(_S_key(__j._M_node), __v.first))
        return pair<iterator, bool>(_M_insert(__x, __y, __v), true);
    return pair<iterator, bool>(__j, false);
}

typedef pair<const CWideString, bool> _SignatureValue;
template pair<_Rb_tree<CWideString, _SignatureValue, less<CWideString> >::iterator, bool>
_Rb_tree<CWideString, _SignatureValue, less<CWideString> >::insert_unique(const _SignatureValue&);
}