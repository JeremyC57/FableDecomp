#include <cstdio>
static void* g_param = 0; static unsigned long g_stack = 0; static unsigned long g_slept = 0; static unsigned long g_flags = 1;
static unsigned long (__stdcall* g_proc)(void*) = 0;
void* __stdcall FakeCreateThread(void* sec, unsigned long stackSize, unsigned long (__stdcall* start)(void*), void* param, unsigned long flags, unsigned long* id) { g_stack = stackSize; g_proc = start; g_param = param; g_flags = flags; return sec == 0 && id == 0 ? (void*)0x1234 : 0; }
void __stdcall FakeSleep(unsigned long ms) { g_slept = ms; }
unsigned long __stdcall AsyncThreadProc(void*) { return 7; }
struct CAsyncThreadLauncher {
    unsigned char _pad[8]; unsigned long StackSize; void* ThreadHandle;
    void StartThread() { ThreadHandle = FakeCreateThread(0, StackSize, AsyncThreadProc, this, 0, 0); FakeSleep(20); }
};
int main()
{
    CAsyncThreadLauncher l; l.StackSize = 65536; l.ThreadHandle = 0;
    l.StartThread();
    if (l.ThreadHandle != (void*)0x1234 || g_stack != 65536 || g_proc != &AsyncThreadProc || g_param != &l || g_flags != 0 || g_slept != 20) { std::printf("00c2a270_TEST FAIL\n"); return 1; }
    std::printf("00c2a270_TEST PASS\n");
    return 0;
}