// Behaviour test for 005293d0: a copy of the source's model and logic with its out-of-line callees stubbed.
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
class CCharString { public: const char* PData; };
class CWideString { public: const unsigned short* PData; };
struct SNPCName {                          // pair<CCharString, CWideString>
    CCharString first;
    CWideString second;
};
struct SValue {                            // pair<const unsigned __int64, pair<CCharString, CWideString> >
    u64      first;
    SNPCName second;
};
static char g_names[128];
static unsigned short g_wnames[128];
static SValue MakeValue(int k)
{
    SValue v;
    v.first = (u64)k << 32;
    v.second.first.PData = g_names + k;
    v.second.second.PData = g_wnames + k;
    return v;
}
static int KeyOf(const SValue& v) { return (int)(v.first >> 32); }
static bool ExtraOk(const SValue& v, int k) { return v.second.first.PData == g_names + k && v.second.second.PData == g_wnames + k; }
struct SNode : _Rb_tree_node_base {
    SValue _M_value_field;
};

static int g_rebalanceCalls = 0;
static _Rb_tree_node_base* g_rebalanceNode = 0;
static _Rb_tree_node_base** g_rebalanceRoot = 0;
static void Rebalance(_Rb_tree_node_base* node, _Rb_tree_node_base*& root)     // stub for 0x0042971D
{
    ++g_rebalanceCalls;
    g_rebalanceNode = node;
    g_rebalanceRoot = &root;
}

struct CTree {
    _Rb_tree_node_base* _M_header;
    unsigned long       _M_node_count;
    struct iterator {
        _Rb_tree_node_base* _M_node;
        iterator(_Rb_tree_node_base* node) : _M_node(node) { }
    };
    SNode* _M_create_node(const SValue& v)
    {
        SNode* tmp = (SNode*)std::malloc(sizeof(SNode));
        tmp->_M_value_field = v;
        return tmp;
    }
    iterator _M_insert(_Rb_tree_node_base* x_, _Rb_tree_node_base* y_, const SValue& v, _Rb_tree_node_base* w_)
    {
        SNode* w = (SNode*)w_;
        SNode* y = (SNode*)y_;
        SNode* x = (SNode*)x_;
        SNode* z;
        if (y == _M_header || (w == 0 && (x != 0 || v.first < y->_M_value_field.first))) {
            z = _M_create_node(v);
            y->_M_left = z;
            if (y == _M_header) {
                _M_header->_M_parent = z;
                _M_header->_M_right = z;
            }
            else if (y == _M_header->_M_left)
                _M_header->_M_left = z;
        }
        else {
            z = _M_create_node(v);
            y->_M_right = z;
            if (y == _M_header->_M_right)
                _M_header->_M_right = z;
        }
        z->_M_parent = y;
        z->_M_left = 0;
        z->_M_right = 0;
        Rebalance(z, _M_header->_M_parent);
        ++_M_node_count;
        return iterator(z);
    }
};

static void RunTest()
{
    _Rb_tree_node_base header;
    header._M_color = false;
    header._M_parent = 0;
    header._M_left = &header;
    header._M_right = &header;
    CTree t;
    t._M_header = &header;
    t._M_node_count = 0;

    // empty tree: y is the header -> the new node is root, leftmost and rightmost
    SNode* n1 = (SNode*)t._M_insert(0, &header, MakeValue(50), 0)._M_node;
    CHECK(header._M_parent == n1 && header._M_left == n1 && header._M_right == n1);
    CHECK(n1->_M_parent == &header && n1->_M_left == 0 && n1->_M_right == 0);
    CHECK(KeyOf(n1->_M_value_field) == 50 && ExtraOk(n1->_M_value_field, 50));
    CHECK(t._M_node_count == 1 && g_rebalanceCalls == 1 && g_rebalanceNode == n1);
    CHECK(g_rebalanceRoot == &header._M_parent);

    // smaller key under the root: left child and the new leftmost
    SNode* n2 = (SNode*)t._M_insert(0, n1, MakeValue(20), 0)._M_node;
    CHECK(n1->_M_left == n2 && n2->_M_parent == n1 && header._M_left == n2 && header._M_right == n1);

    // larger key under the root: right child and the new rightmost
    SNode* n3 = (SNode*)t._M_insert(0, n1, MakeValue(80), 0)._M_node;
    CHECK(n1->_M_right == n3 && n3->_M_parent == n1 && header._M_right == n3 && header._M_left == n2);

    // a non-null x forces the left side even for a larger key; leftmost/rightmost unchanged
    SNode* n4 = (SNode*)t._M_insert(n3, n3, MakeValue(90), 0)._M_node;
    CHECK(n3->_M_left == n4 && header._M_right == n3 && header._M_left == n2);

    // a non-null w forces the right side even for a smaller key
    SNode* n5 = (SNode*)t._M_insert(0, n2, MakeValue(10), n2)._M_node;
    CHECK(n2->_M_right == n5 && n2->_M_left == 0 && header._M_left == n2);
    CHECK(KeyOf(n5->_M_value_field) == 10 && ExtraOk(n5->_M_value_field, 10));
    CHECK(t._M_node_count == 5 && g_rebalanceCalls == 5 && g_rebalanceNode == n5);

    std::free(n1); std::free(n2); std::free(n3); std::free(n4); std::free(n5);
}

int main()
{
    RunTest();
    if (g_fail) {
        std::printf("005293d0_TEST FAIL\n");
        return 1;
    }
    std::printf("005293d0_TEST PASS\n");
    return 0;
}