// Behaviour test for 0x00CA47D0 CBaseRenderer::WaitForReceiveToComplete (DirectShow renbase.cpp): spin (peek + Sleep(1)) while Receive is active, then post a WM_NULL to wake the thread if a posted message is queued.
// Test model: plain C++98 copies of the members the function touches; Win32 / COM callees are recording stand-ins.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#ifdef _MSC_VER
typedef __int64 LONGLONG;
#else
typedef long long LONGLONG;
#endif
typedef LONGLONG REFERENCE_TIME;
typedef int HRESULT;
struct GUID { unsigned int Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; };
inline int operator==(const GUID& a, const GUID& b) { return std::memcmp(&a, &b, sizeof(GUID)) == 0; }
#define NOERROR 0
#define S_OK 0
#define S_FALSE 1
#define E_POINTER ((HRESULT)0x80004003u)
#define E_OUTOFMEMORY ((HRESULT)0x8007000Eu)
#define E_FAIL ((HRESULT)0x80004005u)
#define FAILED(hr) (((HRESULT)(hr)) < 0)
#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
#define CheckPointer(p, ret) { if ((p) == 0) return (ret); }
#define State_Stopped 0
#define State_Paused 1
#define State_Running 2
struct CCritSec {
    int depth; int entered;
    CCritSec() : depth(0), entered(0) {}
    void Lock() { depth++; entered++; }
    void Unlock() { depth--; }
};
struct CAutoLock {
    CCritSec* m_pLock;
    CAutoLock(CCritSec* plock) : m_pLock(plock) { m_pLock->Lock(); }
    ~CAutoLock() { m_pLock->Unlock(); }
};
struct CAMEvent {                                // the test's event: a flag instead of a kernel HANDLE
    int signalled;
    CAMEvent() : signalled(0) {}
    void Set() { signalled = 1; }
    void Reset() { signalled = 0; }
};
struct MSG { void* hwnd; unsigned int message; };
static int g_peeks, g_sleeps, g_posts, g_sleep_arg = -1;
static unsigned int g_queue_status;
static int* g_in_receive; static int g_clear_after;
static int PeekMessageW(MSG* m, void*, unsigned int, unsigned int, unsigned int) { g_peeks++; m->message = 0; return 0; }
static void Sleep(unsigned int ms) { g_sleeps++; g_sleep_arg = (int)ms; if (g_sleeps >= g_clear_after) *g_in_receive = 0; }
static unsigned int GetQueueStatus(unsigned int) { return g_queue_status; }
static unsigned int GetCurrentThreadId() { return 1234; }
static unsigned int g_post_thread;
static int PostThreadMessageW(unsigned int id, unsigned int msg, unsigned int, long) { g_posts++; g_post_thread = id + msg; return 1; }
#define WM_NULL 0x0000
#define PM_NOREMOVE 0x0000
#define QS_POSTMESSAGE 0x0008
#define HIWORD(l) ((unsigned short)((((unsigned int)(l)) >> 16) & 0xffff))
struct CBaseRenderer {
    int m_bInReceive;
    void WaitForReceiveToComplete();
};
void CBaseRenderer::WaitForReceiveToComplete()
{
    for (;;) {
        if (!m_bInReceive) {
            break;
        }
        MSG msg;
        PeekMessageW(&msg, 0, WM_NULL, WM_NULL, PM_NOREMOVE);
        Sleep(1);
    }
    if (HIWORD(GetQueueStatus(QS_POSTMESSAGE)) & QS_POSTMESSAGE) {
        PostThreadMessageW(GetCurrentThreadId(), WM_NULL, 0, 0);
    }
}
int main()
{
    CBaseRenderer r;
    int ok = 1;
    r.m_bInReceive = 0; g_in_receive = &r.m_bInReceive; g_clear_after = 1; g_queue_status = 0x00000008;
    r.WaitForReceiveToComplete();
    ok &= g_peeks == 0 && g_sleeps == 0 && g_posts == 0;      // only the low word has QS_POSTMESSAGE: nothing posted
    r.m_bInReceive = 1; g_clear_after = 3; g_queue_status = 0x00080000;
    r.WaitForReceiveToComplete();
    ok &= g_peeks == 3 && g_sleeps == 3 && g_sleep_arg == 1 && g_posts == 1 && g_post_thread == 1234 && r.m_bInReceive == 0;
    if (!ok) { std::printf("00ca47d0_TEST FAIL\n"); return 1; }
    std::printf("00ca47d0_TEST PASS\n");
    return 0;
}