// Test for 0x00C36170 CSemaphore::Release: ReleaseSemaphore(handle, 1, 0) (a recording stand-in for the import).
#include <cstdio>
static void* g_h = 0; static long g_n = 0; static long* g_prev = (long*)1;
static int ReleaseSemaphoreStub(void* h, long n, long* prev) { g_h = h; g_n = n; g_prev = prev; return 1; }
struct CSemaphore { void* m_handle; void Release() { ReleaseSemaphoreStub(m_handle, 1, 0); } };
int main()
{
    int dummy; CSemaphore s; s.m_handle = &dummy; s.Release();
    if (g_h != &dummy || g_n != 1 || g_prev != 0) { std::printf("00c36170_TEST FAIL\n"); return 1; }
    std::printf("00c36170_TEST PASS\n");
    return 0;
}