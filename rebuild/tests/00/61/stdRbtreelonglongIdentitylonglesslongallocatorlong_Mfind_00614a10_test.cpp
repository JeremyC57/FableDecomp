// Behaviour test for 0x00614A10 std::_Rb_tree<long, long, _Identity<long>, less<long> >::_M_find(const long&) const: lower-bound
// walk from the root, then the header unless the candidate's key equals k (CTCVillage::PlayersCrimes, a set<long>).
#include <cstdio>

namespace stl {
template <class T> struct less { bool operator()(const T& x, const T& y) const { return x < y; } };
template <class T> struct _Identity { const T& operator()(const T& x) const { return x; } };
template <class T> struct allocator { };

struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
struct _STLP_alloc_proxy : public _MaybeReboundAlloc { _Value _M_data; };

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Key key_type;
    _STLP_alloc_proxy<_Link_type, _Rb_tree_node<_Value>, _Alloc> _M_header;
    unsigned int _M_node_count;
    _Compare _M_key_compare;

    _Link_type _M_root() const { return (_Link_type)_M_header._M_data->_M_parent; }
    static _Link_type _S_left(_Link_type x) { return (_Link_type)x->_M_left; }
    static _Link_type _S_right(_Link_type x) { return (_Link_type)x->_M_right; }
    static const _Key& _S_key(_Link_type x) { return _KeyOfValue()(x->_M_value_field); }

    _Rb_tree_node<_Value>* _M_find(const key_type& __k) const
    {
        _Link_type __y = this->_M_header._M_data;
        _Link_type __x = _M_root();
        while (__x != 0)
            if (!_M_key_compare(_S_key(__x), __k))
                __y = __x, __x = _S_left(__x);
            else
                __x = _S_right(__x);
        if (__y == this->_M_header._M_data || _M_key_compare(__k, _S_key(__y)))
            __y = this->_M_header._M_data;
        return __y;
    }
};
}

typedef stl::_Rb_tree<long, long, stl::_Identity<long>, stl::less<long>, stl::allocator<long> > Tree;
typedef stl::_Rb_tree_node<long> Node;

static void Link(Node& n, long key, Node* parent, Node* left, Node* right)
{
    n._M_color = false; n._M_value_field = key; n._M_parent = parent; n._M_left = left; n._M_right = right;
}

int main()
{
    int ok = 1;
    // root 20; 10 (children -5, 15) on the left, 30 (right child 40) on the right
    Node header, n20, n10, n30, nm5, n15, n40;
    Link(n20, 20, &header, &n10, &n30);
    Link(n10, 10, &n20, &nm5, &n15);
    Link(n30, 30, &n20, 0, &n40);
    Link(nm5, -5, &n10, 0, 0);
    Link(n15, 15, &n10, 0, 0);
    Link(n40, 40, &n30, 0, 0);
    header._M_parent = &n20; header._M_left = &nm5; header._M_right = &n40; header._M_color = true;
    Tree t;
    t._M_header._M_data = &header;
    t._M_node_count = 6;
    long hits[6] = { 20, 10, 30, -5, 15, 40 };
    Node* want[6] = { &n20, &n10, &n30, &nm5, &n15, &n40 };
    for (int i = 0; i < 6; ++i)
        if (t._M_find(hits[i]) != want[i]) ok = 0;
    long misses[5] = { -100, 0, 12, 35, 1000 };
    for (int i = 0; i < 5; ++i)
        if (t._M_find(misses[i]) != &header) ok = 0;
    Tree e;
    Node eh;
    eh._M_parent = 0; eh._M_left = eh._M_right = &eh;
    e._M_header._M_data = &eh;
    if (e._M_find(5) != &eh) ok = 0;
    if (!ok) { std::printf("00614a10_TEST FAIL\n"); return 1; }
    std::printf("00614a10_TEST PASS\n");
    return 0;
}