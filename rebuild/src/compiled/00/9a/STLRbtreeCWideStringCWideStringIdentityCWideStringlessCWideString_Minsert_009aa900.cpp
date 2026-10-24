// _STL::_Rb_tree<CWideString, CWideString, _Identity<CWideString>, less<CWideString> >::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const CWideString& v, _Rb_tree_node_base* w) -- set<CWideString> node insert: create a node for v, link it left or right of y, fix root/leftmost/rightmost, rebalance, count, return the iterator
// Leaf: ?_M_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009AA900, 187 bytes, thiscall `ret 14h` (the hidden iterator result pointer first).
// Catalog name "BinaryTree_InsertNode" (descriptive). Caller: insert_unique @ 0x009AAF1E (0x009AAEB0, this chunk).
// Callees: CWideString::operator< @ 0x0099AF70, malloc @ 0x00BFEA0E + CWideString(const CWideString&) @ 0x0099B720
// (_M_create_node inlined: allocate(1) = malloc(0x14), placement copy with VC7.1's null check), _Rb_global<bool>::
// _Rebalance @ 0x0042971D (__cdecl (node, root&)).
// Shape: STLport 4.6 `_M_insert` verbatim in an /O2 TU (the landed 0x00485BF0 is the same source in a size TU): VC7.1
// lays the right-insert arm first and the left-insert arm (the `y == header` target) second.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

class CWideString {
public:
    void* PStringData;                                      // +0x00
    CWideString(const CWideString& other);                  // 0x0099B720
    bool operator<(const CWideString& other) const;         // 0x0099AF70
};

namespace _STL {
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
};

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp> class _STLP_alloc_proxy : public allocator<_Tp> {
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

template <class _Key, class _Value, class _KeyOfValue, class _Compare> class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value> iterator;

    _STLP_alloc_proxy<_Link_type, _Rb_tree_node<_Value> > _M_header;   // +0x00
    unsigned int _M_node_count;                                         // +0x04
    _Compare _M_key_compare;                                            // +0x08

    static const _Key& _S_key(_Rb_tree_node_base* __x) { return _KeyOfValue()(((_Link_type)__x)->_M_value_field); }
    _Link_type _M_create_node(const _Value& __x)
    {
        _Link_type __tmp = this->_M_header.allocate(1);
        _Construct(&__tmp->_M_value_field, __x);
        return __tmp;
    }
    iterator _M_insert(_Rb_tree_node_base* __x, _Rb_tree_node_base* __y, const _Value& __v, _Rb_tree_node_base* __w = 0);
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare>::iterator
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare>::_M_insert(_Rb_tree_node_base* __x_, _Rb_tree_node_base* __y_,
                                                         const _Value& __v, _Rb_tree_node_base* __w_)
{
    _Link_type __w = (_Link_type)__w_;
    _Link_type __x = (_Link_type)__x_;
    _Link_type __y = (_Link_type)__y_;
    _Link_type __z;

    if (__y == this->_M_header._M_data ||
        (__w == 0 && (__x != 0 || _M_key_compare(_KeyOfValue()(__v), _S_key(__y))))) {
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

template _Rb_tree<CWideString, CWideString, _Identity<CWideString>, less<CWideString> >::iterator
_Rb_tree<CWideString, CWideString, _Identity<CWideString>, less<CWideString> >::_M_insert(
    _Rb_tree_node_base*, _Rb_tree_node_base*, const CWideString&, _Rb_tree_node_base*);
}