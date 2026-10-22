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
extern "C" __declspec(dllimport) void* __stdcall GetProcAddress(void* module, const char* name);   // IAT 0x0143FE44
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