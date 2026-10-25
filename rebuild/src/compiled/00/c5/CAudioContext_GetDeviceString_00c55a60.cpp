// CAudioContext::GetDeviceString(long param) -- resolve alcGetString on first use and return the device string
// Leaf: ?GetDeviceString@CAudioContext@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C55A60, length 50 bytes (+ int3 padding); `ret 4`.
// Callees: GetProcAddress (IAT 0x0143FE44, "alcGetString" @ 0x012B8BDC), the resolved entry point (cdecl, device + param).
// No direct callers. The catalog label "CAudioContext_GetStringProc" is propagated.
extern "C" __declspec(dllimport) void* __stdcall GetProcAddress(void* module, const char* name);   // IAT 0x0143FE44
extern "C" __declspec(dllimport) int __stdcall FreeLibrary(void* module);                           // IAT 0x0143FE3C

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
typedef unsigned char (__cdecl* PFN_alIsExtensionPresent)(const char* name);
typedef void* (__cdecl* PFN_alGetProcAddress)(const char* name);
typedef long (__cdecl* PFN_EAXGet)(const void* propertySetGuid, unsigned long property, unsigned long source, void* value, unsigned long size);

enum {
    AL_NO_ERROR = 0,
    AL_INVALID_NAME = 0xA001,
    AL_INVALID_ENUM = 0xA002,
    AL_INVALID_VALUE = 0xA003,
    AL_INVALID_OPERATION = 0xA004,
    AL_OUT_OF_MEMORY = 0xA005,
    AL_VENDOR = 0xB001
};

enum EAudioResult {
    AUDIO_OK = 0,
    AUDIO_INVALID_ARGUMENT = -3,
    AUDIO_OUT_OF_MEMORY = -5,
    AUDIO_FAILED = -9
};

// Retail OpenAL wrapper (no RTTI; vtable 0x012B8C18, the global instance at 0x0143BDB0): the OpenAL32.dll /
// wrap_oal.dll module at +4, the lazily resolved alc entry points (+8..+0x28, always from the DLL export table) and
// al / EAX entry points (+0x2c..+0xf4, through alGetProcAddress when +0x104 is set), the device +0xfc, the context
// +0x100, the suspended flag +0x1b0 and the reference count +0x1b4. Offsets of the named entry points come from the
// name strings pushed before each store (0x012B8790.. 0x012B8EAC); the landed CAudioContext rows use the same layout.
struct CAudioContext {
    void*                     __vftable;                 // +0x00
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
    void*                     pfnGenSources;             // +0x44
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
    const char* GetDeviceString(long param);
};

const char* CAudioContext::GetDeviceString(long param)
{
    if (!pfnGetDeviceString)
        pfnGetDeviceString = (PFN_alcGetString)::GetProcAddress(Module, "alcGetString");
    return pfnGetDeviceString(Device, param);
}