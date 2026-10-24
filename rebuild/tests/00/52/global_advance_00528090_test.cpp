// Behaviour test for 0x00528090 advance(set iterator&, unsigned n): n == 0 leaves the iterator alone; otherwise it
// steps n nodes in key order (through a parent climb and a right-subtree descent), landing on end() after the last key.
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
}

typedef std::_SetIter It;
int main()
{
    build_tree();
    It it; it._M_node = g_hdr._M_left;
    std::advance(it, 0u);
    int ok = it._M_node == &g_n[0];
    std::advance(it, 1u);
    ok &= it._M_node == &g_n[1];
    std::advance(it, 2u);
    ok &= it._M_node == &g_n[3] && (*it).Id == 40;
    std::advance(it, 2u);
    ok &= it._M_node == &g_hdr;
    It j; j._M_node = &g_n[0];
    std::advance(j, 5u);
    ok &= j._M_node == &g_hdr;
    // the decrement arm (header -> rightmost, then a left-climb) through the shared helper
    It k; k._M_node = &g_hdr; --k; ok &= k._M_node == &g_n[4];
    --k; ok &= k._M_node == &g_n[3];
    --k; ok &= k._M_node == &g_n[2];
    --k; ok &= k._M_node == &g_n[1];
    if (!ok) { std::printf("00528090_TEST FAIL\n"); return 1; }
    std::printf("00528090_TEST PASS\n");
    return 0;
}