// Behaviour test for 0x00528610 CNPCNameMapTree::_M_lower_bound: on a hand-built 5-node tree (keys straddle the 32-bit
// halves) the walk returns the first node not below k, or the header past the largest key / in an empty tree.
#include <cstdio>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#define K64(x) x##ui64
#else
typedef unsigned long long u64;
#define K64(x) x##ULL
#endif
class CCharString { public: const char* PString; };
class CWideString { public: const unsigned short* PString; };
template <class T1, class T2> struct pair { T1 first; T2 second; };
struct _Rb_tree_node_base {
    char _M_color; _Rb_tree_node_base* _M_parent; _Rb_tree_node_base* _M_left; _Rb_tree_node_base* _M_right;
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base { V _M_value_field; };
class CNPCNameMapTree {
public:
    typedef pair<u64, pair<CCharString, CWideString> > value_type;
    typedef _Rb_tree_node<value_type> _Node;
    _Node* _M_header;
    unsigned long _M_node_count;
    static const u64& _S_key(_Rb_tree_node_base* x) { return ((_Node*)x)->_M_value_field.first; }
    _Rb_tree_node_base* _M_lower_bound(const u64& k) const;
};
_Rb_tree_node_base* CNPCNameMapTree::_M_lower_bound(const u64& k) const
{
    _Rb_tree_node_base* y = _M_header;
    _Rb_tree_node_base* x = _M_header->_M_parent;
    while (x != 0)
        if (!(_S_key(x) < k))
            y = x, x = x->_M_left;
        else
            x = x->_M_right;
    return y;
}

static CNPCNameMapTree::_Node n[6];
static void link(int i, int p, int l, int r) {
    n[i]._M_parent = &n[p]; n[i]._M_left = l ? &n[l] : 0; n[i]._M_right = r ? &n[r] : 0;
}
int main()
{
    // header n[0]; tree: n3 root, n2 (left, with n1 left), n5 (right, with n4 left)
    u64 keys[6] = { 0, 5, K64(0x100000000), K64(0x1FFFFFFFF), K64(0x200000000), K64(0x300000005) };
    for (int i = 1; i < 6; ++i) n[i]._M_value_field.first = keys[i];
    // in-order: n1(5) < n2(0x1_00000000) < n3(0x1_FFFFFFFF) < n4(0x2_00000000) < n5(0x3_00000005)
    link(3, 0, 2, 5); link(2, 3, 1, 0); link(1, 2, 0, 0); link(5, 3, 4, 0); link(4, 5, 0, 0);
    n[0]._M_parent = &n[3];
    CNPCNameMapTree t; t._M_header = &n[0]; t._M_node_count = 5;
    int ok = 1;
    u64 q;
    q = 0;                   ok &= t._M_lower_bound(q) == &n[1];
    q = 5;                   ok &= t._M_lower_bound(q) == &n[1];
    q = 6;                   ok &= t._M_lower_bound(q) == &n[2];
    q = K64(0xFFFFFFFF);     ok &= t._M_lower_bound(q) == &n[2];
    q = K64(0x100000001);    ok &= t._M_lower_bound(q) == &n[3];
    q = K64(0x1FFFFFFFF);    ok &= t._M_lower_bound(q) == &n[3];
    q = K64(0x200000000);    ok &= t._M_lower_bound(q) == &n[4];
    q = K64(0x300000004);    ok &= t._M_lower_bound(q) == &n[5];
    q = K64(0x300000006);    ok &= t._M_lower_bound(q) == &n[0];
    n[0]._M_parent = 0;      ok &= t._M_lower_bound(q) == &n[0];
    if (!ok) { std::printf("00528610_TEST FAIL\n"); return 1; }
    std::printf("00528610_TEST PASS\n");
    return 0;
}