#include <cstdio>
static int g_deleted = 0; static void* g_order[3];
struct SNode { unsigned char _pad[4]; SNode* m_pNext; static void operator delete(void* p) { g_order[g_deleted++] = p; ::operator delete(p); } };
struct COwner { unsigned char _pad[8]; SNode* m_pHead; void DeleteNodeChain() { SNode* n = m_pHead; while (n) { SNode* victim = n; n = n->m_pNext; delete victim; } } };
int main()
{
    COwner o; o.m_pHead = 0; o.DeleteNodeChain(); if (g_deleted != 0) { std::printf("00cb1210_TEST FAIL\n"); return 1; }
    SNode* c = new SNode; c->m_pNext = 0; SNode* b = new SNode; b->m_pNext = c; SNode* a = new SNode; a->m_pNext = b; o.m_pHead = a; o.DeleteNodeChain();
    if (g_deleted != 3 || g_order[0] != a || g_order[1] != b || g_order[2] != c) { std::printf("00cb1210_TEST FAIL\n"); return 1; }
    std::printf("00cb1210_TEST PASS\n"); return 0;
}
