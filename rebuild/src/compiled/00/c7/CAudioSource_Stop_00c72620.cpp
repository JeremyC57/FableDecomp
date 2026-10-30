// CAudioSource::Stop() -- release the OpenAL source: stop it when it is playing or paused, detach its buffer (AL_BUFFER = 0), then, with the global context suspended (processed again only if it was running), delete the AL source name
// Leaf: ?Stop@CAudioSource@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C72620, 385 bytes, thiscall, `ret`. Slot 2 of the three OpenAL source vtables
//   0x012B8CC8 / 0x012B8D48 / 0x012BEE40 (xrefs 0x012B8CD0, 0x012B8D50, 0x012BEE48: its only references), right after the
//   landed slot 1 CAudioSource::Initialise 0x00C72930; it overrides slot 2 0x00C59710 of the voice base vtable 0x012B5C10
//   (which only marks the voice handle +0x2C invalid, 0x80000000).
// Callees (all on the global CAudioContext 0x0143BDB0, inline lazy resolvers): alGetSourcei (+0x68, "alGetSourcei"
//   0x012B8E64), alSourceStop (+0x7C, 0x012B8E84), alSourcei (+0x5C, 0x012B8E58), alcSuspendContext (+0x1C, 0x012B8790),
//   alDeleteSources (+0x48, 0x012B8E48), alcProcessContext (+0x20, 0x012B87A4); al entry points through
//   CAudioContext::GetProcAddress 0x00C4D740 when UseALGetProcAddress (+0x104) is set, else GetProcAddress (IAT 0x0143FE44)
//   on the module (+4); alc entry points always from the export table.
// Name: the slot name is proven, the class descriptive (GameCODA / libpk OpenAL layer: no PDB or RTTI). Slot 2 of the voice
//   base is bound `?Stop@CVtable_012B5C10@@UAEXXZ` (0x00C59710) by two landed callers: ~CVtable_012B5C10 0x00C3F3D0 (calls
//   it unless the stopped flag is set) and CAudioPlatform::ReleaseVoice 0x00C4D070 (`voice->Stop()` before pooling), so
//   this override is CAudioSource::Stop: it undoes Initialise's slot 1 (which generates the AL source under the same
//   suspend/process bracket). The class keeps its landed CAudioSource name. Manifest label "sub_00c72620".
// Shape: the AL enums AL_SOURCE_STATE 0x1010, AL_PLAYING 0x1012, AL_PAUSED 0x1013, AL_BUFFER 0x1009. The previous Suspended
//   flag rides in bl across the delete (as in Initialise). A reference local to the global context
//   (`CAudioContext& context = g_AudioContext;`) is what gives retail's registers: with direct `g_AudioContext.` calls
//   VC7.1 puts &SourceId in esi and the value in edi (every other byte equal, 384v385); with the reference the first value
//   load comes straight from [ecx+58h] into esi and &SourceId goes to edi, as retail. Named pointer locals, a pointer
//   declared after the first call, block scopes, a switch and the QFE compiler all left the swap.
// Layout: the landed CAudioContext / CAudioSource models (CAudioSource_Initialise_00c72930), with the five entry-point
//   slots this row uses typed.
extern "C" __declspec(dllimport) void* __stdcall GetProcAddress(void* module, const char* name);   // IAT 0x0143FE44

typedef void* (__cdecl* PFN_alcOpenDevice)(const char* deviceName);
typedef void* (__cdecl* PFN_alcCreateContext)(void* device, const long* attributes);
typedef unsigned char (__cdecl* PFN_alcMakeContextCurrent)(void* context);
typedef void (__cdecl* PFN_alcCloseDevice)(void* device);
typedef void (__cdecl* PFN_alcDestroyContext)(void* context);
typedef void (__cdecl* PFN_alcSuspendContext)(void* context);
typedef void (__cdecl* PFN_alcProcessContext)(void* context);
typedef unsigned char (__cdecl* PFN_alcIsExtensionPresent)(void* device, const char* name);
typedef const char* (__cdecl* PFN_alcGetString)(void* device, long param);
typedef void (__cdecl* PFN_alBufferData)(unsigned long buffer, long format, const void* data, long size, long frequency);
typedef void (__cdecl* PFN_alSourceQueueBuffers)(unsigned long source, long count, const unsigned long* buffers);
typedef const char* (__cdecl* PFN_alGetString)(long param);
typedef long (__cdecl* PFN_alGetError)();
typedef void (__cdecl* PFN_alGenSources)(long count, unsigned long* sources);
typedef void (__cdecl* PFN_alDeleteSources)(long count, const unsigned long* sources);
typedef void (__cdecl* PFN_alSourcei)(unsigned long source, long param, long value);
typedef void (__cdecl* PFN_alGetSourcei)(unsigned long source, long param, long* value);
typedef void (__cdecl* PFN_alSourceStop)(unsigned long source);
typedef unsigned char (__cdecl* PFN_alIsExtensionPresent)(const char* name);
typedef void* (__cdecl* PFN_alGetProcAddress)(const char* name);
typedef long (__cdecl* PFN_EAXGet)(const void* propertySetGuid, unsigned long property, unsigned long source, void* value, unsigned long size);

// Retail OpenAL wrapper (no RTTI; vtable 0x012B8C18, the global instance at 0x0143BDB0): layout of the landed
// CAudioContext rows (CheckError 0x00C4DBE0, Suspend 0x00C4D790, Process 0x00C4D7D0).
struct CAudioContext {
    virtual void UnusedSlot0();                          // +0x00 vtable 0x012B8C18 (slot 0 not called here)
    void*                     Module;                    // +0x04
    PFN_alcOpenDevice         pfnOpenDevice;             // +0x08
    PFN_alcCreateContext      pfnCreateContext;          // +0x0c
    PFN_alcMakeContextCurrent pfnMakeContextCurrent;     // +0x10
    PFN_alcCloseDevice        pfnCloseDevice;            // +0x14
    PFN_alcDestroyContext     pfnDestroyContext;         // +0x18
    PFN_alcSuspendContext     pfnSuspendContext;         // +0x1c
    PFN_alcProcessContext     pfnProcessContext;         // +0x20
    PFN_alcIsExtensionPresent pfnIsDeviceExtensionPresent;   // +0x24
    PFN_alcGetString          pfnGetDeviceString;        // +0x28
    void*                     pfnGenBuffers;             // +0x2c
    void*                     pfnDeleteBuffers;          // +0x30
    void*                     pfnOther_34;               // +0x34 (unnamed entry point)
    PFN_alBufferData          pfnBufferData;             // +0x38
    void*                     pfnOther_3c[2];            // +0x3c..+0x40 (unnamed entry points)
    PFN_alGenSources          pfnGenSources;             // +0x44
    PFN_alDeleteSources       pfnDeleteSources;          // +0x48
    void*                     pfnOther_4c[4];            // +0x4c..+0x58
    PFN_alSourcei             pfnSourcei;                // +0x5c
    void*                     pfnOther_60[2];            // +0x60..+0x64
    PFN_alGetSourcei          pfnGetSourcei;             // +0x68
    void*                     pfnSourcePlay;             // +0x6c
    void*                     pfnOther_70[3];            // +0x70..+0x78
    PFN_alSourceStop          pfnSourceStop;             // +0x7c
    void*                     pfnOther_80[3];            // +0x80..+0x88
    PFN_alSourceQueueBuffers  pfnSourceQueueBuffers;     // +0x8c
    void*                     pfnSourceUnqueueBuffers;   // +0x90
    void*                     pfnOther_94;               // +0x94
    void*                     pfnListener3f;             // +0x98
    void*                     pfnOther_9c[13];           // +0x9c..+0xcc
    PFN_alGetString           pfnGetString;              // +0xd0
    void*                     pfnDistanceModel;          // +0xd4
    void*                     pfnOther_d8[2];            // +0xd8..+0xdc
    PFN_alGetError            pfnGetError;               // +0xe0
    PFN_alIsExtensionPresent  pfnIsExtensionPresent;     // +0xe4
    PFN_alGetProcAddress      pfnGetProcAddress;         // +0xe8
    void*                     pfnGetEnumValue;           // +0xec
    void*                     pfnEAXSet;                 // +0xf0
    PFN_EAXGet                pfnEAXGet;                 // +0xf4
    void*                     m_reserved_f8;             // +0xf8
    void*                     Device;                    // +0xfc
    void*                     Context;                   // +0x100
    bool                      UseALGetProcAddress;       // +0x104
    unsigned char             _pad105[3];
    long                      PendingUpdates;            // +0x108
    bool                      m_10c;                     // +0x10c
    unsigned char             _pad10d[0x9f];
    const char*               DeviceName;                // +0x1ac
    bool                      Suspended;                 // +0x1b0
    unsigned char             _pad1b1[3];
    long                      RefCount;                  // +0x1b4
    void* GetProcAddress(const char* name);                       // 0x00C4D740 (resolve through alGetProcAddress)
    long GetError()                                      // 0x00C4D960
    {
        if (!pfnGetError)
            pfnGetError = (PFN_alGetError)(UseALGetProcAddress ? GetProcAddress("alGetError")
                                                               : ::GetProcAddress(Module, "alGetError"));
        return pfnGetError();
    }
    void Suspend()                                       // 0x00C4D790 (out-of-line copy; inlined here)
    {
        if (!pfnSuspendContext)
            pfnSuspendContext = (PFN_alcSuspendContext)::GetProcAddress(Module, "alcSuspendContext");
        pfnSuspendContext(Context);
        Suspended = true;
    }
    void Process()                                       // 0x00C4D7D0 (out-of-line copy; inlined here)
    {
        if (!pfnProcessContext)
            pfnProcessContext = (PFN_alcProcessContext)::GetProcAddress(Module, "alcProcessContext");
        pfnProcessContext(Context);
        Suspended = false;
    }
    void GenSources(long count, unsigned long* sources)
    {
        if (!pfnGenSources)
            pfnGenSources = (PFN_alGenSources)(UseALGetProcAddress ? GetProcAddress("alGenSources")
                                                                   : ::GetProcAddress(Module, "alGenSources"));
        pfnGenSources(count, sources);
    }
    void DeleteSources(long count, const unsigned long* sources)
    {
        if (!pfnDeleteSources)
            pfnDeleteSources = (PFN_alDeleteSources)(UseALGetProcAddress ? GetProcAddress("alDeleteSources")
                                                                         : ::GetProcAddress(Module, "alDeleteSources"));
        pfnDeleteSources(count, sources);
    }
    void Sourcei(unsigned long source, long param, long value)
    {
        if (!pfnSourcei)
            pfnSourcei = (PFN_alSourcei)(UseALGetProcAddress ? GetProcAddress("alSourcei")
                                                             : ::GetProcAddress(Module, "alSourcei"));
        pfnSourcei(source, param, value);
    }
    void GetSourcei(unsigned long source, long param, long* value)
    {
        if (!pfnGetSourcei)
            pfnGetSourcei = (PFN_alGetSourcei)(UseALGetProcAddress ? GetProcAddress("alGetSourcei")
                                                                   : ::GetProcAddress(Module, "alGetSourcei"));
        pfnGetSourcei(source, param, value);
    }
    void SourceStop(unsigned long source)
    {
        if (!pfnSourceStop)
            pfnSourceStop = (PFN_alSourceStop)(UseALGetProcAddress ? GetProcAddress("alSourceStop")
                                                                   : ::GetProcAddress(Module, "alSourceStop"));
        pfnSourceStop(source);
    }
    long CheckError();                                   // 0x00C4DBE0
};
extern CAudioContext g_AudioContext;                     // 0x0143BDB0

class CAudioObject;                                      // the sound a source plays (landed CAudioSource rows)

struct CVtable_012B5C10 {                                // the libpk voice base, vtable 0x012B5C10 (no RTTI)
    virtual ~CVtable_012B5C10();                         // slot 0
    virtual long Initialise(CAudioObject* sound);         // slot 1 (0x00C596E0)
    virtual void Stop();                         // slot 2 (0x00C59710: the handle at +0x2C = 0x80000000)
    unsigned char _body04[0x54];                         // +0x04 .. +0x58 (base state, not touched here)
};

struct CAudioSource : CVtable_012B5C10 {                 // the OpenAL source (vtables 0x012B8CC8 / 0x012B8D48 / 0x012BEE40)
    unsigned long  SourceId;                             // +0x58 the AL source name
    virtual long Initialise(CAudioObject* sound);         // slot 1 (0x00C72930)
    virtual void Stop();                         // slot 2 (0x00C72620)
};


enum { AL_BUFFER = 0x1009, AL_SOURCE_STATE = 0x1010, AL_PLAYING = 0x1012, AL_PAUSED = 0x1013 };

void CAudioSource::Stop()
{
    long state;
    CAudioContext& context = g_AudioContext;
    context.GetSourcei(SourceId, AL_SOURCE_STATE, &state);
    if (state == AL_PLAYING || state == AL_PAUSED)
        context.SourceStop(SourceId);
    context.Sourcei(SourceId, AL_BUFFER, 0);
    bool wasSuspended = context.Suspended;
    context.Suspend();
    context.DeleteSources(1, &SourceId);
    if (!wasSuspended)
        context.Process();
}