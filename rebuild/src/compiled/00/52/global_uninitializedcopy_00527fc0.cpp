// __uninitialized_copy(set<CCharString>::const_iterator first, last, CCharString* result, const __false_type&) -- STLport non-POD copy-construct of a set's strings, in key order, into raw vector storage
// Leaf: ??$__uninitialized_copy@U?$_Rb_tree_iterator@VCCharString@@
//
// Retail 0x00527FC0, 103 bytes, `ret 8` (/Gr __fastcall: the iterators are one-pointer structs, so they go on the stack;
//   result takes ecx and the tag reference edx, unused). Returns the end of the constructed range.
// Callees: 0x0099EC30 CCharString::CCharString(const CCharString&).
// Callers: 0x0052B6A0 (twice) -- STLport vector<CCharString>::_M_range_insert(pos, first, last, forward_iterator_tag)
//   instantiated for set<CCharString> iterators (catalog "CCombatWheel::Insert" is propagated): the tail copy when
//   the inserted range outruns the old end, and the middle copy in the reallocation path.
// Shape: STLport `for (; first != last; ++first, ++cur) _Construct(&*cur, *first); return cur;` -- the placement new
//   keeps its null test on cur, and STLport 4.5's static _Rb_global<bool>::_M_increment(node) is inlined through the iterator's
//   `_M_node = _Rb_global_inst::_M_increment(_M_node)` (the copy-in/assign-back gives the eax temporary and the
//   `mov esi,eax` after the loop test): right subtree's leftmost node, else climb while we are the right child.
// Label: catalog "Tree_CopyElements_CCharString" (descriptive); the STLport name is __uninitialized_copy.
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
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter first, _InputIter last, _ForwardIter result, const __false_type&)
{
    _ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
template CCharString* __fastcall __uninitialized_copy<_SetIter, CCharString*>(_SetIter, _SetIter, CCharString*,
                                                                               const __false_type&);
}