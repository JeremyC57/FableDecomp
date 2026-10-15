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
static long g_waited = -1; static float g_progress = 0.0f;
struct CVideoSys {
    void* GBuilder; IMediaControl* MControl; unsigned char _pad08[0x20]; long State;
    void WaitForState(long state) { g_waited = state; }
    float GetProgress() { return g_progress; }
    unsigned char Stop() { if (State == 0) return 0; MControl->Stop(); WaitForState(0); State = 3; return 1; }
    unsigned char IsFinished() { if (State != 1 && State != 2) return 1; if (GetProgress() >= 1.0f) Stop(); return 0; }
};
int main()
{
    IMediaControl mc; CVideoSys v; v.MControl = &mc;
    v.State = 0; if (v.IsFinished() != 1) { std::printf("00a3b330_TEST FAIL\n"); return 1; }
    v.State = 3; if (v.IsFinished() != 1) { std::printf("00a3b330_TEST FAIL\n"); return 1; }
    v.State = 1; g_progress = 0.5f; if (v.IsFinished() != 0 || g_stop != 0 || v.State != 1) { std::printf("00a3b330_TEST FAIL\n"); return 1; }
    v.State = 2; g_progress = 1.0f; if (v.IsFinished() != 0 || g_stop != 1 || g_waited != 0 || v.State != 3) { std::printf("00a3b330_TEST FAIL\n"); return 1; }
    std::printf("00a3b330_TEST PASS\n"); return 0;
}