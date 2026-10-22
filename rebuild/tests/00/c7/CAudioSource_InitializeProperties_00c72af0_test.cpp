// Behaviour test for 00c72af0: final.cpp copied verbatim (the dllimport declaration made plain), the declared callees
// stubbed below so calls and arguments are observable. CAudioSource::InitializeProperties
#include <cstdio>
#include <cstring>
#include <cstdlib>
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#define __fastcall
#define __stdcall
#define __cdecl
#define __int64 long long
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static char g_log[1024];
static void note(const char* s) { if (strlen(g_log) + strlen(s) + 2 < sizeof(g_log)) { strcat(g_log, s); strcat(g_log, ";"); } }
#define GetProcAddress Test_GetProcAddress   // keep the test's stub clear of kernel32's export
// ---- source (verbatim) ----
// CAudioSource::InitializeProperties(CAudioSound* sound, const CAudioSourceProperties* props, __int64 now) -- derive the playback flags of a source from its sound and property block: pick the mix path, schedule the gain fade-in and pitch fade end times from `now`, hand the pitch block to the pitch setup, clear the OpenAL source's buffer state and return the flags
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C72AF0, length 336 bytes, ret 10h (the 64-bit time is passed by value).
// Callees: __ftol2 @ 0x00BFEA70 (twice), CAudioSource::SetPitchProperties @ 0x00C71F10 (catalog "Audio_InitSource_Properties"),
//          CAudioContext::GetProcAddress @ 0x00C4D740 / GetProcAddress (IAT 0x0143FE44) through the inline alSourcei resolver of the
//          global audio context @ 0x0143BDB0 (entry slot +0x50 = 0x0143BE00, flag +0x104 = 0x0143BEB4, "alSourcei" @ 0x012BEE14),
//          the sound's virtual slot 1 (bool). Global: the audio clock rate (__int64) @ 0x0139CA20; 0.0f @ 0x0122DEDC.
// Layout (descriptive; no PDB class for the audio layer): CAudioSource Flags +0x28, FadeGain +0x30, SourceId +0x58,
//          FadeEndTime +0x68, PitchEndTime +0x78; the sound's flag word at +0xc; the property block: Flags +0, FadeTime +0x18
//          (float*), PitchTime +0x20 (float*).
// Shape: the result word lives in the dead property-block argument slot; the fade time and pitch time pointers are named
//          locals (the pitch pointer survives the SetPitchProperties call in edi); the masked Flags go through a local that
//          is stored before the fade branch and again inside it (`flags |= 5` written before the mask); the alSourcei
//          resolver is the landed CAudioContext::Listener3f shape inlined on the global context object.
// Leaf: InitializeProperties
extern "C" void* __stdcall GetProcAddress(void* module, const char* name);   // IAT 0x0143FE44 (plain declaration for the test)
typedef void (__cdecl* PFN_alSourcei)(unsigned long source, long param, long value);

struct CAudioContext {
    void*          __vftable;
    void*          Module;                               // +0x04
    unsigned char  _pad08[0x48];
    PFN_alSourcei  pfnSourcei;                           // +0x50 (0x0143BE00)
    unsigned char  _pad54[0xb0];
    bool           UseALGetProcAddress;                  // +0x104 (0x0143BEB4)
    void* GetProcAddress(const char* name);              // 0x00C4D740
    void Sourcei(unsigned long source, long param, long value)
    {
        if (!pfnSourcei)
            pfnSourcei = (PFN_alSourcei)(UseALGetProcAddress ? GetProcAddress("alSourcei")
                                                             : ::GetProcAddress(Module, "alSourcei"));
        pfnSourcei(source, param, value);
    }
};
extern CAudioContext g_AudioContext;                     // 0x0143BDB0
extern __int64 g_AudioClockRate;                         // 0x0139CA20

struct CAudioSound {
    virtual void v00();
    virtual bool IsStreamed();                           // slot 1
    unsigned char _pad04[8];
    unsigned long Flags;                                 // +0x0c
};
struct CAudioSourceProperties {
    unsigned long  Flags;                                // +0x00
    unsigned char  _pad04[0x14];
    const float*   FadeTime;                             // +0x18
    unsigned char  _pad1c[4];
    const float*   PitchTime;                            // +0x20
};
struct CAudioSource {
    unsigned char  _pad00[0x28];
    unsigned long  Flags;                                // +0x28
    unsigned char  _pad2c[4];
    float          FadeGain;                             // +0x30
    unsigned char  _pad34[0x24];
    unsigned long  SourceId;                             // +0x58
    unsigned char  _pad5c[0xc];
    __int64        FadeEndTime;                          // +0x68
    unsigned char  _pad70[8];
    __int64        PitchEndTime;                         // +0x78
    void SetPitchProperties(const float* pitchTime, __int64 now);   // 0x00C71F10
    unsigned long InitializeProperties(CAudioSound* sound, const CAudioSourceProperties* props, __int64 now);
};

unsigned long CAudioSource::InitializeProperties(CAudioSound* sound, const CAudioSourceProperties* props, __int64 now)
{
    unsigned long soundFlags = sound->Flags;
    unsigned long flags = props->Flags;
    if (!(flags & 0xe02)) {
        if (soundFlags & 0x40000)
            flags |= 0x800;
        else if (soundFlags & 0x20000)
            flags |= 0x400;
        else
            flags |= 2;
    }
    const float* fade = props->FadeTime;
    flags |= 5;
    unsigned long f = Flags & ~0x5e;
    Flags = f;
    if (fade) {
        FadeEndTime = (__int64)(g_AudioClockRate * *fade) + now;
        FadeGain = 0.0f;
        Flags = f;
    } else
        FadeGain = 1.0f;
    if (props->FadeTime)
        flags |= 8;
    const float* pitch = props->PitchTime;
    SetPitchProperties(pitch, now);
    if (pitch && *pitch > 0.0f)
        PitchEndTime = (__int64)(g_AudioClockRate * *pitch) + now;
    if (props->PitchTime)
        flags |= 0x20;
    if (!(sound->Flags & 0x2000)) {
        if (sound->IsStreamed())
            flags |= 0x1000000;
        else
            flags |= 0x4000;
    }
    g_AudioContext.Sourcei(SourceId, 0x1021, 0);
    return flags;
}

// ---- stubs ----
CAudioContext g_AudioContext;
__int64 g_AudioClockRate = 1000;
static void __cdecl StubSourcei(unsigned long source, long param, long value) { char b[64]; _snprintf(b, sizeof b, "sourcei(%lu,%lx,%ld)", source, param, value); note(b); }
static void* g_module = (void*)0x1234;
extern "C" void* __stdcall GetProcAddress(void* module, const char* name) { CHECK(module == g_module); char b[64]; _snprintf(b, sizeof b, "GetProcAddress(%s)", name); note(b); return (void*)&StubSourcei; }
void* CAudioContext::GetProcAddress(const char* name) { CHECK(this == &g_AudioContext); char b[64]; _snprintf(b, sizeof b, "ctx.GetProcAddress(%s)", name); note(b); return (void*)&StubSourcei; }
static bool g_streamed = false;
void CAudioSound::v00() {}
bool CAudioSound::IsStreamed() { return false; }
struct TestSound : CAudioSound { void v00() {} bool IsStreamed() { note("streamed?"); return g_streamed; } };
static const float* g_lastPitch = 0; static __int64 g_lastNow = 0; static CAudioSource* g_lastThis = 0;
void CAudioSource::SetPitchProperties(const float* pitchTime, __int64 now) { note("setpitch"); g_lastPitch = pitchTime; g_lastNow = now; g_lastThis = this; }

static CAudioSource make() { CAudioSource s; memset(&s, 0, sizeof s); s.Flags = 0xffffffff; s.SourceId = 42; s.FadeGain = -1.0f; s.FadeEndTime = -1; s.PitchEndTime = -1; return s; }
static void reset() { g_log[0] = 0; memset(&g_AudioContext, 0, sizeof g_AudioContext); g_AudioContext.Module = g_module; g_streamed = false; }

int main()
{
    float fade = 2.5f, pitch = 0.5f, zeroPitch = 0.0f;
    TestSound sound; sound.Flags = 0;
    CAudioSourceProperties props; memset(&props, 0, sizeof props);
    // 1. no mix bits, no sound hints: path bit 2; no fade (gain 1), no pitch; resolver through alGetProcAddress; not streamed -> 0x4000
    { CAudioSource s = make(); reset(); g_AudioContext.UseALGetProcAddress = true; props.Flags = 0x100; sound.Flags = 0;
      unsigned long r = s.InitializeProperties(&sound, &props, 1000000);
      CHECK(r == (0x100 | 2 | 5 | 0x4000)); CHECK(s.Flags == 0xffffffa1); CHECK(s.FadeGain == 1.0f); CHECK(s.FadeEndTime == -1 && s.PitchEndTime == -1);
      CHECK(strcmp(g_log, "setpitch;streamed?;ctx.GetProcAddress(alSourcei);sourcei(42,1021,0);") == 0); CHECK(g_lastPitch == 0 && g_lastNow == 1000000 && g_lastThis == &s);
      CHECK(g_AudioContext.pfnSourcei == &StubSourcei); }
    // 2. sound hint 0x40000 -> 0x800; fade and pitch present; resolver through the module; streamed -> 0x1000000
    { CAudioSource s = make(); reset(); props.Flags = 0; sound.Flags = 0x40000; props.FadeTime = &fade; props.PitchTime = &pitch; g_streamed = true;
      unsigned long r = s.InitializeProperties(&sound, &props, 1000);
      CHECK(r == (0x800 | 5 | 8 | 0x20 | 0x1000000)); CHECK(s.Flags == 0xffffffa1); CHECK(s.FadeGain == 0.0f);
      CHECK(s.FadeEndTime == 2500 + 1000); CHECK(s.PitchEndTime == 500 + 1000); CHECK(g_lastPitch == &pitch);
      CHECK(strcmp(g_log, "setpitch;streamed?;GetProcAddress(alSourcei);sourcei(42,1021,0);") == 0); }
    // 3. sound hint 0x20000 -> 0x400; pitch present but zero: no pitch end time; sound flag 0x2000 skips the stream test; cached resolver
    { CAudioSource s = make(); reset(); g_AudioContext.pfnSourcei = &StubSourcei; props.Flags = 0; sound.Flags = 0x20000 | 0x2000; props.FadeTime = 0; props.PitchTime = &zeroPitch;
      unsigned long r = s.InitializeProperties(&sound, &props, 7);
      CHECK(r == (0x400 | 5 | 0x20)); CHECK(s.PitchEndTime == -1 && s.FadeEndTime == -1); CHECK(s.FadeGain == 1.0f);
      CHECK(strcmp(g_log, "setpitch;sourcei(42,1021,0);") == 0); }
    // 4. mix bits already chosen (0x200): no path bit added; nothing else
    { CAudioSource s = make(); reset(); g_AudioContext.pfnSourcei = &StubSourcei; props.Flags = 0x200; sound.Flags = 0x40000 | 0x2000; props.PitchTime = 0;
      unsigned long r = s.InitializeProperties(&sound, &props, 0);
      CHECK(r == (0x200 | 5)); }
    if (fails == 0) printf("00c72af0_TEST PASS\n"); else printf("00c72af0_TEST FAIL (%d)\n", fails);
    return fails ? 1 : 0;
}