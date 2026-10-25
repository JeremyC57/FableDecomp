// _Rb_tree::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const unsigned __int64& v, _Rb_tree_node_base* w) -- STLport set<unsigned __int64> node insert: create the node under y (left or right by the hint and key order), keep leftmost/rightmost, rebalance, count it
// Leaf: ?_M_insert@?$_Rb_tree@_K_KU?$_Identity@_K@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082F150, 203 bytes (+5 int3), thiscall `ret 14h` (the iterator through the hidden
//   pointer, then x, y, v, w).
// Identity: the 0x18-byte node with an unsigned __int64 value at +0x10 is the set<unsigned __int64> of
//   CScriptedMapBrush::ThingsToDelete / ThingsToAdd (their trees malloc 0x18 headers, 0x0082E600). The catalog label
//   "CMemoryAllocatorVariableSize_InsertNode" is propagated.
// Callees: malloc 0x00BFEA0E (_M_create_node), _Rb_global<bool>::_Rebalance(z, root&) 0x0042971D (__cdecl).
// Shape: STLport 4.x `_M_insert` verbatim (left when y is the header, or no hint w and either x or v < key(y); the
//   u64 compare high dword first), the node copy a null-tested placement of the two key dwords; explicit
//   instantiation.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

namespace std {
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class T> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};
template <class T> struct less { bool operator()(const T& a, const T& b) const { return a < b; } };
template <class T> struct _Identity { const T& operator()(const T& x) const { return x; } };

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                                     // +0x00
    _Base_ptr _M_parent;                                    // +0x04
    _Base_ptr _M_left;                                      // +0x08
    _Base_ptr _M_right;                                     // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};
template <class _Dummy> struct _Rb_global {
    static void __cdecl _Rebalance(_Rb_tree_node_base* x, _Rb_tree_node_base*& root);   // 0x0042971D
};
typedef _Rb_global<bool> _Rb_global_inst;
template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node_base* x) { _M_node = x; }
};

template <class _Value, class _Node, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};

template <class _Value, class _Alloc> class _Rb_tree_base {
public:
    typedef _Rb_tree_node<_Value> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;   // +0x00
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_iterator<_Value> iterator;
    unsigned int _M_node_count;                             // +0x04
    _Compare     _M_key_compare;                            // +0x08
    _Base_ptr& _M_root() const { return this->_M_header._M_data->_M_parent; }
    _Base_ptr& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    _Base_ptr& _M_rightmost() const { return this->_M_header._M_data->_M_right; }
    static _Base_ptr& _S_left(_Base_ptr x) { return x->_M_left; }
    static _Base_ptr& _S_right(_Base_ptr x) { return x->_M_right; }
    static _Base_ptr& _S_parent(_Base_ptr x) { return x->_M_parent; }
    static const _Key& _S_key(_Base_ptr x) { return _KeyOfValue()(((_Link_type)x)->_M_value_field); }
    _Link_type _M_create_node(const _Value& x)
    {
        _Link_type tmp = this->_M_header.allocate(1);
        _Construct(&tmp->_M_value_field, x);
        return tmp;
    }
    iterator _M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const _Value& v, _Rb_tree_node_base* w = 0);
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_insert(_Rb_tree_node_base* __x_, _Rb_tree_node_base* __y_,
                                                                const _Value& __v, _Rb_tree_node_base* __w_)
{
    _Link_type __w = (_Link_type)__w_;
    _Link_type __x = (_Link_type)__x_;
    _Link_type __y = (_Link_type)__y_;
    _Link_type __z;

    if (__y == this->_M_header._M_data ||
        (__w == 0 &&
         (__x != 0 ||
          _M_key_compare(_KeyOfValue()(__v), _S_key(__y))))) {
        __z = _M_create_node(__v);
        _S_left(__y) = __z;
        if (__y == this->_M_header._M_data) {
            _M_root() = __z;
            _M_rightmost() = __z;
        }
        else if (__y == _M_leftmost())
            _M_leftmost() = __z;
    }
    else {
        __z = _M_create_node(__v);
        _S_right(__y) = __z;
        if (__y == _M_rightmost())
            _M_rightmost() = __z;
    }
    _S_parent(__z) = __y;
    _S_left(__z) = 0;
    _S_right(__z) = 0;
    _Rb_global_inst::_Rebalance(__z, this->_M_header._M_data->_M_parent);
    ++_M_node_count;
    return iterator(__z);
}
}

template class std::_Rb_tree<unsigned __int64, unsigned __int64, std::_Identity<unsigned __int64>, std::less<unsigned __int64> >;