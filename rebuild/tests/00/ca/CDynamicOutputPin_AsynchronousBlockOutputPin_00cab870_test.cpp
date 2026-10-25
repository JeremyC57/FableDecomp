// Behaviour test for 00cab870: CDynamicOutputPin::AsynchronousBlockOutputPin -- refusals, duplicate failure, PENDING + immediate block
#include <cstdio>
static int g_held = 0, g_locks = 0, g_dupOk = 1, g_blocked = 0, g_streaming = 0;
static unsigned long g_tid = 7;
static unsigned long GetCurrentThreadId() { return g_tid; }
static void* GetCurrentProcess() { return (void*)-1; }
static int DuplicateHandle(void*, void* src, void*, void** dst, unsigned long access, int, unsigned long)
{ if (!g_dupOk) return 0; *dst = (void*)((char*)src + 100); return access == 2; }
static long AmGetLastErrorToHResult() { return (long)0x80070005L; }
struct CCritSec { void Lock() { ++g_held; ++g_locks; } void Unlock() { --g_held; } };
struct CAutoLock { CCritSec* m_pLock; CAutoLock(CCritSec* l) : m_pLock(l) { m_pLock->Lock(); } ~CAutoLock() { m_pLock->Unlock(); } };
class CDynamicOutputPin {
public:
    enum BLOCK_STATE { NOT_BLOCKED, PENDING, BLOCKED };
    CCritSec m_BlockStateLock; void* m_hNotifyCallerPinBlockedEvent; BLOCK_STATE m_BlockState; unsigned long m_dwBlockCallerThreadID;
    virtual bool StreamingThreadUsingOutputPin() { return g_streaming != 0; }
    void BlockOutputPin() { ++g_blocked; m_BlockState = BLOCKED; }
    long AsynchronousBlockOutputPin(void* hEvent)
    {
        CAutoLock alBlockStateLock(&m_BlockStateLock);
        if (NOT_BLOCKED != m_BlockState) {
            if (m_dwBlockCallerThreadID == GetCurrentThreadId()) return (long)0x80040293L;
            else return (long)0x80040294L;
        }
        int fSuccess = DuplicateHandle(GetCurrentProcess(), hEvent, GetCurrentProcess(), &m_hNotifyCallerPinBlockedEvent, 2, 0, 0);
        if (!fSuccess) return AmGetLastErrorToHResult();
        m_BlockState = PENDING;
        m_dwBlockCallerThreadID = GetCurrentThreadId();
        if (!StreamingThreadUsingOutputPin()) BlockOutputPin();
        return 0;
    }
};
int main()
{
    CDynamicOutputPin p; p.m_BlockState = CDynamicOutputPin::NOT_BLOCKED; p.m_hNotifyCallerPinBlockedEvent = 0; p.m_dwBlockCallerThreadID = 0;
    int ok = 1;
    g_streaming = 1; ok &= p.AsynchronousBlockOutputPin((void*)8) == 0 && p.m_BlockState == 1 && p.m_dwBlockCallerThreadID == 7 && p.m_hNotifyCallerPinBlockedEvent == (void*)108 && g_blocked == 0;
    ok &= p.AsynchronousBlockOutputPin((void*)8) == (long)0x80040293L;
    g_tid = 9; ok &= p.AsynchronousBlockOutputPin((void*)8) == (long)0x80040294L;
    p.m_BlockState = CDynamicOutputPin::NOT_BLOCKED; g_streaming = 0; ok &= p.AsynchronousBlockOutputPin((void*)8) == 0 && g_blocked == 1 && p.m_BlockState == 2;
    p.m_BlockState = CDynamicOutputPin::NOT_BLOCKED; g_dupOk = 0; ok &= p.AsynchronousBlockOutputPin((void*)8) == (long)0x80070005L && p.m_BlockState == 0;
    ok &= g_held == 0 && g_locks == 5;
    std::printf(ok ? "00cab870_TEST PASS\n" : "00cab870_TEST FAIL\n");
    return ok ? 0 : 1;
}
