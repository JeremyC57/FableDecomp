// Behaviour test for 0x00616F90 std::_Rb_tree<long, pair<const long, pair<long,long> >, ...>::_M_insert(x, y, v, w): new node as
// y's left child (y == header, or x set, or v.first < key(y), unless the hint w is set) else right child; root/leftmost/
// rightmost upkeep, _Rebalance (0x0042971D, stubbed: records its arguments), count, iterator to the node.
#include <cstdio>
#include <cstdlib>
#include <new>

namespace stl {
template <class T> struct less { bool operator()(const T& x, const T& y) const { return x < y; } };
template <class T> struct allocator { };
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair& __p) : first(__p.first), second(__p.second) { }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };

static _Rb_tree_node_base*  g_rebalX = 0;
static _Rb_tree_node_base** g_rebalRoot = 0;
static int g_rebalCalls = 0;
struct _Rb_global_inst {
    static void _Rebalance(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root)
    {
        g_rebalX = __x; g_rebalRoot = &__root; ++g_rebalCalls;
    }
};

template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node<_Value>* __x) : _M_node(__x) { }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) : _M_node(__it._M_node) { }
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value> iterator;
    _Link_type   _M_header;
    unsigned int _M_node_count;
    _Compare     _M_key_compare;

    static _Rb_tree_node_base*& _S_left(_Rb_tree_node_base* __x) { return __x->_M_left; }
    static _Rb_tree_node_base*& _S_right(_Rb_tree_node_base* __x) { return __x->_M_right; }
    static _Rb_tree_node_base*& _S_parent(_Rb_tree_node_base* __x) { return __x->_M_parent; }
    static const _Key& _S_key(_Rb_tree_node_base* __x) { return _KeyOfValue()(((_Link_type)__x)->_M_value_field); }
    _Rb_tree_node_base*& _M_root() const { return _M_header->_M_parent; }
    _Rb_tree_node_base*& _M_leftmost() const { return _M_header->_M_left; }
    _Rb_tree_node_base*& _M_rightmost() const { return _M_header->_M_right; }
    _Link_type _M_create_node(const _Value& __x)
    {
        _Link_type __tmp = (_Link_type)std::malloc(sizeof(_Rb_tree_node<_Value>));
        new (&__tmp->_M_value_field) _Value(__x);
        return __tmp;
    }
    iterator _M_insert(_Rb_tree_node_base* __x_, _Rb_tree_node_base* __y_, const _Value& __v, _Rb_tree_node_base* __w_ = 0)
    {
        _Link_type __w = (_Link_type)__w_;
        _Link_type __x = (_Link_type)__x_;
        _Link_type __y = (_Link_type)__y_;
        _Link_type __z;
        if (__y == _M_header || (__w == 0 && (__x != 0 || _M_key_compare(_KeyOfValue()(__v), _S_key(__y))))) {
            __z = _M_create_node(__v);
            _S_left(__y) = __z;
            if (__y == _M_header) {
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
        _Rb_global_inst::_Rebalance(__z, _M_header->_M_parent);
        ++_M_node_count;
        return iterator(__z);
    }
};
}

typedef stl::pair<long, long> Counter;
typedef stl::pair<const long, Counter> Value;
typedef stl::_Rb_tree<long, Value, stl::_Select1st<Value>, stl::less<long>, stl::allocator<Value> > Tree;
typedef stl::_Rb_tree_node<Value> Node;

int main()
{
    int ok = 1;
    Node* header = (Node*)std::malloc(sizeof(Node));
    header->_M_color = true; header->_M_parent = 0; header->_M_left = header; header->_M_right = header;
    Tree t;
    t._M_header = header;
    t._M_node_count = 0;
    // first node under the header: becomes root, leftmost and rightmost
    Tree::iterator r = t._M_insert(0, header, Value(50, Counter(1, 0)));
    Node* n50 = (Node*)r._M_node;
    if (header->_M_parent != n50 || header->_M_left != n50 || header->_M_right != n50 || n50->_M_parent != header ||
        n50->_M_left != 0 || n50->_M_right != 0 || t._M_node_count != 1 || n50->_M_value_field.first != 50 ||
        n50->_M_value_field.second.first != 1 || stl::g_rebalX != n50 || stl::g_rebalRoot != &header->_M_parent) ok = 0;
    // smaller key under the leftmost: left child, new leftmost
    Node* n20 = (Node*)t._M_insert(0, n50, Value(20, Counter(2, 5)))._M_node;
    if (n50->_M_left != n20 || header->_M_left != n20 || header->_M_right != n50 || n20->_M_parent != n50 ||
        n20->_M_value_field.second.second != 5) ok = 0;
    // larger key: right child, new rightmost
    Node* n80 = (Node*)t._M_insert(0, n50, Value(80, Counter(3, 0)))._M_node;
    if (n50->_M_right != n80 || header->_M_right != n80 || header->_M_left != n20) ok = 0;
    // x != 0 forces the left side even for a larger key
    Node* n90 = (Node*)t._M_insert(n80, n80, Value(90, Counter(4, 0)))._M_node;
    if (n80->_M_left != n90 || header->_M_right != n80) ok = 0;
    // a hint w forces the right side even for a smaller key
    Node* n10 = (Node*)t._M_insert(0, n20, Value(10, Counter(5, 0)), n50)._M_node;
    if (n20->_M_right != n10 || header->_M_left != n20) ok = 0;
    if (t._M_node_count != 5 || stl::g_rebalCalls != 5 || stl::g_rebalX != n10) ok = 0;
    std::free(n10); std::free(n90); std::free(n80); std::free(n20); std::free(n50); std::free(header);
    if (!ok) { std::printf("00616f90_TEST FAIL\n"); return 1; }
    std::printf("00616f90_TEST PASS\n");
    return 0;
}