// test for 00431773 _M_create_node
#include <stdio.h>
#include <stdlib.h>
#include <new>
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const CCharString& s);                   // 0x0099EC30
    bool operator<(const CCharString& other) const;
};
namespace std {
template <class T1, class T2> struct pair {
    typedef T1 first_type;
    T1 first;
    T2 second;
    __forceinline pair(const pair& p) : first(p.first), second(p.second) {}
};
template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};
template <class T> class allocator {};
template <class V> struct _Select1st {
    const typename V::first_type& operator()(const V& x) const { return x.first; }
};
template <class T1, class T2> __forceinline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
struct _Rb_tree_node_base {
    bool                _M_color;                        // +0x00
    _Rb_tree_node_base* _M_parent;                       // +0x04
    _Rb_tree_node_base* _M_left;                         // +0x08
    _Rb_tree_node_base* _M_right;                        // +0x0c
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                    // +0x10
};
template <class K, class V, class KoV, class C, class A> class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<V>*   _Link_type;
    _Base_ptr _M_header;                                 // +0x00
    size_t    _M_node_count;                             // +0x04
    C         _M_key_compare;
    _Link_type _M_create_node(const V& x)
    {
        _Link_type tmp = (_Link_type)malloc(sizeof(_Rb_tree_node<V>));
        _Construct(&tmp->_M_value_field, x);
        return tmp;
    }
};
}
typedef std::pair<const CCharString, long> CCharStringLongValue;
template class std::_Rb_tree<CCharString, CCharStringLongValue, std::_Select1st<CCharStringLongValue>, std::less<CCharString>, std::allocator<CCharStringLongValue> >;

static int g_cc;
CCharString::CCharString(const CCharString& s) : PStringData(s.PStringData) { ++g_cc; }
bool CCharString::operator<(const CCharString& o) const { return PStringData < o.PStringData; }
typedef std::_Rb_tree<CCharString, CCharStringLongValue, std::_Select1st<CCharStringLongValue>, std::less<CCharString>, std::allocator<CCharStringLongValue> > Tree;
int main()
{
    unsigned long raw[2] = { 0x77, 42 };
    const CCharStringLongValue& v = *(const CCharStringLongValue*)raw;
    Tree t;
    Tree::_Link_type n = t._M_create_node(v);
    int ok = n != 0 && g_cc == 1 && n->_M_value_field.first.PStringData == (CCharStringData*)0x77 && n->_M_value_field.second == 42
          && (char*)&n->_M_value_field - (char*)n == 0x10;
    free(n);
    printf(ok ? "00431773_TEST PASS\n" : "00431773_TEST FAIL\n");
    return 0;
}