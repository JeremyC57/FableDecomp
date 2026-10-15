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
struct CVideoSys { void* GBuilder; IMediaControl* MControl; void* MPosition; void WaitForState(long state); };
void CVideoSys::WaitForState(long state) { long current; long i = 0; do { MControl->GetState(10, &current); ++i; } while (state != current && i < 100); }
int main()
{
    IMediaControl mc; CVideoSys v; v.MControl = &mc;
    for (int k = 0; k < 200; ++k) g_states[k] = 0;
    g_states[3] = 1; v.WaitForState(1);
    if (g_calls != 4) { std::printf("00a3b0f0_TEST FAIL\n"); return 1; }
    g_calls = 0; g_stateIdx = 0; g_states[3] = 0; v.WaitForState(1);       // never reached: 100 polls
    if (g_calls != 100) { std::printf("00a3b0f0_TEST FAIL\n"); return 1; }
    std::printf("00a3b0f0_TEST PASS\n"); return 0;
}