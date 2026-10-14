#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_frees = 0; static void* g_freed[4]; static void FreeBlock(void* p) { if (g_frees < 4) g_freed[g_frees] = p; ++g_frees; std::free(p); }
struct SNode { unsigned char _pad[4]; SNode* m_pParent; SNode* m_pLeft; SNode* m_pRight; };
static SNode* g_erased = 0; static SNode* g_left = 0; static SNode* g_right = 0; static SNode* g_parent = (SNode*)1; static long g_size = -1;
struct CTree { SNode* m_pHead; long m_size; void Erase(SNode* n) { g_erased = n; }
    ~CTree() { if (m_size != 0) { Erase(m_pHead->m_pParent); m_pHead->m_pLeft = m_pHead; m_pHead->m_pParent = 0; m_pHead->m_pRight = m_pHead; m_size = 0; }
        if (m_pHead) { g_left = m_pHead->m_pLeft; g_right = m_pHead->m_pRight; g_parent = m_pHead->m_pParent; g_size = m_size; FreeBlock(m_pHead); } } };

int main()
{
    CTree* t = new CTree; SNode* head = (SNode*)std::malloc(sizeof(SNode)); SNode root; head->m_pParent = &root; head->m_pLeft = 0; head->m_pRight = 0; t->m_pHead = head; t->m_size = 3;
    delete t; if (g_erased != &root || g_frees != 1 || g_freed[0] != head || g_left != head || g_right != head || g_parent != 0 || g_size != 0) { std::printf("004ae7b0_TEST FAIL\n"); return 1; }
    g_erased = 0; g_frees = 0; t = new CTree; t->m_pHead = 0; t->m_size = 0; delete t; if (g_erased || g_frees) { std::printf("004ae7b0_TEST FAIL\n"); return 1; }
    std::printf("004ae7b0_TEST PASS\n");
    return 0;
}
