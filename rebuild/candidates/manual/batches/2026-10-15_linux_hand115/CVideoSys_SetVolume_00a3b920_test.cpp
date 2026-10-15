#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __forceinline inline
#define __declspec(x)
#endif
#include <math.h>
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
static long g_volume = 12345; static int g_put = 0;
struct IBasicAudio : IDispatchRetail { virtual long __stdcall put_Volume(long v) { g_volume = v; ++g_put; return 0; } };
struct CVideoSys {
    void* GBuilder; void* MControl; void* MPosition; void* MSeeking; void* MEvent; IBasicAudio* BasicAudio;
    void SetVolume(float volume) { if (BasicAudio) { if (volume < 0.001f) volume = 0.001f; BasicAudio->put_Volume((long)(log(volume) * 500.0f)); } }
};
int main()
{
    IBasicAudio ba; CVideoSys v; v.BasicAudio = 0; v.SetVolume(1.0f);
    if (g_put != 0) { std::printf("00a3b920_TEST FAIL\n"); return 1; }
    v.BasicAudio = &ba; v.SetVolume(1.0f);
    if (g_put != 1 || g_volume != 0) { std::printf("00a3b920_TEST FAIL\n"); return 1; }
    v.SetVolume(0.0f);                                  // clamped to 0.001: 500 * ln(0.001) = -3453.8 -> -3453
    if (g_put != 2 || g_volume != -3453) { std::printf("00a3b920_TEST FAIL\n"); return 1; }
    v.SetVolume(0.5f);                                  // 500 * ln(0.5) = -346.57 -> -346
    if (g_put != 3 || g_volume != -346) { std::printf("00a3b920_TEST FAIL\n"); return 1; }
    std::printf("00a3b920_TEST PASS\n"); return 0;
}
