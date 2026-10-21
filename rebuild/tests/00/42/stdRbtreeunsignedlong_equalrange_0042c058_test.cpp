// test for 0042c058 _Rb_tree::equal_range
#include <stdio.h>
#include <string.h>
namespace std {
template <class T1, class T2> struct pair {
    typedef T1 first_type;
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) {}
};
template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};
template <class T> class allocator {};
template <class V> struct _Select1st {
    const typename V::first_type& operator()(const V& x) const { return x.first; }
};
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class V> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) {}
    _Rb_tree_iterator(const _Rb_tree_iterator& it) : _M_node(it._M_node) {}
};
template <class K, class V, class KoV, class C, class A> class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_iterator<V> iterator;
    _Base_ptr _M_header;                                 // +0x00
    size_t    _M_node_count;                             // +0x04
    _Base_ptr _M_lower_bound(const K& k) const;          // 0x0042AE90
    _Base_ptr _M_upper_bound(const K& k) const;          // 0x0042AEB5
    iterator lower_bound(const K& k) { return iterator(_M_lower_bound(k)); }
    iterator upper_bound(const K& k) { return iterator(_M_upper_bound(k)); }
    pair<iterator, iterator> equal_range(const K& k)
    {
        return pair<iterator, iterator>(lower_bound(k), upper_bound(k));
    }
};
}
typedef std::pair<const unsigned long, long> CULongValue;
template std::pair<std::_Rb_tree<unsigned long, CULongValue, std::_Select1st<CULongValue>, std::less<unsigned long>, std::allocator<CULongValue> >::iterator, std::_Rb_tree<unsigned long, CULongValue, std::_Select1st<CULongValue>, std::less<unsigned long>, std::allocator<CULongValue> >::iterator> std::_Rb_tree<unsigned long, CULongValue, std::_Select1st<CULongValue>, std::less<unsigned long>, std::allocator<CULongValue> >::equal_range(const unsigned long&);

typedef std::_Rb_tree<unsigned long, CULongValue, std::_Select1st<CULongValue>, std::less<unsigned long>, std::allocator<CULongValue> > Tree;
static unsigned long g_lk, g_uk; static int g_order;
template <> std::_Rb_tree_node_base* Tree::_M_lower_bound(const unsigned long& k) const { g_lk = k; g_order = g_order * 10 + 1; return (std::_Rb_tree_node_base*)0x100; }
template <> std::_Rb_tree_node_base* Tree::_M_upper_bound(const unsigned long& k) const { g_uk = k; g_order = g_order * 10 + 2; return (std::_Rb_tree_node_base*)0x200; }
int main()
{
    Tree t; memset(&t, 0, sizeof(t));
    unsigned long k = 42;
    std::pair<Tree::iterator, Tree::iterator> r = t.equal_range(k);
    int ok = r.first._M_node == (std::_Rb_tree_node_base*)0x100 && r.second._M_node == (std::_Rb_tree_node_base*)0x200
          && g_lk == 42 && g_uk == 42 && g_order == 21;
    printf(ok ? "0042c058_TEST PASS\n" : "0042c058_TEST FAIL\n");
    return ok ? 0 : 1;
}