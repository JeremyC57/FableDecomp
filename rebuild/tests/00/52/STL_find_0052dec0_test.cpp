// Behaviour test for 0052dec0: a copy of the source's model and logic with its out-of-line callees stubbed.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
static int g_fail = 0;
static void Check(bool ok, int line)
{
    if (!ok) {
        g_fail = 1;
        std::printf("check failed at line %d\n", line);
    }
}
#define CHECK(c) Check((c) ? true : false, __LINE__)

struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* _M_node)   // STLport _Rb_global<bool>::_M_increment
{
    if (_M_node->_M_right != 0) {
        _M_node = _M_node->_M_right;
        while (_M_node->_M_left != 0)
            _M_node = _M_node->_M_left;
    }
    else {
        _Rb_tree_node_base* y = _M_node->_M_parent;
        while (_M_node == y->_M_right) {
            _M_node = y;
            y = y->_M_parent;
        }
        if (_M_node->_M_right != y)
            _M_node = y;
    }
    return _M_node;
}
// build a balanced tree over nodes[0..n) (in key order) under header; returns the root
static _Rb_tree_node_base* BuildRange(_Rb_tree_node_base** nodes, int lo, int hi, _Rb_tree_node_base* parent)
{
    if (lo >= hi) return 0;
    int mid = (lo + hi) / 2;
    _Rb_tree_node_base* n = nodes[mid];
    n->_M_color = false;
    n->_M_parent = parent;
    n->_M_left = BuildRange(nodes, lo, mid, n);
    n->_M_right = BuildRange(nodes, mid + 1, hi, n);
    return n;
}
static void BuildTree(_Rb_tree_node_base* header, _Rb_tree_node_base** nodes, int n)
{
    header->_M_color = true;
    header->_M_parent = BuildRange(nodes, 0, n, header);
    header->_M_left = n ? nodes[0] : header;
    header->_M_right = n ? nodes[n - 1] : header;
}

enum EEvent { EVENT_NONE };
struct input_iterator_tag { input_iterator_tag() { } };
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;
};
template <class V> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node_base* n) : _M_node(n) { }
    V& operator*() const { return ((_Rb_tree_node<V>*)_M_node)->_M_value_field; }
    _Rb_tree_iterator& operator++() { _M_node = _M_increment(_M_node); return *this; }
    bool operator==(const _Rb_tree_iterator& o) const { return _M_node == o._M_node; }
    bool operator!=(const _Rb_tree_iterator& o) const { return _M_node != o._M_node; }
};
template <class I, class T> I __find(I first, I last, const T& value, const input_iterator_tag&)
{
    while (first != last && !(*first == value))
        ++first;
    return first;
}
typedef _Rb_tree_iterator<EEvent> iterator;
static void RunTest()
{
    _Rb_tree_node<EEvent> n[6];
    _Rb_tree_node_base* order[6];
    int i;
    for (i = 0; i < 6; ++i) {
        n[i]._M_value_field = (EEvent)(i * 3 - 4);    // -4 -1 2 5 8 11
        order[i] = &n[i];
    }
    _Rb_tree_node_base header;
    BuildTree(&header, order, 6);
    iterator first(header._M_left), last(&header);
    for (i = 0; i < 6; ++i) {
        EEvent e = (EEvent)(i * 3 - 4);
        CHECK(__find(first, last, e, input_iterator_tag())._M_node == &n[i]);
    }
    EEvent missing = (EEvent)3;
    CHECK(__find(first, last, missing, input_iterator_tag())._M_node == &header);
    // a sub-range stops at last even when the value lies beyond it
    EEvent e11 = (EEvent)11;
    CHECK(__find(first, iterator(&n[3]), e11, input_iterator_tag())._M_node == &n[3]);
    // the empty range
    CHECK(__find(last, last, e11, input_iterator_tag())._M_node == &header);
}

int main()
{
    RunTest();
    if (g_fail) {
        std::printf("0052dec0_TEST FAIL\n");
        return 1;
    }
    std::printf("0052dec0_TEST PASS\n");
    return 0;
}