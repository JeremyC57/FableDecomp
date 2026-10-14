#include <cstdio>
#include <cstdlib>
#include <cstring>
static void* g_waited = 0; static int g_closed = 0; static void* g_closedH[4]; static int g_deleted = 0;
static long InterlockedExchange(long volatile* t, long v) { long o = *t; *t = v; return o; }
static unsigned long WaitForSingleObject(void* h, unsigned long) { g_waited = h; return 0; }
static int CloseHandle(void* h) { g_closedH[g_closed++] = h; return 1; }
struct _RTL_CRITICAL_SECTION { unsigned char _body[0x18]; }; static void DeleteCriticalSection(_RTL_CRITICAL_SECTION*) { ++g_deleted; }
struct CWorkerThread_00cb0d20 { long m_0; void* m_hEventA; void* m_hEventB; unsigned char _pad0c[8]; long volatile m_hThread; _RTL_CRITICAL_SECTION m_sectionA; _RTL_CRITICAL_SECTION m_sectionB;
    ~CWorkerThread_00cb0d20() { void* t = (void*)InterlockedExchange(&m_hThread, 0); if (t) { WaitForSingleObject(t, 0xffffffff); CloseHandle(t); } DeleteCriticalSection(&m_sectionB); DeleteCriticalSection(&m_sectionA); if (m_hEventB) CloseHandle(m_hEventB); if (m_hEventA) CloseHandle(m_hEventA); } };
int main() {
    { CWorkerThread_00cb0d20 w; std::memset(&w, 0, sizeof w); w.m_hThread = 7; w.m_hEventA = (void*)3; w.m_hEventB = 0; }
    if (g_waited != (void*)7 || g_closed != 2 || g_closedH[0] != (void*)7 || g_closedH[1] != (void*)3 || g_deleted != 2) { std::printf("00cb0d20_TEST FAIL\n"); return 1; }
    { CWorkerThread_00cb0d20 w; std::memset(&w, 0, sizeof w); } if (g_closed != 2 || g_deleted != 4) { std::printf("00cb0d20_TEST FAIL\n"); return 1; }
    std::printf("00cb0d20_TEST PASS\n"); return 0;
}