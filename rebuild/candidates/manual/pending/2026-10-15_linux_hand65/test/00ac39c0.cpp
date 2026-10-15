#include <cstdio>
#include <cstdlib>
// SBinaryTreeNode_00ac39c0 deleting destructor: both children deleted recursively, the item array freed, the node deleted
static long g_nodesDeleted, g_arraysFreed;
struct CItemArray {
    void* m_pFirst; void* m_pLast; void* m_pEnd;
    CItemArray() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
    ~CItemArray() { if (m_pFirst) { std::free(m_pFirst); ++g_arraysFreed; } }
};
struct SBinaryTreeNode {
    unsigned char _payload[0x30];
    CItemArray m_items;
    SBinaryTreeNode* m_pChildA;
    SBinaryTreeNode* m_pChildB;
    SBinaryTreeNode() : m_pChildA(0), m_pChildB(0) { }
    ~SBinaryTreeNode() { delete m_pChildA; delete m_pChildB; ++g_nodesDeleted; }
};
int main()
{
    SBinaryTreeNode* root = new SBinaryTreeNode;
    root->m_pChildA = new SBinaryTreeNode;
    root->m_pChildB = new SBinaryTreeNode;
    root->m_pChildA->m_pChildB = new SBinaryTreeNode;
    root->m_items.m_pFirst = std::malloc(8);
    root->m_pChildA->m_pChildB->m_items.m_pFirst = std::malloc(8);
    delete root;
    if (g_nodesDeleted != 4 || g_arraysFreed != 2) { std::printf("00ac39c0_TEST FAIL\n"); return 1; }
    std::printf("00ac39c0_TEST PASS\n");
    return 0;
}
