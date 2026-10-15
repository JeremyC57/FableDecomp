#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __forceinline inline
#define __declspec(x)
#endif
struct IDispatchRetail {
    virtual long __stdcall QueryInterface(void*, void**) { return 0; }
    virtual unsigned long __stdcall AddRef() { return 1; }
    virtual unsigned long __stdcall Release() { return 0; }
    virtual long __stdcall GetTypeInfoCount(unsigned int*) { return 0; }
    virtual long __stdcall GetTypeInfo(unsigned int, unsigned long, void**) { return 0; }
    virtual long __stdcall GetIDsOfNames(void*, void*, unsigned int, unsigned long, long*) { return 0; }
    virtual long __stdcall Invoke(long, void*, unsigned long, unsigned short, void*, void*, void*, unsigned int*) { return 0; }
};
static int g_calls = 0; static long g_states[200]; static int g_stateIdx = 0; static int g_run = 0, g_pause = 0, g_stop = 0;
struct IMediaControl : IDispatchRetail {
    virtual long __stdcall Run() { ++g_run; return 0; }
    virtual long __stdcall Pause() { ++g_pause; return 0; }
    virtual long __stdcall Stop() { ++g_stop; return 0; }
    virtual long __stdcall GetState(long timeout, long* state) { if (timeout != 10) return -1; *state = g_states[g_stateIdx++]; ++g_calls; return 0; }
};
static long g_waited = -1;
struct CVideoSys {
    void* GBuilder; IMediaControl* MControl; unsigned char _pad08[0x20]; long State;
    void WaitForState(long state) { g_waited = state; }
    unsigned char Pause() { if (State == 0) return 0; MControl->Pause(); WaitForState(1); State = 2; return 1; }
};
int main()
{
    IMediaControl mc; CVideoSys v; v.MControl = &mc; v.State = 0;
    if (v.Pause() != 0 || g_pause != 0 || g_waited != -1) { std::printf("00a3b1c0_TEST FAIL\n"); return 1; }
    v.State = 1;
    if (v.Pause() != 1 || g_pause != 1 || g_waited != 1 || v.State != 2) { std::printf("00a3b1c0_TEST FAIL\n"); return 1; }
    std::printf("00a3b1c0_TEST PASS\n"); return 0;
}