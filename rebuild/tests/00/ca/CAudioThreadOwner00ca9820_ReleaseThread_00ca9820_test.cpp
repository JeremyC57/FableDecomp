#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
typedef long long Int64;
typedef unsigned long long UInt64;
#else
typedef __int64 Int64;
typedef unsigned __int64 UInt64;
#endif
// Behaviour test for 00ca9820: ReleaseThread resets the event, signals and closes the thread handle, sets the state to 2 and clears the handle.
static void* g_calls[3][2]; static int g_nCalls = 0;
static int TestResetEvent(void* h) { g_calls[g_nCalls][0] = (void*)1; g_calls[g_nCalls++][1] = h; return 1; }
static int TestSetEvent(void* h) { g_calls[g_nCalls][0] = (void*)2; g_calls[g_nCalls++][1] = h; return 1; }
static int TestCloseHandle(void* h) { g_calls[g_nCalls][0] = (void*)3; g_calls[g_nCalls++][1] = h; return 1; }
struct CAudioThreadOwner {
    void* m_hEvent;
    void* m_hThread;
    long m_state;
    void ReleaseThread();
};
void CAudioThreadOwner::ReleaseThread()
{
    TestResetEvent(m_hEvent);
    TestSetEvent(m_hThread);
    TestCloseHandle(m_hThread);
    m_state = 2;
    m_hThread = 0;
}
int main()
{
    int ev, th;
    CAudioThreadOwner o; o.m_hEvent = &ev; o.m_hThread = &th; o.m_state = 1;
    o.ReleaseThread();
    if (g_nCalls != 3 || g_calls[0][0] != (void*)1 || g_calls[0][1] != &ev || g_calls[1][0] != (void*)2 || g_calls[1][1] != &th ||
        g_calls[2][0] != (void*)3 || g_calls[2][1] != &th || o.m_state != 2 || o.m_hThread != 0 || o.m_hEvent != &ev) { std::printf("00ca9820_TEST FAIL\n"); return 1; }
    std::printf("00ca9820_TEST PASS\n");
    return 0;
}