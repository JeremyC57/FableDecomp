#include <cstdio>
#include <cstring>
// Behaviour of 0x00CA5540 (chunk 901 piece): Sleep(interval / 10000) for a positive interval, else Sleep(0)
static unsigned long g_ms = 99; static int g_calls = 0;
static void Sleep(unsigned long ms) { g_ms = ms; ++g_calls; }
struct CLibraryObject_Retail {
    unsigned char _pad_0x00[0xf0];
    long m_interval;
    void func_00ca5540();
};
void CLibraryObject_Retail::func_00ca5540() { if (m_interval > 0) Sleep(m_interval / 10000); else Sleep(0); }
int main()
{
    CLibraryObject_Retail o; o.m_interval = 250000; o.func_00ca5540(); unsigned long a = g_ms;
    o.m_interval = -3; o.func_00ca5540(); unsigned long b = g_ms;
    o.m_interval = 9999; o.func_00ca5540(); unsigned long c = g_ms;
    if (a != 25 || b != 0 || c != 0 || g_calls != 3) { std::printf("00ca5540_TEST FAIL\n"); return 1; }
    std::printf("00ca5540_TEST PASS\n"); return 0;
}
