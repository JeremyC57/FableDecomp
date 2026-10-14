#include <cstdio>

static long g_initial = -1, g_maximum = -1;
struct CSemaphore { void* m_handle; CSemaphore(long initialCount, long maximumCount) : m_handle((void*)0x55) { g_initial = initialCount; g_maximum = maximumCount; } };
struct CVtable_012b4148 { virtual void v00() { } CSemaphore m_semaphore; void* m_pContext;
    CVtable_012b4148(void* context) : m_semaphore(0, 1), m_pContext(context) { } };
int main()
{
    int ctx; CVtable_012b4148 a(&ctx);
    if (a.m_pContext != &ctx || a.m_semaphore.m_handle != (void*)0x55 || g_initial != 0 || g_maximum != 1) { std::printf("00c2a230_TEST FAIL\n"); return 1; }
    std::printf("00c2a230_TEST PASS\n"); return 0;
}