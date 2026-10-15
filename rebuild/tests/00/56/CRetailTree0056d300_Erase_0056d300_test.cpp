#include <cstdio>
#include <cstdlib>
// Behaviour of CRetailTree_0056d300::Erase (0x0056D300): the node is unlinked by the rebalance helper, its value's three
// strings are destroyed (wide B, wide A, name), the node is freed and the node count drops by one.
static int g_log[8]; static int g_n = 0; static void* g_freed = 0;
static void TestFree(void* p) { g_freed = p; std::free(p); }
struct CCharString { void* m_pStringData; ~CCharString() { g_log[g_n++] = 1; } };
struct CWideString { void* m_pStringData; int m_tag; ~CWideString() { g_log[g_n++] = 10 + m_tag; } };
struct SValue { unsigned char m_key[0x10]; CCharString m_name; CWideString m_pathA; CWideString m_pathB; };
struct CRetailTree {
    struct SNode { long _M_color; SNode* _M_parent; SNode* _M_left; SNode* _M_right; SValue _M_value; };
    SNode* _M_header;
    unsigned long _M_node_count;
    void Erase(SNode* where);
};
static CRetailTree::SNode** g_args[3]; static CRetailTree::SNode* g_where = 0;
static CRetailTree::SNode* RebalanceForErase(CRetailTree::SNode* where, CRetailTree::SNode** root, CRetailTree::SNode** leftmost, CRetailTree::SNode** rightmost)
{
    g_where = where; g_args[0] = root; g_args[1] = leftmost; g_args[2] = rightmost; return where;
}
void CRetailTree::Erase(SNode* where)
{
    SNode* victim = RebalanceForErase(where, &_M_header->_M_parent, &_M_header->_M_left, &_M_header->_M_right);
    victim->_M_value.~SValue();
    if (victim)
        TestFree(victim);
    --_M_node_count;
}
int main()
{
    CRetailTree::SNode header;
    CRetailTree::SNode* node = (CRetailTree::SNode*)std::malloc(sizeof(CRetailTree::SNode));
    node->_M_value.m_pathA.m_tag = 1; node->_M_value.m_pathB.m_tag = 2;
    CRetailTree t; t._M_header = &header; t._M_node_count = 4;
    t.Erase(node);
    if (g_where != node || g_args[0] != &header._M_parent || g_args[1] != &header._M_left || g_args[2] != &header._M_right ||
        g_n != 3 || g_log[0] != 12 || g_log[1] != 11 || g_log[2] != 1 || g_freed != (void*)node || t._M_node_count != 3) { std::printf("0056d300_TEST FAIL\n"); return 1; }
    std::printf("0056d300_TEST PASS\n");
    return 0;
}