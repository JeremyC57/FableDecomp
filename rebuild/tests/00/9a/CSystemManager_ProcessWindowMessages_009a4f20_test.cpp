// CSystemManager::ProcessWindowMessages() -- under the manager's critical section, pump every pending message of the application window: offer each to the message filter first, translate and dispatch it when the filter is absent or declines
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009A4F20, length 141 bytes (+3 int3 padding).
// Callees (imports): EnterCriticalSection @ [0x0143FE0C], LeaveCriticalSection @ [0x0143FE10], PeekMessageW @ [0x01440370]
// (kept in edi), TranslateMessage @ [0x0144036C] (kept in ebp, loaded once inside the loop prologue), DispatchMessageW @
// [0x01440368]; the filter is a `bool (__fastcall*)(MSG*)` member pointer called through eax.
// Shape: `while (PeekMessage(...))` rotated by VC: one test before the loop, one at its foot; `&CriticalSection` kept in ebx;
// the window handle is read through the inline accessor GetApplicationWinHandle(), which puts &msg/hwnd in eax/ecx as retail.
// Model: PDB CSystemManager (+0x34 CriticalSection, ApplicationWinHandle, MessageFilter); retail sits 8 bytes above the PDB
// from the counted-pointer block on (window handle +0x94, filter +0x17C).
struct HWND__;
struct POINT { long x, y; };
struct MSG {
    HWND__* hwnd;
    unsigned int message;
    unsigned int wParam;
    long lParam;
    unsigned long time;
    POINT pt;
};
struct RTL_CRITICAL_SECTION { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(RTL_CRITICAL_SECTION*);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(RTL_CRITICAL_SECTION*);
extern "C" __declspec(dllimport) int __stdcall PeekMessageW(MSG*, HWND__*, unsigned int, unsigned int, unsigned int);
extern "C" __declspec(dllimport) int __stdcall TranslateMessage(const MSG*);
extern "C" __declspec(dllimport) long __stdcall DispatchMessageW(const MSG*);

class CSystemManager { public:
    unsigned char _pad00[0x34];
    RTL_CRITICAL_SECTION CriticalSection;       // +0x34
    unsigned char _pad4c[0x94 - 0x4c];
    HWND__* ApplicationWinHandle;               // +0x94
    unsigned char _pad98[0x17c - 0x98];
    bool (__fastcall* MessageFilter)(MSG* msg); // +0x17c
    void ProcessWindowMessages();
    HWND__* GetApplicationWinHandle() const { return ApplicationWinHandle; }
};

void CSystemManager::ProcessWindowMessages()
{
    MSG msg;
    EnterCriticalSection(&CriticalSection);
    while (PeekMessageW(&msg, GetApplicationWinHandle(), 0, 0, 1)) {
        if (!MessageFilter || !MessageFilter(&msg)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }
    LeaveCriticalSection(&CriticalSection);
}

// ---- test harness ----
#include <stdio.h>
#pragma comment(lib, "user32.lib")
extern "C" __declspec(dllimport) int __stdcall PostThreadMessageW(unsigned long, unsigned int, unsigned int, long);
extern "C" __declspec(dllimport) unsigned long __stdcall GetCurrentThreadId();
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(RTL_CRITICAL_SECTION*);
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(RTL_CRITICAL_SECTION*);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_seen = 0, g_sum = 0;
static bool __fastcall Filter(MSG* msg) { ++g_seen; g_sum += (int)msg->wParam; return (msg->wParam & 1) != 0; }

int main()
{
    static CSystemManager mgr;
    InitializeCriticalSection(&mgr.CriticalSection);
    mgr.ApplicationWinHandle = 0;
    MSG probe;
    PeekMessageW(&probe, 0, 0, 0, 0);          // make sure the thread has a queue
    unsigned long tid = GetCurrentThreadId();
    for (int i = 1; i <= 5; ++i) PostThreadMessageW(tid, 0x400 + i, i, 0);
    mgr.MessageFilter = Filter;
    mgr.ProcessWindowMessages();
    CHECK(g_seen == 5 && g_sum == 15);
    CHECK(!PeekMessageW(&probe, 0, 0, 0, 0));
    for (int i = 0; i < 3; ++i) PostThreadMessageW(tid, 0x500 + i, i, 0);
    mgr.MessageFilter = 0;
    mgr.ProcessWindowMessages();
    CHECK(g_seen == 5);
    CHECK(!PeekMessageW(&probe, 0, 0, 0, 0));
    mgr.ProcessWindowMessages();                // empty queue: no call
    CHECK(mgr.CriticalSection.RecursionCount == 0);
    DeleteCriticalSection(&mgr.CriticalSection);
    if (fails == 0) printf("009a4f20_TEST PASS\n");
    return fails;
}