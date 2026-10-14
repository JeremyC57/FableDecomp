#include <cstdio>
#include <cstdlib>
#include <cstring>
static void* g_cleared = 0; static unsigned long g_flags = 0; struct CManager { void ClearErrorMessage(void* m) { g_cleared = m; } }; static CManager g_manager; static CManager* g_pManager = &g_manager;
struct COwner { unsigned char _pad[0x7c]; void* m_pMessage; void ClearErrorMessage() { if (m_pMessage) { g_pManager->ClearErrorMessage(m_pMessage); m_pMessage = 0; } g_flags |= 0x1000000; } };

int main() {
    COwner o; int msg; o.m_pMessage = &msg; g_flags = 1; o.ClearErrorMessage(); if (g_cleared != &msg || o.m_pMessage || g_flags != 0x1000001) { std::printf("00c40800_TEST FAIL\n"); return 1; } g_cleared = 0; o.ClearErrorMessage(); if (g_cleared || g_flags != 0x1000001) { std::printf("00c40800_TEST FAIL\n"); return 1; }
    std::printf("00c40800_TEST PASS\n"); return 0;
}