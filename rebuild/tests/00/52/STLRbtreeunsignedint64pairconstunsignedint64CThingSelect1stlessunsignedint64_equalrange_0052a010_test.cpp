// Behaviour test for 0052a010: a copy of the source's model and logic with its out-of-line callees stubbed.
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

class CThing;
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
struct SUIDThingPair {
    u64     first;
    CThing* second;
};
struct SUIDThingNode : _Rb_tree_node_base {
    SUIDThingPair _M_value_field;
};
template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};
struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) { }
    _Rb_tree_iterator(const _Rb_tree_iterator& it) : _M_node(it._M_node) { }
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
static int g_lowerBoundCalls = 0;
class CUIDToThingTree {
public:
    typedef _Rb_tree_iterator iterator;
    _Rb_tree_node_base* _M_header;
    unsigned long       _M_node_count;
    less<u64>           _M_key_compare;
    static const u64& _S_key(_Rb_tree_node_base* x) { return ((SUIDThingNode*)x)->_M_value_field.first; }
    _Rb_tree_node_base* _M_root() const { return _M_header->_M_parent; }
    _Rb_tree_node_base* _M_lower_bound(const u64& k) const          // out of line in retail (0x00528650)
    {
        ++g_lowerBoundCalls;
        _Rb_tree_node_base* y = _M_header;
        _Rb_tree_node_base* x = _M_root();
        while (x != 0)
            if (!_M_key_compare(_S_key(x), k))
                y = x, x = x->_M_left;
            else
                x = x->_M_right;
        return y;
    }
    _Rb_tree_node_base* _M_upper_bound(const u64& k) const
    {
        _Rb_tree_node_base* y = _M_header;
        _Rb_tree_node_base* x = _M_root();
        while (x != 0)
            if (_M_key_compare(k, _S_key(x)))
                y = x, x = x->_M_left;
            else
                x = x->_M_right;
        return y;
    }
    iterator lower_bound(const u64& k) { return iterator(_M_lower_bound(k)); }
    iterator upper_bound(const u64& k) { return iterator(_M_upper_bound(k)); }
    pair<iterator, iterator> equal_range(const u64& k)
    {
        return pair<iterator, iterator>(lower_bound(k), upper_bound(k));
    }
};
static SUIDThingNode g_n[5];
static void RunTest()
{
    // keys 10 20 30 40 50 (high words differ to exercise the 64-bit compare); root 30
    int i;
    for (i = 0; i < 5; ++i) {
        g_n[i]._M_left = g_n[i]._M_right = 0;
        g_n[i]._M_value_field.first = ((u64)(i + 1) * 10) << 32;
        g_n[i]._M_value_field.second = 0;
    }
    _Rb_tree_node_base header;
    header._M_parent = &g_n[2];
    g_n[2]._M_parent = &header;
    g_n[2]._M_left = &g_n[1]; g_n[1]._M_parent = &g_n[2];
    g_n[1]._M_left = &g_n[0]; g_n[0]._M_parent = &g_n[1];
    g_n[2]._M_right = &g_n[3]; g_n[3]._M_parent = &g_n[2];
    g_n[3]._M_right = &g_n[4]; g_n[4]._M_parent = &g_n[3];
    header._M_left = &g_n[0];
    header._M_right = &g_n[4];
    CUIDToThingTree t;
    t._M_header = &header;
    t._M_node_count = 5;

    u64 k = (u64)30 << 32;
    pair<CUIDToThingTree::iterator, CUIDToThingTree::iterator> r = t.equal_range(k);
    CHECK(r.first._M_node == &g_n[2] && r.second._M_node == &g_n[3]);
    k = ((u64)30 << 32) + 1;                       // absent, between 30 and 40: an empty range at 40
    r = t.equal_range(k);
    CHECK(r.first._M_node == &g_n[3] && r.second._M_node == &g_n[3]);
    k = (u64)50 << 32;                             // the largest key: upper bound is end()
    r = t.equal_range(k);
    CHECK(r.first._M_node == &g_n[4] && r.second._M_node == &header);
    k = 5;                                         // below everything
    r = t.equal_range(k);
    CHECK(r.first._M_node == &g_n[0] && r.second._M_node == &g_n[0]);
    CHECK(g_lowerBoundCalls == 4);
}

int main()
{
    RunTest();
    if (g_fail) {
        std::printf("0052a010_TEST FAIL\n");
        return 1;
    }
    std::printf("0052a010_TEST PASS\n");
    return 0;
}