#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_n = 0; static void* g_last = 0; struct CBank { void FreeDeviceSpecificResources() { ++g_n; g_last = this; } }; struct SNode { SNode* m_pNext; SNode* m_pPrev; long m_8; CBank* m_pBank; };
struct CManager { virtual void v000() { } SNode* m_pBanks; virtual void OnPreDeviceReset() { for (SNode* n = m_pBanks->m_pNext; n != m_pBanks; n = n->m_pNext) n->m_pBank->FreeDeviceSpecificResources(); } };

int main() {
    CBank b1, b2; SNode head, n1, n2; head.m_pNext = &n1; n1.m_pNext = &n2; n2.m_pNext = &head; n1.m_pBank = &b1; n2.m_pBank = &b2; CManager m; m.m_pBanks = &head; m.OnPreDeviceReset(); if (g_n != 2 || g_last != &b2) { std::printf("00a09d90_TEST FAIL\n"); return 1; } head.m_pNext = &head; g_n = 0; m.OnPreDeviceReset(); if (g_n) { std::printf("00a09d90_TEST FAIL\n"); return 1; }
    std::printf("00a09d90_TEST PASS\n"); return 0;
}