#include <cstdio>
#include <cstdlib>
#include <cstring>
static void* g_closed = 0; static int CloseHandle(void* h) { g_closed = h; return 1; }
static bool g_stopped = false; struct COwner { virtual void Stop() { g_stopped = true; } long m_4; void* m_hHandle; void Shutdown() { Stop(); if (m_hHandle != (void*)-1) { CloseHandle(m_hHandle); m_hHandle = (void*)-1; } } };

int main() {
    COwner o; int h; o.m_hHandle = &h; o.Shutdown(); if (!g_stopped || g_closed != &h || o.m_hHandle != (void*)-1) { std::printf("00c5e630_TEST FAIL\n"); return 1; } g_closed = 0; o.Shutdown(); if (g_closed) { std::printf("00c5e630_TEST FAIL\n"); return 1; }
    std::printf("00c5e630_TEST PASS\n"); return 0;
}
