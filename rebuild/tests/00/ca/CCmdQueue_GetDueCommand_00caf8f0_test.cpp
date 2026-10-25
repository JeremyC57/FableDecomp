// Behaviour test for 00caf8f0: CCmdQueue::GetDueCommand -- earliest of the two list heads, due check, AddRef, wait / E_ABORT
#include <cstdio>
static int g_held = 0, g_waits = 0, g_addref = 0; static unsigned long g_waitResult = 0; static int g_dueAfterWaits = 0;
struct CDeferredCommand { __int64 m_time; int m_bStream; __int64 GetTime() { return m_time; } int IsStreamTime() { return m_bStream; } void AddRef() { ++g_addref; } };
struct CList { CDeferredCommand* m_head; int GetCount() const { return m_head ? 1 : 0; } CDeferredCommand* Get() const { return m_head; } };
static unsigned long WaitForSingleObject(void*, unsigned long) { ++g_waits; return g_waitResult; }
struct CCmdQueue {
    CList m_listPresentation, m_listStream; int m_bRunning; __int64 m_StreamTimeOffset; __int64 m_now; CDeferredCommand* m_checked;
    int CheckTime(__int64 time, int bStream) { if (bStream) time += m_StreamTimeOffset; return g_waits >= g_dueAfterWaits && time <= m_now; }
    long GetDueCommand(CDeferredCommand** ppCmd, long msTimeout)
    {
        for (;;) {
            {
                ++g_held;
                CDeferredCommand* pCmd = 0;
                if (m_listPresentation.GetCount() > 0) pCmd = m_listPresentation.Get();
                if (m_bRunning && (m_listStream.GetCount() > 0)) {
                    CDeferredCommand* pStrm = m_listStream.Get();
                    __int64 t = pStrm->GetTime() + m_StreamTimeOffset;
                    if (!pCmd || (t < pCmd->GetTime())) pCmd = pStrm;
                }
                if (pCmd) {
                    if (CheckTime(pCmd->GetTime(), pCmd->IsStreamTime())) { pCmd->AddRef(); *ppCmd = pCmd; --g_held; return 0; }
                }
                --g_held;
            }
            if (WaitForSingleObject(0, (unsigned long)msTimeout) != 0) return (long)0x80004004L;
        }
    }
};
int main()
{
    CDeferredCommand pres = { 50, 0 }, strm = { 10, 1 }; CDeferredCommand* got = 0;
    CCmdQueue q; q.m_listPresentation.m_head = &pres; q.m_listStream.m_head = &strm; q.m_bRunning = 1; q.m_StreamTimeOffset = 30; q.m_now = 45;
    int ok = 1;
    ok &= q.GetDueCommand(&got, 5) == 0 && got == &strm && g_addref == 1 && g_waits == 0;
    q.m_bRunning = 0; got = 0; q.m_now = 49; g_waitResult = 0x102; ok &= q.GetDueCommand(&got, 5) == (long)0x80004004L && got == 0 && g_waits == 1;
    q.m_now = 50; g_waitResult = 0; g_dueAfterWaits = 3; ok &= q.GetDueCommand(&got, 5) == 0 && got == &pres && g_waits == 3 && g_addref == 2;
    ok &= g_held == 0;
    std::printf(ok ? "00caf8f0_TEST PASS\n" : "00caf8f0_TEST FAIL\n");
    return ok ? 0 : 1;
}
