#include <cstdio>
#include <stdlib.h>
#include <new>
struct _Rb_tree_node_base {
    bool                _M_color;               // +0x00
    _Rb_tree_node_base* _M_parent;              // +0x04
    _Rb_tree_node_base* _M_left;                // +0x08
    _Rb_tree_node_base* _M_right;               // +0x0c
};
static int g_rebal = 0; static _Rb_tree_node_base* g_lastRebal = 0;
template <class _Dummy> struct _Rb_global {
    static void _Rebalance(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root) { ++g_rebal; g_lastRebal = __x; (void)__root; }
};
struct SValue {                                 // 4 bytes
    long Key;
};
struct SNode : _Rb_tree_node_base {
    SValue Value;                               // +0x10
};
struct SIterator {
    SNode* m_node;
};
struct CRbTree {                                 // STLport _Rb_tree: header, count
    SNode*        m_header;                     // +0x00
    unsigned long m_node_count;                 // +0x04
    static SNode* CreateNode(const SValue& value)
    {
        SNode* node = (SNode*)malloc(sizeof(SNode));
        new (&node->Value) SValue(value);
        return node;
    }
    SIterator* _M_insert(SIterator* result, SNode* x, SNode* y, const SValue& value, SNode* w);
};

SIterator* CRbTree::_M_insert(SIterator* result, SNode* x, SNode* y, const SValue& value, SNode* w)
{
    SNode* z;
    if (y == m_header || (w == 0 && (x != 0 || value.Key < y->Value.Key))) {
        z = CreateNode(value);
        y->_M_left = z;
        if (y == m_header) {
            m_header->_M_parent = z;
            m_header->_M_right = z;
        }
        else if (y == m_header->_M_left)
            m_header->_M_left = z;
    }
    else {
        z = CreateNode(value);
        y->_M_right = z;
        if (y == m_header->_M_right)
            m_header->_M_right = z;
    }
    z->_M_parent = y;
    z->_M_left = 0;
    z->_M_right = 0;
    _Rb_global<bool>::_Rebalance(z, m_header->_M_parent);
    ++m_node_count;
    result->m_node = z;
    return result;
}

static SValue Make(int k) { SValue v; v.Key = k; return v; }
static long KeyOf(const SNode* n) { return (long)n->Value.Key; }
static int fail() { std::printf("009c2440_TEST FAIL\n"); return 1; }
int main()
{
    SNode* header = (SNode*)malloc(sizeof(SNode));
    header->_M_color = false; header->_M_parent = 0; header->_M_left = header; header->_M_right = header;
    CRbTree tree; tree.m_header = header; tree.m_node_count = 0;
    SIterator r; r.m_node = 0;
    SIterator* ret = tree._M_insert(&r, 0, header, Make(5), 0);           // first node: root, leftmost, rightmost
    SNode* n5 = r.m_node;
    if (ret != &r || header->_M_parent != n5 || header->_M_left != n5 || header->_M_right != n5 || n5->_M_parent != header || KeyOf(n5) != 5 || tree.m_node_count != 1) return fail();
    tree._M_insert(&r, 0, n5, Make(3), 0);                                 // 3 < 5: left of n5, new leftmost
    SNode* n3 = r.m_node;
    if (n5->_M_left != n3 || header->_M_left != n3 || header->_M_right != n5 || n3->_M_parent != n5 || n3->_M_left != 0 || n3->_M_right != 0) return fail();
    tree._M_insert(&r, 0, n5, Make(7), 0);                                 // 7 > 5: right of n5, new rightmost
    SNode* n7 = r.m_node;
    if (n5->_M_right != n7 || header->_M_right != n7 || header->_M_left != n3 || tree.m_node_count != 3 || g_rebal != 3 || g_lastRebal != n7) return fail();
    tree._M_insert(&r, 0, n3, Make(4), n3);                                // w set: right of n3, not rightmost
    if (n3->_M_right != r.m_node || header->_M_right != n7 || KeyOf(r.m_node) != 4) return fail();
    tree._M_insert(&r, n7, n7, Make(9), 0);                                // x set: left of n7
    if (n7->_M_left != r.m_node || tree.m_node_count != 5) return fail();
    std::printf("009c2440_TEST PASS\n");
    return 0;
}
