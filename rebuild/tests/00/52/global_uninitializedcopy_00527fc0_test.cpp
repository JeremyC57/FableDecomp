// Behaviour test for 0x00527FC0 __uninitialized_copy over set<CCharString> iterators: walks the tree in key order,
// copy-constructs each string into raw storage and returns the end; an empty range constructs nothing.
#include <cstdio>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif

static int g_copies, g_assigns;
class CCharString {
public:
    int Id;
    CCharString() : Id(0) {}
    CCharString(const CCharString& other) : Id(other.Id) { ++g_copies; }
    CCharString& operator=(const CCharString& other) { Id = other.Id; ++g_assigns; return *this; }
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
// A hand-built 5-node tree in STLport layout: header (red, parent=root, left=leftmost, right=rightmost) and keys 1..5.
static std::_Rb_tree_node_base g_hdr;
static std::_Rb_tree_node<CCharString> g_n[5];
static void build_tree()
{
    // shape:      2
    //           1   4
    //              3 5
    std::_Rb_tree_node_base* n[5];
    for (int i = 0; i < 5; ++i) { n[i] = &g_n[i]; g_n[i]._M_value_field.Id = 10 * (i + 1); g_n[i]._M_color = true; g_n[i]._M_left = g_n[i]._M_right = 0; }
    g_hdr._M_color = false;
    g_hdr._M_parent = n[1]; n[1]->_M_parent = &g_hdr;
    n[1]->_M_left = n[0]; n[0]->_M_parent = n[1];
    n[1]->_M_right = n[3]; n[3]->_M_parent = n[1];
    n[3]->_M_left = n[2]; n[2]->_M_parent = n[3];
    n[3]->_M_right = n[4]; n[4]->_M_parent = n[3];
    g_hdr._M_left = n[0]; g_hdr._M_right = n[4];
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
}

typedef std::_SetIter It;
int main()
{
    build_tree();
    It b; b._M_node = g_hdr._M_left; It e; e._M_node = &g_hdr;
    static char raw[8 * sizeof(CCharString)];
    CCharString* out = (CCharString*)raw;
    std::__false_type tag;
    int ok = std::__uninitialized_copy(b, b, out, tag) == out && g_copies == 0;
    CCharString* r = std::__uninitialized_copy(b, e, out, tag);
    ok &= r == out + 5 && g_copies == 5;
    for (int i = 0; i < 5; ++i) ok &= out[i].Id == 10 * (i + 1);
    It m; m._M_node = &g_n[3];   // from key 40 to the end
    r = std::__uninitialized_copy(m, e, out + 5, tag);
    ok &= r == out + 7 && out[5].Id == 40 && out[6].Id == 50 && g_copies == 7;
    if (!ok) { std::printf("00527fc0_TEST FAIL\n"); return 1; }
    std::printf("00527fc0_TEST PASS\n");
    return 0;
}