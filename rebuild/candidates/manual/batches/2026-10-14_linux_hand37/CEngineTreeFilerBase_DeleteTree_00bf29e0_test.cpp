#include <cstdio>
#include <cstdlib>
#include <cstring>
struct STreeNode_00bf29e0; static int g_freed = 0; static STreeNode_00bf29e0* g_order[8];
struct STreeNode_00bf29e0 { unsigned char _pad[0xc]; STreeNode_00bf29e0* m_pNextSibling; STreeNode_00bf29e0* m_pFirstChild; };
struct CEngineTreeFilerBase { void DeleteTree(STreeNode_00bf29e0* node) { STreeNode_00bf29e0* child = node->m_pFirstChild; while (child) { STreeNode_00bf29e0* next = child->m_pNextSibling; DeleteTree(child); child = next; } g_order[g_freed++] = node; delete node; } };
int main() {
    CEngineTreeFilerBase f; STreeNode_00bf29e0* root = new STreeNode_00bf29e0; STreeNode_00bf29e0* c1 = new STreeNode_00bf29e0; STreeNode_00bf29e0* c2 = new STreeNode_00bf29e0; STreeNode_00bf29e0* gc = new STreeNode_00bf29e0;
    root->m_pFirstChild = c1; c1->m_pNextSibling = c2; c1->m_pFirstChild = gc; c2->m_pNextSibling = 0; c2->m_pFirstChild = 0; gc->m_pNextSibling = 0; gc->m_pFirstChild = 0; root->m_pNextSibling = 0;
    f.DeleteTree(root); if (g_freed != 4 || g_order[0] != gc || g_order[1] != c1 || g_order[2] != c2 || g_order[3] != root) { std::printf("00bf29e0_TEST FAIL\n"); return 1; }
    std::printf("00bf29e0_TEST PASS\n"); return 0;
}
