// _STL::_Rb_tree<CWideString, CWideString, _Identity<CWideString>, less<CWideString> >::insert_unique(const CWideString& v) -- set<CWideString> insert: find v's position, insert it unless an equal string exists, return the iterator and whether it was inserted
// Leaf: ?insert_unique@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009AAEB0, 155 bytes, thiscall `ret 8` (hidden pair<iterator, bool> result). Catalog
// name "RBTree_LowerBound" is a guess. Callers: CBankFileManager 0x009A7A63 / 0x009A7DBC (CreatedRetailBanks, the
// set<CWideString> at +0x28) and 0x009AB68D.
// Callees: CWideString::operator< @ 0x0099AF70, iterator::operator--() @ 0x009AA3E0 (thiscall, out of line; its
// body is in this TU), _M_insert @
// 0x009AA900 (this chunk; iterator by hidden pointer).
// Shape: STLport 4.6 `insert_unique(const value_type&)` verbatim (the landed 0x00485E5C is the same source in a size TU):
// `__comp` starts true (`mov al,1`), `__j` and `_M_insert`'s iterator result are both homed in the v argument slot.
class CWideString {
public:
    void* PStringData;                                      // +0x00
    bool operator<(const CWideString& other) const;         // 0x0099AF70
};

namespace _STL {
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
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

template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) { }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) : _M_node(__it._M_node) { }
    bool operator==(const _Rb_tree_iterator& __y) const { return _M_node == __y._M_node; }
    _Rb_tree_iterator& operator--();                        // 0x009AA3E0
};

// The iterator's operator-- body (landed 0x009AA3E0, same TU) kept out of line: with it visible VC7.1 knows &__j does not
// escape the call, so __j's and _M_insert's result temporaries share the dead v argument slot (as retail).
#pragma auto_inline(off)
template <class _Value>
__declspec(noinline) _Rb_tree_iterator<_Value>& _Rb_tree_iterator<_Value>::operator--()
{
    _Rb_tree_node_base* __p = _M_node;
    if (__p->_M_color == 0 && __p->_M_parent->_M_parent == __p)
        __p = __p->_M_right;
    else if (__p->_M_left) {
        _Rb_tree_node_base* __q = __p->_M_left;
        while (__q->_M_right)
            __q = __q->_M_right;
        __p = __q;
    }
    else {
        _Rb_tree_node_base* __q = __p->_M_parent;
        while (__p == __q->_M_left) {
            __p = __q;
            __q = __q->_M_parent;
        }
        __p = __q;
    }
    _M_node = __p;
    return *this;
}
#pragma auto_inline(on)

template <class _Key, class _Value, class _KeyOfValue, class _Compare> class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value> iterator;

    _STLP_alloc_proxy<_Link_type, _Rb_tree_node<_Value> > _M_header;   // +0x00
    unsigned int _M_node_count;                                         // +0x04
    _Compare _M_key_compare;                                            // +0x08

    static const _Key& _S_key(_Rb_tree_node_base* __x) { return _KeyOfValue()(((_Link_type)__x)->_M_value_field); }
    _Link_type _M_root() const { return (_Link_type)this->_M_header._M_data->_M_parent; }
    _Link_type _M_leftmost() const { return (_Link_type)this->_M_header._M_data->_M_left; }
    static _Link_type _S_left(_Link_type __x) { return (_Link_type)__x->_M_left; }
    static _Link_type _S_right(_Link_type __x) { return (_Link_type)__x->_M_right; }
    iterator begin() { return iterator(_M_leftmost()); }
    iterator _M_insert(_Rb_tree_node_base* __x, _Rb_tree_node_base* __y, const _Value& __v,
                       _Rb_tree_node_base* __w = 0);                     // 0x009AA900
    pair<iterator, bool> insert_unique(const _Value& __v);
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare>
pair<typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare>::iterator, bool>
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare>::insert_unique(const _Value& __v)
{
    _Link_type __y = this->_M_header._M_data;
    _Link_type __x = _M_root();
    bool __comp = true;
    while (__x != 0) {
        __y = __x;
        __comp = _M_key_compare(_KeyOfValue()(__v), _S_key(__x));
        __x = __comp ? _S_left(__x) : _S_right(__x);
    }
    iterator __j = iterator(__y);
    if (__comp)
        if (__j == begin())
            return pair<iterator, bool>(_M_insert(/* __x*/ __y, __y, __v), true);
        else
            --__j;
    if (_M_key_compare(_S_key(__j._M_node), _KeyOfValue()(__v)))
        return pair<iterator, bool>(_M_insert(__x, __y, __v), true);
    return pair<iterator, bool>(__j, false);
}

template pair<_Rb_tree<CWideString, CWideString, _Identity<CWideString>, less<CWideString> >::iterator, bool>
_Rb_tree<CWideString, CWideString, _Identity<CWideString>, less<CWideString> >::insert_unique(const CWideString&);
}