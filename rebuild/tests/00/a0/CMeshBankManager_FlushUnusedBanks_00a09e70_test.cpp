#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SInfo { long m_state; }; struct SNode { SNode* m_pNext; SNode* m_pPrev; unsigned char _pad[8]; SInfo* m_pInfo; }; static SNode* g_erased = 0; static long g_count = 0;
struct CIt { SNode* m_pNode; CIt(SNode* n) : m_pNode(n) { } CIt(const CIt& o) : m_pNode(o.m_pNode) { } }; struct CList { SNode* m_pHead; CIt erase(CIt w) { g_erased = w.m_pNode; ++g_count; return CIt(w.m_pNode->m_pNext); } };
struct CManager { long m_0, m_4; CList m_infos; void FlushUnusedBanks() { for (SNode* n = m_infos.m_pHead->m_pNext; n != m_infos.m_pHead; n = n->m_pNext) { SInfo* i = n->m_pInfo; if (i && i->m_state == 1) { m_infos.erase(CIt(n)); return; } } } };

int main() {
    SInfo i1, i2, i3; i1.m_state = 0; i2.m_state = 1; i3.m_state = 1; SNode head, n1, n2, n3; head.m_pNext = &n1; n1.m_pNext = &n2; n2.m_pNext = &n3; n3.m_pNext = &head; n1.m_pInfo = &i1; n2.m_pInfo = &i2; n3.m_pInfo = &i3; CManager m; m.m_infos.m_pHead = &head;
    m.FlushUnusedBanks(); if (g_erased != &n2 || g_count != 1) { std::printf("00a09e70_TEST FAIL\n"); return 1; } n1.m_pInfo = 0; i2.m_state = 0; i3.m_state = 0; g_erased = 0; m.FlushUnusedBanks(); if (g_erased || g_count != 1) { std::printf("00a09e70_TEST FAIL\n"); return 1; }
    std::printf("00a09e70_TEST PASS\n"); return 0;
}