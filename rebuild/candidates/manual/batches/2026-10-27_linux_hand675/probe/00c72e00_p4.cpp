// CAudioSource::ApplySound(CAudioObject* sound, __int64 now) -- re-apply a sound's property block to its OpenAL source
// Leaf: ?ApplySound@CAudioSource@@
extern "C" __declspec(dllimport) void* __stdcall GetProcAddress(void* module, const char* name);   // IAT 0x0143FE44
typedef void (__cdecl* PFN_alSourcei)(unsigned long source, long param, long value);

struct CAudioContext {
    void*          __vftable;
    void*          Module;                               // +0x04
    unsigned char  _pad08[0x48];
    PFN_alSourcei  pfnSourcei;                           // +0x50 (0x0143BE00)
    unsigned char  _pad54[0xb0];
    bool           UseALGetProcAddress;                  // +0x104 (0x0143BEB4)
    unsigned long  ForcedSourceFlags;                    // +0x108 (0x0143BEB8)
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
struct CAudioMaster {
    unsigned char  _pad00[0x54];
    long           PendingUpdate;                        // +0x54
};
extern CAudioMaster* g_pAudioMaster;                     // 0x0143BC2C

struct CFadeParams {
    float          Time;                                 // +0x00
    unsigned char  _pad04[0x10];
    float          Target;                               // +0x14
};
struct CAudioSourceProperties {
    unsigned long  Flags;                                // +0x00
    unsigned char  _pad04[0x14];
    const CFadeParams* Fade;                             // +0x18
    unsigned char  _pad1c[4];
    const float*   PitchTime;                            // +0x20
};
class CAudioObject {
public:
    virtual void v00();
    virtual bool IsStreamed();                           // slot 1
    CAudioObject* AsStream() { return IsStreamed() ? this : 0; }
    CAudioObject* AsSample() { return IsStreamed() ? 0 : this; }
    unsigned char  _pad04[0x0c];
    unsigned long  StateFlags;                           // +0x10
    CAudioSourceProperties Properties;                   // +0x14
    unsigned char  _pad38[0x40];
    void*          LoopData;                             // +0x78
};
struct CAudioSource {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual bool IsHardware();                           // slot 5 (+0x14)
    unsigned char  _pad08[0x18];
    __int64        FadeOutTime;                          // +0x20
    unsigned long  Flags;                                // +0x28
    unsigned long  Flags2;                               // +0x2c
    unsigned char  _pad30[0x28];
    unsigned long  SourceId;                             // +0x58
    void SetPitchProperties(const float* pitchTime, __int64 now);                     // 0x00C71F10
    void ApplyPitch(const float* pitchTime, __int64 now);                             // 0x00C71F80
    void StartFade(__int64 now, float target);                                        // 0x00C720C0
    bool UpdateGain(const CFadeParams* fade, bool restart, __int64 now);             // 0x00C72200
    unsigned long InitializeProperties(CAudioObject* sound, const CAudioSourceProperties* props, __int64 now);   // 0x00C72AF0
    void ApplyHardware(CAudioObject* stream, unsigned long flags);                    // 0x00C880A0
    void ApplySoftware(CAudioObject* sample, unsigned long flags);                    // 0x00C733D0
    void ApplySound(CAudioObject* sound, __int64 now);
};

void CAudioSource::ApplySound(CAudioObject* sound, __int64 now)
{
    const CAudioSourceProperties* props = &sound->Properties;
    unsigned long flags;
    if (Flags & 1)
        flags = InitializeProperties(sound, props, now);
    else
        flags = props->Flags;
    flags |= g_AudioContext.ForcedSourceFlags;
    if (flags & 1) {
        long looping;
        if (sound->LoopData && (unsigned char)(sound->StateFlags >> 8) & 1)
            looping = 1;
        else
            looping = 0;
        g_AudioContext.Sourcei(SourceId, 0x1007, looping);
    }
    if ((unsigned char)(Flags2 >> 9) & 1) {
        if (FadeOutTime <= now) {
            Flags2 &= ~0x200;
            Flags &= ~0x5e;
            if (props->Fade && !(Flags & 0x20)) {
                Flags |= 0x20;
                StartFade(now, props->Fade->Target);
            }
        }
    }
    if (!((unsigned char)(Flags >> 6) & 1)) {
        if (UpdateGain(props->Fade, (unsigned char)(flags >> 3) & 1, now))
            flags |= 8;
    }
    if ((flags & 0x20) && !(Flags & 1))
        SetPitchProperties(props->PitchTime, now);
    if (props->PitchTime) {
        ApplyPitch(props->PitchTime, now);
        flags |= 0x20;
    }
    if (flags || g_pAudioMaster->PendingUpdate) {
        if (IsHardware())
            ApplyHardware(sound->AsStream(), flags);
        else
            ApplySoftware(sound->AsSample(), flags);
    }
    Flags &= ~1;
}
