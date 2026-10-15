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
struct IMediaPosition : IDispatchRetail {
    virtual long __stdcall get_Duration(double* v) { *v = 12.5; return 0; }
    virtual long __stdcall put_CurrentPosition(double) { return 0; }
    virtual long __stdcall get_CurrentPosition(double* v) { *v = 3.25; return 0; }
};
struct CVideoSys {
    void* GBuilder; IMediaControl* MControl; IMediaPosition* MPosition;
    float GetCurTime() { if (MPosition == 0) return 0.0f; double t = 0.0; MPosition->get_CurrentPosition(&t); return (float)t; }
};
int main()
{
    IMediaPosition mp; CVideoSys v; v.MPosition = 0;
    if (v.GetCurTime() != 0.0f) { std::printf("00a3b260_TEST FAIL\n"); return 1; }
    v.MPosition = &mp; double expect = 0; mp.get_CurrentPosition(&expect);
    if (v.GetCurTime() != (float)expect) { std::printf("00a3b260_TEST FAIL\n"); return 1; }
    std::printf("00a3b260_TEST PASS\n"); return 0;
}
