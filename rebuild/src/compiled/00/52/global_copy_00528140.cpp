// __copy(set<CCharString>::const_iterator first, last, CCharString* result, const input_iterator_tag&, int*) -- STLport input-iterator copy: assign a set's strings, in key order, over an existing CCharString range
// Leaf: ??$__copy@U?$_Rb_tree_iterator@VCCharString@@
//
// Retail 0x00528140, 99 bytes, `ret 0Ch` (/Gr __fastcall: result in ecx, the tag reference in edx (unused); first, last
//   and the null _Distance* on the stack). Returns the end of the assigned range.
// Callees: 0x0099EFB0 CCharString::operator=(const CCharString&).
// Callers: 0x0052B6A0 (twice, the in-place paths of vector<CCharString>::_M_range_insert for set<CCharString>
//   iterators: `copy(first, last, pos)` and `copy(first, mid, pos)`), 0x005295B0 (vector<CCharString>::_M_assign_aux
//   from a set, catalog "std::vector_assign_buffer"), and one more site at 0x00528AD0.
// Shape: STLport `for (; first != last; ++result, ++first) *result = *first; return result;` (++result before ++first,
//   hence the `add edi,4` ahead of the walk); STLport 4.5's static _Rb_global<bool>::_M_increment is inlined through
//   the iterator's `_M_node = _M_increment(_M_node)`.
// Label: catalog "Tree_AssignElements_CCharString" (descriptive); the STLport name is __copy.
inline void* __cdecl operator new(unsigned int, void* where) throw() { return where; }

class CCharString {
public:
    const char* PString;
    CCharString(const CCharString& other);                  // 0x0099EC30
    const CCharString& operator=(const CCharString& other);       // 0x0099EFB0
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
template <class _InputIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_InputIter __first, _InputIter __last, _OutputIter __result, const input_iterator_tag&,
                              _Distance*)
{
    for (; __first != __last; ++__result, ++__first)
        *__result = *__first;
    return __result;
}
template CCharString* __fastcall __copy<_SetIter, CCharString*, int>(_SetIter, _SetIter, CCharString*,
                                                                      const input_iterator_tag&, int*);
}