// Test for 0x00C38B10 CActivationReceptor_00c38b20::Final: one message word is passed through the queue.
#include <cstdio>
static int g_calls = 0; static void* g_last = 0;
struct CQueue { void* DispatchMessage(void* m) { ++g_calls; g_last = m; return m; } };
struct CReceptor { CQueue m_queue; void Final() { int message; m_queue.DispatchMessage(&message); } };
int main()
{
    CReceptor r; r.Final();
    if (g_calls != 1 || g_last == 0) { std::printf("00c38b10_TEST FAIL\n"); return 1; }
    std::printf("00c38b10_TEST PASS\n");
    return 0;
}