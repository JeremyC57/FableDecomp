// Behaviour test for 00cb09a0: DirectShow base-class WaitDispatchingMessages -- waits on the object while pumping messages
// and the notify event, with the Win32 calls replaced by counting stand-ins
#include <cstdio>
typedef unsigned long DWORD; typedef void* HANDLE; typedef void* HWND; typedef unsigned int UINT;
enum { WAIT_OBJECT_0 = 0, WAIT_TIMEOUT = 258, INFINITE = 0xFFFFFFFF, PM_REMOVE = 1, QS_ALLEVENTS = 0xBF, QS_ALLINPUT = 0xFF,
       THREAD_PRIORITY_HIGHEST = 2, THREAD_PRIORITY_NORMAL = 0 };
struct MSG { HWND hwnd; UINT message; };
static int g_waits = 0, g_msgWaits = 0, g_dispatched = 0, g_posted = 0, g_prio = 0, g_prioRestored = 0;
static DWORD g_tick = 1000; static int g_signalAfter = 1;
static DWORD GetTickCount() { return g_tick += 100; }
static DWORD WaitForMultipleObjects(DWORD, const HANDLE*, int, DWORD) { ++g_waits; return WAIT_OBJECT_0; }
static DWORD MsgWaitForMultipleObjects(DWORD count, const HANDLE*, int, DWORD, DWORD) { ++g_msgWaits; return g_signalAfter > 0 ? WAIT_OBJECT_0 + count : WAIT_OBJECT_0; }
static int PeekMessageW(MSG* m, HWND, UINT, UINT, UINT) { if (g_signalAfter == 1) { m->hwnd = 0; m->message = 0xC000; g_signalAfter = 0; return 1; } return 0; }
static long DispatchMessageW(const MSG*) { ++g_dispatched; return 0; }
static HANDLE GetCurrentThread() { return (HANDLE)1; }
static int GetThreadPriority(HANDLE) { return g_prio; }
static int SetThreadPriority(HANDLE, int p) { if (p == THREAD_PRIORITY_HIGHEST) g_prio = p; else ++g_prioRestored; return 1; }
static DWORD GetQueueStatus(UINT) { return 0; }
static UINT RegisterWindowMessageW(const wchar_t*) { return 0xC000; }
static DWORD GetCurrentThreadId() { return 7; }
static int PostThreadMessageW(DWORD, UINT, UINT, long) { ++g_posted; return 1; }
static DWORD WaitDispatchingMessages(HANDLE hObject, DWORD dwWait, HWND hwnd, UINT uMsg, HANDLE hNotifyEvent)
{
    int dwResult; int dwThreadPriority = THREAD_PRIORITY_NORMAL; static UINT uMsgId = 0;
    HANDLE hObjects[2] = { hObject, hNotifyEvent }; int nCount = hNotifyEvent ? 2 : 1;
    DWORD dwStart = 0, dwThreadPriorityStart = 0;
    if (dwWait != INFINITE && dwWait != 0) { dwStart = GetTickCount(); }
    for (;;) {
        DWORD dwTimeOut = dwWait;
        if (dwWait != INFINITE && dwWait != 0) {
            DWORD dwElapsed = GetTickCount() - dwStart;
            if (dwElapsed > dwWait) dwTimeOut = 0; else dwTimeOut = dwWait - dwElapsed;
        }
        if (hwnd) { dwResult = WaitForMultipleObjects(nCount, hObjects, 0, dwTimeOut); }
        else { dwResult = MsgWaitForMultipleObjects(nCount, hObjects, 0, dwTimeOut, hNotifyEvent ? QS_ALLINPUT : QS_ALLEVENTS); }
        if (dwResult == WAIT_OBJECT_0 + nCount || dwResult == WAIT_TIMEOUT) {
            MSG msg;
            if (hwnd) { while (PeekMessageW(&msg, hwnd, uMsg, uMsg, PM_REMOVE)) DispatchMessageW(&msg); }
            if (uMsgId == 0) uMsgId = RegisterWindowMessageW(L"AXWaitDispatchingMessages");
            if (PeekMessageW(&msg, 0, uMsgId, uMsgId, PM_REMOVE)) {
                if (msg.message == uMsgId) {
                    if (!hwnd || msg.hwnd == hwnd) DispatchMessageW(&msg);
                    else PostThreadMessageW(GetCurrentThreadId(), msg.message, 0, 0);
                }
            }
            if (dwThreadPriority == THREAD_PRIORITY_NORMAL) {
                dwThreadPriority = THREAD_PRIORITY_HIGHEST; dwThreadPriorityStart = GetTickCount();
                SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
            }
            if (dwResult == WAIT_TIMEOUT) break;
        } else break;
    }
    if (dwThreadPriority != THREAD_PRIORITY_NORMAL) SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
    return dwResult;
}
int main()
{
    HANDLE ev = (HANDLE)2;
    DWORD r = WaitDispatchingMessages((HANDLE)3, 5000, 0, 0, ev);
    bool ok = r == WAIT_OBJECT_0 && g_msgWaits == 2 && g_waits == 0 && g_dispatched == 1 && g_prio == THREAD_PRIORITY_HIGHEST && g_prioRestored == 1;
    r = WaitDispatchingMessages((HANDLE)3, INFINITE, (HWND)9, 0x400, 0);
    ok = ok && r == WAIT_OBJECT_0 && g_waits == 1 && g_msgWaits == 2 && g_prioRestored == 1;
    std::printf(ok ? "00cb09a0_TEST PASS\n" : "00cb09a0_TEST FAIL\n");
    return ok ? 0 : 1;
}