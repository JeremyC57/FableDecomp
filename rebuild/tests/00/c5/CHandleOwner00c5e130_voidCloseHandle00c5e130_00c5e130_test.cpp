#include <cstdio>
#include <cstdlib>
#include <cstring>
static void* g_closed = 0; static int CloseHandle(void* h) { g_closed = h; return 1; }
struct COwner { long m_0; void* m_hHandle; void Close() { if (m_hHandle) { CloseHandle(m_hHandle); m_hHandle = 0; } } };

int main() {
    COwner o; int h; o.m_hHandle = &h; o.Close(); if (g_closed != &h || o.m_hHandle) { std::printf("00c5e130_TEST FAIL\n"); return 1; } g_closed = 0; o.Close(); if (g_closed) { std::printf("00c5e130_TEST FAIL\n"); return 1; }
    std::printf("00c5e130_TEST PASS\n"); return 0;
}