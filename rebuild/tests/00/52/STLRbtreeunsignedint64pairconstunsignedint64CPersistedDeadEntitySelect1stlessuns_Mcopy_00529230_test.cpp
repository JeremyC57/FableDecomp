// Behaviour test for 00529230: a copy of the source's model and logic with its out-of-line callees stubbed.
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

struct CPersistedDeadEntity {
    u64           UID;
    bool          GamePersistent;
    unsigned long SectionNameCRC;
};
struct SDeadEntityPair {
    u64                  first;
    CPersistedDeadEntity second;
};
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
struct SDeadEntityNode : _Rb_tree_node_base {
    SDeadEntityPair _M_value_field;
};
static int g_allocs = 0;
struct CDeadEntityTree {
    _Rb_tree_node_base* _M_header;
    unsigned long       _M_node_count;
    static _Rb_tree_node_base*& _S_left(_Rb_tree_node_base* x) { return x->_M_left; }
    static _Rb_tree_node_base*& _S_right(_Rb_tree_node_base* x) { return x->_M_right; }
    SDeadEntityNode* _M_create_node(const SDeadEntityPair& v)
    {
        SDeadEntityNode* tmp = (SDeadEntityNode*)std::malloc(sizeof(SDeadEntityNode));
        ++g_allocs;
        tmp->_M_value_field = v;
        return tmp;
    }
    SDeadEntityNode* _M_clone_node(SDeadEntityNode* x)
    {
        SDeadEntityNode* tmp = _M_create_node(x->_M_value_field);
        tmp->_M_color = x->_M_color;
        tmp->_M_left = 0;
        tmp->_M_right = 0;
        return tmp;
    }
    SDeadEntityNode* _M_copy(SDeadEntityNode* x, SDeadEntityNode* p)
    {
        SDeadEntityNode* top = _M_clone_node(x);
        top->_M_parent = p;
        if (x->_M_right)
            top->_M_right = _M_copy((SDeadEntityNode*)_S_right(x), top);
        p = top;
        x = (SDeadEntityNode*)_S_left(x);
        while (x != 0) {
            SDeadEntityNode* y = _M_clone_node(x);
            p->_M_left = y;
            y->_M_parent = p;
            if (x->_M_right)
                y->_M_right = _M_copy((SDeadEntityNode*)_S_right(x), y);
            p = y;
            x = (SDeadEntityNode*)_S_left(x);
        }
        return top;
    }
};

static SDeadEntityNode g_src[7];
static void Link(int parent, int child, bool left)
{
    if (left) g_src[parent]._M_left = &g_src[child]; else g_src[parent]._M_right = &g_src[child];
    g_src[child]._M_parent = &g_src[parent];
}
// structural equality: same shape, values, colours, and correct parent links in the copy
static bool Same(const _Rb_tree_node_base* a, const _Rb_tree_node_base* b, const _Rb_tree_node_base* bParent)
{
    if (a == 0 || b == 0) return a == b;
    const SDeadEntityNode* na = (const SDeadEntityNode*)a;
    const SDeadEntityNode* nb = (const SDeadEntityNode*)b;
    if (a == b || nb->_M_parent != bParent || na->_M_color != nb->_M_color) return false;
    if (na->_M_value_field.first != nb->_M_value_field.first) return false;
    if (na->_M_value_field.second.SectionNameCRC != nb->_M_value_field.second.SectionNameCRC) return false;
    return Same(a->_M_left, b->_M_left, b) && Same(a->_M_right, b->_M_right, b);
}
static void FreeTree(_Rb_tree_node_base* n)
{
    if (n == 0) return;
    FreeTree(n->_M_left);
    FreeTree(n->_M_right);
    std::free(n);
}
static void RunTest()
{
    int i;
    for (i = 0; i < 7; ++i) {
        g_src[i]._M_color = (i & 1) != 0;
        g_src[i]._M_parent = 0;
        g_src[i]._M_left = 0;
        g_src[i]._M_right = 0;
        g_src[i]._M_value_field.first = (u64)(i + 1) << 33;
        g_src[i]._M_value_field.second.UID = g_src[i]._M_value_field.first;
        g_src[i]._M_value_field.second.GamePersistent = true;
        g_src[i]._M_value_field.second.SectionNameCRC = 0xABC0u + i;
    }
    // 3 is the root; 1 (left: 0, 2), 5 (left: 4, right: 6); plus a long left spine handled by the loop
    Link(3, 1, true); Link(3, 5, false); Link(1, 0, true); Link(1, 2, false); Link(5, 4, true); Link(5, 6, false);
    _Rb_tree_node_base header;
    CDeadEntityTree t;
    t._M_header = &header;
    t._M_node_count = 7;
    SDeadEntityNode* copy = t._M_copy(&g_src[3], (SDeadEntityNode*)&header);
    CHECK(copy != &g_src[3] && copy->_M_parent == &header);
    CHECK(Same(&g_src[3], copy, &header));
    CHECK(g_allocs == 7);
    FreeTree(copy);
    // a single node: no recursion, no loop
    g_allocs = 0;
    SDeadEntityNode* single = t._M_copy(&g_src[0], (SDeadEntityNode*)&header);
    CHECK(g_allocs == 1 && single->_M_left == 0 && single->_M_right == 0 && single->_M_parent == &header);
    std::free(single);
}

int main()
{
    RunTest();
    if (g_fail) {
        std::printf("00529230_TEST FAIL\n");
        return 1;
    }
    std::printf("00529230_TEST PASS\n");
    return 0;
}