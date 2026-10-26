// Behaviour test for 0x007AD180 _Rb_tree<CCharString, pair<const CCharString,bool>>::erase(first, last).
#include <cstdio>
#include <cstdlib>

static int g_strDtors = 0, g_frees = 0, g_subtreeErases = 0;
class CCharString { public: const char* p; ~CCharString() { ++g_strDtors; } };
struct SNode { long c; SNode* m_parent; SNode* m_left; SNode* m_right; CCharString m_key; bool m_value; };

// rebalance stand-in for the test: the nodes here are unlinked by their caller's bookkeeping only
static SNode* Rebalance(SNode* node, SNode**, SNode**, SNode**) { return node; }
static void Free(void* p) { if (p) ++g_frees; }

struct Tree {
    SNode* m_header;
    unsigned int m_node_count;
    void _M_erase(SNode*) { ++g_subtreeErases; }
    static SNode* _M_increment(SNode* node)
    {
        if (node->m_right != 0) { node = node->m_right; while (node->m_left != 0) node = node->m_left; }
        else { SNode* y = node->m_parent; while (node == y->m_right) { node = y; y = y->m_parent; } if (node->m_right != y) node = y; }
        return node;
    }
    void erase(SNode* first, SNode* last)
    {
        if (first == m_header->m_left && last == m_header) {
            if (m_node_count != 0) {
                _M_erase(m_header->m_parent);
                m_header->m_left = m_header; m_header->m_parent = 0; m_header->m_right = m_header; m_node_count = 0;
            }
        } else {
            while (first != last) {
                SNode* victim = first;
                first = _M_increment(first);
                SNode* node = Rebalance(victim, &m_header->m_parent, &m_header->m_left, &m_header->m_right);
                node->m_key.~CCharString();
                if (node) Free(node);
                --m_node_count;
            }
        }
    }
};

int main()
{
    // header H, root B with children A (left) and C (right): in-order A, B, C
    SNode H, A, B, C;
    H.m_parent = &B; H.m_left = &A; H.m_right = &C;
    B.m_parent = &H; B.m_left = &A; B.m_right = &C;
    A.m_parent = &B; A.m_left = 0; A.m_right = 0;
    C.m_parent = &B; C.m_left = 0; C.m_right = 0;
    Tree t; t.m_header = &H; t.m_node_count = 3;
    // partial range [A, C): erases A and B
    t.erase(&A, &C);
    if (t.m_node_count != 1 || g_strDtors != 2 || g_frees != 2 || g_subtreeErases != 0) { std::printf("007ad180_TEST FAIL\n"); return 1; }
    // full range on a fresh tree: cleared through _M_erase
    H.m_parent = &B; H.m_left = &A; H.m_right = &C; t.m_node_count = 3;
    t.erase(&A, &H);
    if (t.m_node_count != 0 || g_subtreeErases != 1 || H.m_left != &H || H.m_right != &H || H.m_parent != 0) { std::printf("007ad180_TEST FAIL\n"); return 1; }
    // full range of an empty tree: nothing
    t.erase(H.m_left, &H);
    if (g_subtreeErases != 1) { std::printf("007ad180_TEST FAIL\n"); return 1; }
    std::printf("007ad180_TEST PASS\n");
    return 0;
}