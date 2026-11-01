// WaitDispatchingMessages(HANDLE hObject, DWORD dwWait, HWND hwnd, UINT uMsg, HANDLE hNotifyEvent) -- DirectShow BaseClasses wxutil.cpp: wait for an object (and an optional notify event) while pumping the window's messages, trimming the wait as time passes; raises the thread priority while peeking and reposts a marker message afterwards
// Leaf: ?WaitDispatchingMessages@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB09A0, 520 bytes. `DWORD WINAPI` free function (`ret 14h`), strmbase wxutil.cpp (DX9 SDK era,
//   UNICODE build: PeekMessageW / DispatchMessageW / RegisterWindowMessageW / PostThreadMessageW).
// Identity: (1) the sole caller is CBaseRenderer::GetState 0x00CA6620 (landed, renbase.cpp: `WaitDispatchingMessages(m_evComplete, dwMSecs)`);
//   (2) the wide literal L"AXWaitDispatchingMessages" at 0x012BFA68 and the file-static `uMsgId` at 0x0143CCF8 are wxutil.cpp's.
// Imports (IAT): GetTickCount 0x0143FDFC, WaitForMultipleObjects 0x0143FEC8, MsgWaitForMultipleObjects 0x014403B4, PeekMessageW 0x01440370,
//   DispatchMessageW 0x01440368, GetCurrentThread 0x0143FDCC, GetThreadPriority 0x0143FEFC, SetThreadPriority 0x0143FDD0,
//   GetQueueStatus 0x014403B0, RegisterWindowMessageW 0x014403C0, GetCurrentThreadId 0x0143FE98, PostThreadMessageW 0x014403AC.
// Shape: the SDK text verbatim. VC7.1 peels the first WaitForMultipleObjects out of the `for (;;)` (its early exit returns eax straight,
//   skipping the bPeeked tail it knows is dead), hoists the QS mask (`lea eax,[eax*8+40h]`), and homes dwStart in the dead hObject slot.
typedef unsigned long DWORD;
typedef unsigned int UINT;
typedef int BOOL;
typedef long LONG;
typedef void* HANDLE;
typedef unsigned short WCHAR;
typedef unsigned int WPARAM;
typedef long LPARAM;
struct HWND__ { int unused; };
typedef HWND__* HWND;
struct POINT { LONG x; LONG y; };
struct MSG { HWND hwnd; UINT message; WPARAM wParam; LPARAM lParam; DWORD time; POINT pt; };
#define WINAPI __stdcall
#define INFINITE 0xFFFFFFFF
#define WAIT_OBJECT_0 0
#define WAIT_TIMEOUT 258L
#define QS_POSTMESSAGE 0x0008
#define QS_SENDMESSAGE 0x0040
#define PM_NOREMOVE 0x0000
#define PM_REMOVE 0x0001
#define THREAD_PRIORITY_HIGHEST 2
#define HIWORD(l) ((unsigned short)((DWORD)(l) >> 16))
#define TEXT(s) L##s
extern "C" __declspec(dllimport) DWORD WINAPI GetTickCount();                                                     // 0x0143FDFC
extern "C" __declspec(dllimport) DWORD WINAPI WaitForMultipleObjects(DWORD nCount, const HANDLE* lpHandles, BOOL bWaitAll, DWORD dwMilliseconds);   // 0x0143FEC8
extern "C" __declspec(dllimport) DWORD WINAPI MsgWaitForMultipleObjects(DWORD nCount, const HANDLE* pHandles, BOOL fWaitAll, DWORD dwMilliseconds, DWORD dwWakeMask);   // 0x014403B4
extern "C" __declspec(dllimport) BOOL WINAPI PeekMessageW(MSG* lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);   // 0x01440370
extern "C" __declspec(dllimport) LONG WINAPI DispatchMessageW(const MSG* lpMsg);                                  // 0x01440368
extern "C" __declspec(dllimport) HANDLE WINAPI GetCurrentThread();                                                // 0x0143FDCC
extern "C" __declspec(dllimport) int WINAPI GetThreadPriority(HANDLE hThread);                                    // 0x0143FEFC
extern "C" __declspec(dllimport) BOOL WINAPI SetThreadPriority(HANDLE hThread, int nPriority);                    // 0x0143FDD0
extern "C" __declspec(dllimport) DWORD WINAPI GetQueueStatus(UINT flags);                                         // 0x014403B0
extern "C" __declspec(dllimport) UINT WINAPI RegisterWindowMessageW(const WCHAR* lpString);                       // 0x014403C0
extern "C" __declspec(dllimport) DWORD WINAPI GetCurrentThreadId();                                               // 0x0143FE98
extern "C" __declspec(dllimport) BOOL WINAPI PostThreadMessageW(DWORD idThread, UINT Msg, WPARAM wParam, LPARAM lParam);   // 0x014403AC
#define PeekMessage PeekMessageW
#define DispatchMessage DispatchMessageW
#define RegisterWindowMessage RegisterWindowMessageW
#define PostThreadMessage PostThreadMessageW
#define NULL 0
#define FALSE 0
#define TRUE 1

// wxutil.h
DWORD WINAPI WaitDispatchingMessages(HANDLE hObject, DWORD dwWait, HWND hwnd = NULL, UINT uMsg = 0, HANDLE hNotifyEvent = NULL);

// Wait for a given event, dispatching messages while waiting
DWORD WINAPI WaitDispatchingMessages(
    HANDLE hObject,
    DWORD dwWait,
    HWND hwnd,
    UINT uMsg,
    HANDLE hNotifyEvent)
{
    BOOL bPeeked = 0;
    DWORD dwResult;
    DWORD dwStart;
    DWORD dwThreadPriority;

    static UINT uMsgId = 0;                                                                   // 0x0143CCF8

    HANDLE hObjects[2] = { hObject, hNotifyEvent };
    if (dwWait != INFINITE && dwWait != 0) {
        dwStart = GetTickCount();
    }
    for (; ; ) {
        DWORD nCount = NULL != hNotifyEvent ? 2 : 1;

        //  Minimize the chance of actually dispatching any messages
        //  by seeing if we can lock immediately.
        dwResult = WaitForMultipleObjects(nCount, hObjects, FALSE, 0);
        if (dwResult < WAIT_OBJECT_0 + nCount) {
            break;
        }

        DWORD dwTimeOut = dwWait;
        if (dwTimeOut > 10) {
            dwTimeOut = 10;
        }
        dwResult = MsgWaitForMultipleObjects(
                             nCount,
                             hObjects,
                             FALSE,
                             dwTimeOut,
                             hwnd == NULL ? QS_SENDMESSAGE :
                                            QS_SENDMESSAGE + QS_POSTMESSAGE);
        if (dwResult == WAIT_OBJECT_0 + nCount ||
            dwResult == WAIT_TIMEOUT && dwTimeOut != dwWait) {
            MSG msg;
            if (hwnd != NULL) {
                while (PeekMessage(&msg, hwnd, uMsg, uMsg, PM_REMOVE)) {
                    DispatchMessage(&msg);
                }
            }
            // Do this anyway - the previous peek doesn't flush out the
            // messages
            PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);

            if (dwWait != INFINITE && dwWait != 0) {
                DWORD dwNow = GetTickCount();

                // Working with differences handles wrap-around
                DWORD dwDiff = dwNow - dwStart;
                if (dwDiff > dwWait) {
                    dwWait = 0;
                } else {
                    dwWait -= dwDiff;
                }
                dwStart = dwNow;
            }
            if (!bPeeked) {
                //  Raise our priority to prevent our message queue
                //  building up
                dwThreadPriority = GetThreadPriority(GetCurrentThread());
                if (dwThreadPriority < THREAD_PRIORITY_HIGHEST) {
                    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
                }
                bPeeked = 1;
            }
        } else {
            break;
        }
    }
    if (bPeeked) {
        SetThreadPriority(GetCurrentThread(), dwThreadPriority);
        if (HIWORD(GetQueueStatus(QS_POSTMESSAGE)) & QS_POSTMESSAGE) {
            if (uMsgId == 0) {
                uMsgId = RegisterWindowMessage(TEXT("AXWaitDispatchingMessages"));
            }
            if (uMsgId != 0) {
                MSG msg;
                //  Remove old ones
                while (PeekMessage(&msg, (HWND)-1, uMsgId, uMsgId, PM_REMOVE)) {
                }
            }
            PostThreadMessage(GetCurrentThreadId(), uMsgId, 0, 0);
        }
    }
    return dwResult;
}