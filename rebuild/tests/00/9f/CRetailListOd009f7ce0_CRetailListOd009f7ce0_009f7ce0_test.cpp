#include <cstdio>

static int g_clears = 0; static void* g_freed = 0;
static void FakeFree(void* p) { g_freed = p; }
struct CRetailListOd_009f7ce0 { struct SNode { SNode* m_pNext; SNode* m_pPrev; }; SNode* m_pHead;
    void Clear() { ++g_clears; }
    ~CRetailListOd_009f7ce0() { Clear(); SNode* head = m_pHead; if (head) FakeFree(head); } };
int main()
{
    CRetailListOd_009f7ce0::SNode node;
    { CRetailListOd_009f7ce0 l; l.m_pHead = &node; }
    if (g_clears != 1 || g_freed != &node) { std::printf("009f7ce0_TEST FAIL\n"); return 1; }
    g_freed = 0;
    { CRetailListOd_009f7ce0 l; l.m_pHead = 0; }
    if (g_clears != 2 || g_freed != 0) { std::printf("009f7ce0_TEST FAIL\n"); return 1; }
    std::printf("009f7ce0_TEST PASS\n"); return 0;
}