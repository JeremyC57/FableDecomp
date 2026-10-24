// advance(set<CCharString>::const_iterator& i, unsigned int n) -- STLport bidirectional advance: step a set iterator n nodes forward (or back for a negative distance)
// Leaf: ??$advance@U?$_Rb_tree_iterator@VCCharString@@
//
// Retail 0x00528090, 167 bytes (+ int3 padding to 176), plain `ret` (__cdecl: STLport's _STLP_CALL, iterator reference then distance).
// Callees: none (the tree walks are inline).
// Callers: 0x0052B763 (vector<CCharString>::_M_range_insert for set<CCharString> iterators: `advance(mid,
//   elems_after)`) and one more site at 0x00528A70.
// Shape: STLport `advance` -> inline `__advance(i, n, bidirectional_iterator_tag)`: `if (n > 0) while (n--) ++i; else
//   while (n++) --i;`. The distance is the caller's size_type, so `n > 0` is the unsigned `jbe`, and the decrement arm
//   survives as dead code behind the second `je` (VC does not prove it unreachable); each step stores the node back
//   through the reference. Increment/decrement are STLport 4.5's static _Rb_global<bool>::_M_increment/_M_decrement,
//   inlined (decrement's first test is the end() header: red node whose grandparent is itself -> rightmost).
//   VC7.1 keeps a plain `inline` __advance out of line here (a 25-byte forwarder); __forceinline folds it as retail did.
// Label: catalog "BinaryTree_AdvanceIterator" (descriptive); the STLport name is advance.
inline void* __cdecl operator new(unsigned int, void* where) throw() { return where; }

class CCharString {
public:
    const char* PString;
    CCharString(const CCharString& other);                  // 0x0099EC30
    CCharString& operator=(const CCharString& other);       // 0x0099EFB0
};

namespace std {
struct __false_type { __false_type() {} };
struct input_iterator_tag { input_iterator_tag() {} };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() {} };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() {} };

struct _Rb_tree_node_base {
    bool                _M_color;      // +0x0 (false = red)
    _Rb_tree_node_base* _M_parent;     // +0x4
    _Rb_tree_node_base* _M_left;       // +0x8
    _Rb_tree_node_base* _M_right;      // +0xC
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;             // +0x10
};
template <class _Dummy> struct _Rb_global {
    static _Rb_tree_node_base* __cdecl _M_increment(_Rb_tree_node_base* _M_node)
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        }
        else {
            _Rb_tree_node_base* __y = _M_node->_M_parent;
            while (_M_node == __y->_M_right) {
                _M_node = __y;
                __y = __y->_M_parent;
            }
            if (_M_node->_M_right != __y)
                _M_node = __y;
        }
        return _M_node;
    }
    static _Rb_tree_node_base* __cdecl _M_decrement(_Rb_tree_node_base* _M_node)
    {
        if (_M_node->_M_color == false && _M_node->_M_parent->_M_parent == _M_node)
            _M_node = _M_node->_M_right;
        else if (_M_node->_M_left != 0) {
            _Rb_tree_node_base* __y = _M_node->_M_left;
            while (__y->_M_right != 0)
                __y = __y->_M_right;
            _M_node = __y;
        }
        else {
            _Rb_tree_node_base* __y = _M_node->_M_parent;
            while (_M_node == __y->_M_left) {
                _M_node = __y;
                __y = __y->_M_parent;
            }
            _M_node = __y;
        }
        return _M_node;
    }
};
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    void _M_increment() { _M_node = _Rb_global<bool>::_M_increment(_M_node); }
    void _M_decrement() { _M_node = _Rb_global<bool>::_M_decrement(_M_node); }
};
template <class _Tp> struct _Const_traits {
    typedef const _Tp& reference;
};
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef typename _Traits::reference reference;
    reference operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    _Rb_tree_iterator& operator++() { _M_increment(); return *this; }
    _Rb_tree_iterator& operator--() { _M_decrement(); return *this; }
};
template <class _Value, class _Traits>
inline bool operator!=(const _Rb_tree_iterator<_Value, _Traits>& __x, const _Rb_tree_iterator<_Value, _Traits>& __y)
{
    return __x._M_node != __y._M_node;
}
typedef _Rb_tree_iterator<CCharString, _Const_traits<CCharString> > _SetIter;

template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
}

namespace std {
template <class _BidirectionalIterator, class _Distance>
__forceinline void __cdecl __advance(_BidirectionalIterator& __i, _Distance __n, const bidirectional_iterator_tag&)
{
    if (__n > 0)
        while (__n--) ++__i;
    else
        while (__n++) --__i;
}
template <class _InputIterator, class _Distance>
void __cdecl advance(_InputIterator& __i, _Distance __n)
{
    __advance(__i, __n, bidirectional_iterator_tag());
}
template void __cdecl advance<_SetIter, unsigned int>(_SetIter&, unsigned int);
}