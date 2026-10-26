// std::_Rb_tree<CWideString, pair<const CWideString, bool>, _Select1st, less<CWideString> >::_M_insert(iterator* result, _Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- link a new node for v left or right of y, fix root/leftmost/rightmost, rebalance, count, return the iterator
// Retail: Fable.exe (TLC, VC7.1) @ 0x00485BF0, 160 bytes (+ int3 padding), `ret 14h`.
// Callers: insert_unique(const value_type&) @ 0x00485E5C (both of its insert paths), the map<CWideString, bool>
// signature caches (CTCInGameMenu / NUISystem::CFrontEndManager::Signatures).
// Callees: CWideString::operator< @ 0x0099AF70, _M_create_node(const value_type&) @ 0x00485A46 (out of line in this TU),
// _Rb_global<bool>::_Rebalance @ 0x0042971D (__cdecl (node, root&)).
// Shape: STLport 4.x `_M_insert` (same source as the CHeroLogBook instantiation 0x0047C500) in a size TU with a frame
// pointer: the node stays out of line, `__w`/`__x` are tested in their argument slots, the zero links are `and
// [esi+8],0` and the root/rightmost stores are tail-merged; the count is `inc dword ptr [edi+4]`.
// Label correction: catalog "CMemoryAllocatorVariableSize_InsertNode" is propagated; the key compare is CWideString's.
// Leaf: ?_M_insert@?$_Rb_tree@VCWideString@@U?$pair@$$CBVCWideString@@_N@std@@U?$less@VCWideString@@@3@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCWideString@@_N@std@@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBVCWideString@@_N@2@0@Z
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
};

template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) { }
};

template <class _Key, class _Value, class _Compare> class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value> iterator;

    _STLP_alloc_proxy<_Link_type, _Rb_tree_node<_Value> > _M_header;   // +0x00
    unsigned int _M_node_count;                                         // +0x04
    _Compare _M_key_compare;                                            // +0x08

    static const _Key& _S_key(_Rb_tree_node_base* __x) { return ((_Link_type)__x)->_M_value_field.first; }
protected:
    _Link_type _M_create_node(const _Value& __x);                       // 0x00485A46
private:
    iterator _M_insert(_Rb_tree_node_base* __x, _Rb_tree_node_base* __y, const _Value& __v,
                       _Rb_tree_node_base* __w = 0);
};

template <class _Key, class _Value, class _Compare>
typename _Rb_tree<_Key, _Value, _Compare>::iterator
_Rb_tree<_Key, _Value, _Compare>::_M_insert(_Rb_tree_node_base* __x_, _Rb_tree_node_base* __y_, const _Value& __v,
                                            _Rb_tree_node_base* __w_)
{
    _Link_type __w = (_Link_type)__w_;
    _Link_type __x = (_Link_type)__x_;
    _Link_type __y = (_Link_type)__y_;
    _Link_type __z;

    if (__y == this->_M_header._M_data ||
        (__w == 0 && (__x != 0 || _M_key_compare(__v.first, _S_key(__y))))) {
        __z = _M_create_node(__v);
        __y->_M_left = __z;
        if (__y == this->_M_header._M_data) {
            this->_M_header._M_data->_M_parent = __z;
            this->_M_header._M_data->_M_right = __z;
        }
        else if (__y == this->_M_header._M_data->_M_left)
            this->_M_header._M_data->_M_left = __z;
    }
    else {
        __z = _M_create_node(__v);
        __y->_M_right = __z;
        if (__y == this->_M_header._M_data->_M_right)
            this->_M_header._M_data->_M_right = __z;
    }
    __z->_M_parent = __y;
    __z->_M_left = 0;
    __z->_M_right = 0;
    _Rb_global<bool>::_Rebalance(__z, this->_M_header._M_data->_M_parent);
    ++this->_M_node_count;
    return iterator(__z);
}

typedef pair<const CWideString, bool> _SignatureValue;
template _Rb_tree<CWideString, _SignatureValue, less<CWideString> >::iterator
_Rb_tree<CWideString, _SignatureValue, less<CWideString> >::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*,
                                                                      const _SignatureValue&, _Rb_tree_node_base*);
}