#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static unsigned long g_timeout = 0, g_slept = 0; static void* g_closed = 0; static void Sleep(unsigned long ms) { g_slept = ms; std::strcat(g_seq, "S"); } static int CloseHandle(void* h) { g_closed = h; std::strcat(g_seq, "C"); return 1; }
struct CEvent { void* m_handle; unsigned char _pad[4]; bool Wait(unsigned long t) { g_timeout = t; std::strcat(g_seq, "W"); return true; } };
struct CMap { long m_0; CEvent m_finished; void* m_hThread; void CloseStaticMap() { m_finished.Wait(0xffffffff); Sleep(20); CloseHandle(m_hThread); m_hThread = 0; } };

int main() {
    CMap m; int h; m.m_finished.m_handle = (void*)3; m.m_hThread = &h; m.CloseStaticMap(); if (g_timeout != 0xffffffff || g_slept != 20 || g_closed != &h || m.m_hThread || std::strcmp(g_seq, "WSC")) { std::printf("00c2a2a0_TEST FAIL\n"); return 1; }
    std::printf("00c2a2a0_TEST PASS\n"); return 0;
}
