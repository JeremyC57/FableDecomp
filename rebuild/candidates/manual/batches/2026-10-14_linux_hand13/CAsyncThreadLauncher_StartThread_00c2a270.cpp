// CAsyncThreadLauncher::StartThread() -- creates the worker thread (default security, the launcher's configured
// stack size, the static thread procedure, `this` as its parameter, running immediately, no thread id) and yields
// for 20 ms so the thread gets going before the caller continues.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2A270, length 38 bytes.
// Callees: CreateThread (IAT 0x0143FE68), Sleep (IAT 0x0143FE1C); the thread procedure is 0x00C2A210.
// Layout from the reads: StackSize +8, ThreadHandle +0xc (the handle store is scheduled between Sleep's push and call).
extern "C" __declspec(dllimport) void* __stdcall CreateThread(void* security, unsigned long stackSize,
    unsigned long (__stdcall* startAddress)(void*), void* parameter, unsigned long creationFlags,
    unsigned long* threadId);                                                   // IAT 0x0143FE68
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long milliseconds);   // IAT 0x0143FE1C

unsigned long __stdcall AsyncThreadProc(void* launcher);                        // 0x00C2A210

struct CAsyncThreadLauncher {
    unsigned char _pad[8];
    unsigned long StackSize;                             // +8
    void* ThreadHandle;                                  // +0xc
    void StartThread();
};

void CAsyncThreadLauncher::StartThread()
{
    ThreadHandle = CreateThread(0, StackSize, AsyncThreadProc, this, 0, 0);
    Sleep(20);
}
