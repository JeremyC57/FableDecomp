// test for 0057fed6 _M_upper_bound
#include <stdio.h>
#include <string.h>
namespace std {
template <class T1, class T2> struct pair {
    typedef T1 first_type;
    T1 first;
    T2 second;
};
template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};
template <class T> class allocator {};
template <class V> struct _Select1st {
    const typename V::first_type& operator()(const V& x) const { return x.first; }
};
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
    _Base_ptr _M_root() const { return _M_header->_M_parent; }
    static const K& _S_key(_Base_ptr x) { return KoV()(((_Link_type)x)->_M_value_field); }
    _Base_ptr _M_upper_bound(const K& k) const
    {
        _Base_ptr y = _M_header;
        _Base_ptr x = _M_root();
        while (x != 0)
            if (_M_key_compare(k, _S_key(x)))
                y = x, x = x->_M_left;
            else
                x = x->_M_right;
        return y;
    }
};
}
typedef std::pair<const unsigned long, long> CULongValue;
template class std::_Rb_tree<unsigned long, CULongValue, std::_Select1st<CULongValue>, std::less<unsigned long>, std::allocator<CULongValue> >;

typedef std::_Rb_tree_node<CULongValue> Node;
static Node* mk(unsigned long k) { static unsigned long pool[8][8]; static int n; Node* x = (Node*)pool[n++]; memset(x, 0, sizeof(*x)); *(unsigned long*)&x->_M_value_field.first = k; return x; }
int main()
{
    typedef std::_Rb_tree<unsigned long, CULongValue, std::_Select1st<CULongValue>, std::less<unsigned long>, std::allocator<CULongValue> > Tree;
    std::_Rb_tree_node_base header; memset(&header, 0, sizeof(header));
    Node* r = mk(20); Node* l = mk(10); Node* rr = mk(30);
    r->_M_left = l; r->_M_right = rr; header._M_parent = r;
    Tree t; t._M_header = &header; t._M_node_count = 3;
    unsigned long k;
    int ok = 1;
    k = 5;  ok &= t._M_upper_bound(k) == l;
    k = 10; ok &= t._M_upper_bound(k) == r;
    k = 25; ok &= t._M_upper_bound(k) == rr;
    k = 30; ok &= t._M_upper_bound(k) == &header;
    header._M_parent = 0; k = 1; ok &= t._M_upper_bound(k) == &header;
    printf(ok ? "0057fed6_TEST PASS\n" : "0057fed6_TEST FAIL\n");
    return ok ? 0 : 1;
}