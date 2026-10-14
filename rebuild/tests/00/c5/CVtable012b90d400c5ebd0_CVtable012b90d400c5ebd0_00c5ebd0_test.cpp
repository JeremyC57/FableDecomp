#include <cstdio>
#include <cstdlib>
#include <cstring>
static void* g_closed = 0; static int CloseHandle(void* h) { g_closed = h; return 1; }
struct CObj { virtual void v00() { } void* m_hHandle; ~CObj() { if (m_hHandle != (void*)-1) { CloseHandle(m_hHandle); m_hHandle = (void*)-1; } } };

int main() {
    int h; { CObj o; o.m_hHandle = &h; } if (g_closed != &h) { std::printf("00c5ebd0_TEST FAIL\n"); return 1; } g_closed = 0; { CObj o; o.m_hHandle = (void*)-1; } if (g_closed) { std::printf("00c5ebd0_TEST FAIL\n"); return 1; }
    std::printf("00c5ebd0_TEST PASS\n"); return 0;
}