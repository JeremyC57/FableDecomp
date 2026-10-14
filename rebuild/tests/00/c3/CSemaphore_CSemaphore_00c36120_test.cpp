#include <cstdio>

static long g_initial = -1, g_maximum = -1; static void* g_attr = (void*)1; static const char* g_name = "x";
static void* __stdcall FakeCreateSemaphoreA(void* attributes, long initialCount, long maximumCount, const char* name)
{ g_attr = attributes; g_initial = initialCount; g_maximum = maximumCount; g_name = name; return (void*)0x77; }
struct CSemaphore { void* m_handle; CSemaphore(long initialCount, long maximumCount) { m_handle = FakeCreateSemaphoreA(0, initialCount, maximumCount, 0); } };
int main()
{
    CSemaphore s(0, 1);
    if (s.m_handle != (void*)0x77 || g_attr != 0 || g_name != 0 || g_initial != 0 || g_maximum != 1) { std::printf("00c36120_TEST FAIL\n"); return 1; }
    std::printf("00c36120_TEST PASS\n"); return 0;
}