// CAudioSource::ApplySoftware(CAudioObject* sample, unsigned long flags) -- push a software-mixed source's changed properties to OpenAL: the gain (sample gain x source fade gain x the context's master gain, clamped to 1) when bits 1 or 3 are set; the pitch (sample pitch x source pitch scale) when bits 2 or 5 are set, retried at 0.8x while the implementation reports AL_INVALID_VALUE; and the sample's two EAX source properties (5 and 7, bits 13 and 14) through EAXSet
// Grade: DIFFER(610v610) -- register choice only: retail keeps `flags` in ebx and re-reads `sample` from its argument slot (and homes the float temp in the flags slot); VC7.1 here enregisters `sample` and tests `flags` in memory; every call, branch and the inlining pattern match
// Leaf: ?ApplySoftware@CAudioSource@@
extern "C" __declspec(dllimport) void* __stdcall GetProcAddress(void* module, const char* name);   // IAT 0x0143FE44

typedef void* (__cdecl* PFN_alGetProcAddress)(const char* name);
typedef void (__cdecl* PFN_alSourcef)(unsigned long source, long param, float value);
typedef long (__cdecl* PFN_alGetError)();
typedef long (__cdecl* PFN_EAXSet)(const void* propertySetGuid, unsigned long property, unsigned long source, void* value, unsigned long size);

extern const unsigned char g_eaxSourcePropertySetGuid_012beea0[16];   // 0x012BEEA0

// Retail OpenAL wrapper (the landed CAudioContext model, the global instance at 0x0143BDB0): the DLL module handle at +4,
// lazily resolved entry points, the alGetProcAddress flag at +0x104. GetProcAddress (0x00C4D740) and EAXSetSource
// (0x00C73380) are header-defined: retail inlines EAXSetSource at both EAX sites and GetProcAddress inside the pitch
// retry loop only, and calls the out-of-line copies elsewhere.
struct CAudioContext {
    void*              __vftable;                   // +0x00
    void*              Module;                      // +0x04 (0x0143BDB4)
    unsigned char      _pad08[0x48];
    PFN_alSourcef      pfnSourcef;                  // +0x50 (0x0143BE00) "alSourcef" @ 0x012BEE14
    unsigned char      _pad54[0x8c];
    PFN_alGetError     pfnGetError;                 // +0xe0 (0x0143BE90) "alGetError" @ 0x012B87E8
    unsigned char      _pade4[4];
    PFN_alGetProcAddress pfnGetProcAddress;         // +0xe8 (0x0143BE98) "alGetProcAddress" @ 0x012B877C
    unsigned char      _padec[4];
    PFN_EAXSet         pfnEAXSet;                   // +0xf0 (0x0143BEA0) "EAXSet" @ 0x012B8B80
    unsigned char      _padf4[4];
    float              MasterGain;                  // +0xf8 (0x0143BEA8)
    unsigned char      _padfc[8];
    bool               UseALGetProcAddress;         // +0x104 (0x0143BEB4)
    void* GetProcAddress(const char* name)          // 0x00C4D740
    {
        if (!pfnGetProcAddress) {
            pfnGetProcAddress = (PFN_alGetProcAddress)::GetProcAddress(Module, "alGetProcAddress");
            if (UseALGetProcAddress)
                pfnGetProcAddress = (PFN_alGetProcAddress)pfnGetProcAddress("alGetProcAddress");
        }
        return pfnGetProcAddress(name);
    }
    void Sourcef(unsigned long source, long param, float value)
    {
        if (!pfnSourcef)
            pfnSourcef = (PFN_alSourcef)(UseALGetProcAddress ? GetProcAddress("alSourcef")
                                                             : ::GetProcAddress(Module, "alSourcef"));
        pfnSourcef(source, param, value);
    }
    PFN_alGetError GetErrorProc()
    {
        if (!pfnGetError)
            pfnGetError = (PFN_alGetError)(UseALGetProcAddress ? GetProcAddress("alGetError")
                                                               : ::GetProcAddress(Module, "alGetError"));
        return pfnGetError;
    }
    long GetError()
    {
        if (!pfnGetError)
            pfnGetError = (PFN_alGetError)(UseALGetProcAddress ? GetProcAddress("alGetError")
                                                               : ::GetProcAddress(Module, "alGetError"));
        return pfnGetError();
    }
    long EAXSetSource(unsigned long source, unsigned long property, void* value, unsigned long size)   // 0x00C73380
    {
        if (!pfnEAXSet)
            pfnEAXSet = (PFN_EAXSet)GetProcAddress("EAXSet");
        return pfnEAXSet(g_eaxSourcePropertySetGuid_012beea0, property, source, value, size);
    }
};
extern CAudioContext g_AudioContext;                // 0x0143BDB0

class CAudioObject {                                // the sample a software source plays
public:
    unsigned char  _pad00[0x18];
    float          Gain;                            // +0x18
    unsigned char  _pad1c[4];
    float          Pitch;                           // +0x20
    unsigned char  _pad24[0xdc];
    long           EaxProperty5;                    // +0x100
    long           EaxProperty7;                    // +0x104
};

struct CAudioSource {
    unsigned char  _pad00[0x30];
    float          FadeGain;                        // +0x30
    unsigned char  _pad34[8];
    float          PitchScale;                      // +0x3c
    unsigned char  _pad40[0x18];
    unsigned long  SourceId;                        // +0x58
    void ApplySoftware(CAudioObject* sample, unsigned long flags);
};

void CAudioSource::ApplySoftware(CAudioObject* sample, unsigned long flags)
{
    CAudioContext& context = g_AudioContext;
    if (flags & 0xa) {
        float gain = sample->Gain * FadeGain * context.MasterGain;
        if (gain > 1.0f)
            gain = 1.0f;
        context.Sourcef(SourceId, 0x100a /* AL_GAIN */, gain);
    }
    if (flags & 0x24) {
        context.GetError();
        float pitch = sample->Pitch * PitchScale;
        long error;
        do {
            if (!(pitch > 0.0f))
                break;
            context.Sourcef(SourceId, 0x1003 /* AL_PITCH */, pitch);
            PFN_alGetError alGetError = context.GetErrorProc();
            pitch *= 0.8f;
            error = alGetError();
        } while (error == 0xa003 /* AL_INVALID_VALUE */);
    }
    if (flags & 0x2000)
        context.EAXSetSource(SourceId, 5, &sample->EaxProperty5, 4);
    if (flags & 0x4000)
        context.EAXSetSource(SourceId, 7, &sample->EaxProperty7, 4);
}
