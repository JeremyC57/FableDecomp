#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_args[4]; static void* CreateEventStub(void* a, int b, int c, const unsigned short* d) { g_args[0] = (long)(size_t)a; g_args[1] = b; g_args[2] = c; g_args[3] = (long)(size_t)d; return (void*)&g_args; }
struct CEvent { void* m_hEvent; CEvent(int manualReset) { m_hEvent = CreateEventStub(0, manualReset, 0, 0); } };

int main() {
    CEvent e(1); if (e.m_hEvent != (void*)&g_args || g_args[0] != 0 || g_args[1] != 1 || g_args[2] != 0 || g_args[3] != 0) { std::printf("00cb0190_TEST FAIL\n"); return 1; } CEvent f(0); if (g_args[1] != 0) { std::printf("00cb0190_TEST FAIL\n"); return 1; }
    std::printf("00cb0190_TEST PASS\n"); return 0;
}
