// CAudioSource::Initialise(CAudioSound* sound) -- the OpenAL source set-up: run the voice base's Initialise, then clear the AL error state and, with the global context suspended (restored only if it was running), generate this source's AL name; returns the context's translated AL error
// Leaf: ?Initialise@CAudioSource@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C72930, 259 bytes, thiscall, `ret 4`. Slot 1 of the three OpenAL source vtables
//   0x012B8CC8 / 0x012B8D48 / 0x012BEE40 (xrefs 0x012B8CCC, 0x012B8D4C, 0x012BEE44; slot 8 is the landed CAudioSource::Update
//   0x00C72FE0), which override slot 1 0x00C596E0 of the voice base vtable 0x012B5C10.
// Callees: the base slot 1 0x00C596E0 (direct call: clears +0x0c/+0x2c and takes the sound's +0xf4 word into +0x50, returns 0),
//   on the global CAudioContext 0x0143BDB0: GetProcAddress 0x00C4D740 / GetProcAddress (IAT 0x0143FE44) for "alGetError",
//   "alcSuspendContext", "alGenSources", "alcProcessContext", the resolved cdecl entry points, CheckError 0x00C4DBE0.
// Name: descriptive (libpk/OpenAL layer: no PDB or RTTI). The class keeps the landed CAudioSource name of its slot 8 and
//   the base keeps its landed CVtable_012B5C10 name; "Initialise" for slot 1 (base: reset the voice state from the sound).
// Shape: the context's inline GetError (result dropped), Suspend and Process (the landed out-of-line copies 0x00C4D790 /
//   0x00C4D7D0: alc entry points come only from the DLL export table) and a GenSources wrapper in the al pattern
//   (alGetProcAddress when UseALGetProcAddress is set); the previous Suspended flag lives in bl across the calls.
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
    void*                     pfnDeleteSources;          // +0x48
    void*                     pfnOther_4c[4];            // +0x4c..+0x58
    void*                     pfnSourcei;                // +0x5c
    void*                     pfnOther_60[2];            // +0x60..+0x64
    void*                     pfnGetSourcei;             // +0x68
    void*                     pfnSourcePlay;             // +0x6c
    void*                     pfnOther_70[3];            // +0x70..+0x78
    void*                     pfnSourceStop;             // +0x7c
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
    long CheckError();                                   // 0x00C4DBE0
};
extern CAudioContext g_AudioContext;                     // 0x0143BDB0

class CAudioObject;                                      // the sound a source plays (landed CAudioSource rows)

struct CVtable_012B5C10 {                                // the libpk voice base, vtable 0x012B5C10 (no RTTI)
    virtual ~CVtable_012B5C10();                         // slot 0
    virtual long Initialise(CAudioObject* sound);         // slot 1 (0x00C596E0)
    unsigned char _body04[0x54];                         // +0x04 .. +0x58 (base state, not touched here)
};

struct CAudioSource : CVtable_012B5C10 {                 // the OpenAL source (vtables 0x012B8CC8 / 0x012B8D48 / 0x012BEE40)
    unsigned long  SourceId;                             // +0x58 the AL source name
    virtual long Initialise(CAudioObject* sound);         // slot 1 (0x00C72930)
};

long CAudioSource::Initialise(CAudioObject* sound)
{
    long result = CVtable_012B5C10::Initialise(sound);
    if (result < 0)
        return result;
    g_AudioContext.GetError();
    bool wasSuspended = g_AudioContext.Suspended;
    g_AudioContext.Suspend();
    g_AudioContext.GenSources(1, &SourceId);
    if (!wasSuspended)
        g_AudioContext.Process();
    return g_AudioContext.CheckError();
}