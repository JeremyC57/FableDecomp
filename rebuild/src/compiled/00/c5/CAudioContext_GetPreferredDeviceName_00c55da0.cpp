// CAudioContext::GetPreferredDeviceName(char* deviceName) -- copy into deviceName the OpenAL device whose name starts with the Windows preferred waveOut device's name (waveOutMessage DRVM_MAPPER_PREFERRED_GET + waveOutGetDevCapsA, inside the retail pushad/popad guard), searching the ALC_ENUMERATION_EXT device list or else the single device string; false when none matches
// Leaf: ?GetPreferredDeviceName@CAudioContext@@QAE_NPAD@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C55DA0, 348 bytes, thiscall `ret 4`, ebp frame (inline asm), /GS cookie
//   0x0139C8A8 / __security_check_cookie 0x00BFE9F9.
// Retail-asm: 0x00C55DB4 `pushad` and 0x00C55DDA `popad` bracket the two winmm calls (registers saved around the driver
//   query): VC7.1 never emits pusha/popa from C or C++, so the original had `__asm pushad` / `__asm popad` statements;
//   everything else is compiler-emitted (the asm also forces the ebp frame and keeps the named bool in its frame byte).
// Identity: libpk's OpenAL wrapper (descriptive CAudioContext, the landed family 0x00C558A0..0x00C57730, vtable 0x012B8C18:
//   Module +4, pfnIsDeviceExtensionPresent +0x24, pfnGetDeviceString +0x28, Device +0xfc). Method name descriptive (libpk has
//   no PDB); callers 0x00C56792 / 0x00C56BED (between OpenDevice(0) / CloseDevice and OpenDevice(name) in the device
//   start-up). The catalog label Audio_FindWaveOutDevice is descriptive only.
// Callees: waveOutMessage (IAT 0x0144049C), waveOutGetDevCapsA (IAT 0x014404A0), GetProcAddress (IAT 0x0143FE44), strncmp
//   0x00BFEAF8, strncpy 0x00BFE9C2 (CRT, extern "C"); CAudioContext::GetDeviceString 0x00C55A60 and
//   IsDeviceExtensionPresent 0x00C55A10 inline here (their landed bodies, defined in the class).
// Data: "alcGetString" 0x012B8BDC, "alcIsExtensionPresent" 0x012B8BC4, "ALC_ENUMERATION_EXT" 0x012B8C48.
// Shape: device string list walk `devices += strlen(devices)` (inline strlen), a NUL with a NUL after it ends the list;
//   the found pointer doubles as the result flag.
typedef unsigned int size_t;
extern "C" size_t __cdecl strlen(const char* s);
#pragma intrinsic(strlen)
extern "C" int __cdecl strncmp(const char* a, const char* b, size_t count);       // 0x00BFEAF8
extern "C" char* __cdecl strncpy(char* dest, const char* source, size_t count);   // 0x00BFE9C2
extern "C" __declspec(dllimport) void* __stdcall GetProcAddress(void* module, const char* name);   // IAT 0x0143FE44

struct WAVEOUTCAPSA {                                   // mmsystem.h, 0x34 bytes
    unsigned short wMid;
    unsigned short wPid;
    unsigned int   vDriverVersion;
    char           szPname[32];
    unsigned long  dwFormats;
    unsigned short wChannels;
    unsigned short wReserved1;
    unsigned long  dwSupport;
};
extern "C" __declspec(dllimport) unsigned int __stdcall waveOutMessage(void* hwo, unsigned int msg, unsigned long dw1, unsigned long dw2);   // IAT 0x0144049C
extern "C" __declspec(dllimport) unsigned int __stdcall waveOutGetDevCapsA(unsigned int deviceId, WAVEOUTCAPSA* caps, unsigned int size);  // IAT 0x014404A0
#define WAVE_MAPPER ((void*)-1)
#define DRVM_MAPPER_PREFERRED_GET 0x2015
#define ALC_DEFAULT_DEVICE_SPECIFIER 0x1004

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
    const char* GetDeviceString(long param)            // 0x00C55A60 (inline here)
    {
        if (!pfnGetDeviceString)
            pfnGetDeviceString = (PFN_alcGetString)::GetProcAddress(Module, "alcGetString");
        return pfnGetDeviceString(Device, param);
    }
    bool IsDeviceExtensionPresent(const char* name)     // 0x00C55A10 (inline here)
    {
        if (!pfnIsDeviceExtensionPresent) {
            pfnIsDeviceExtensionPresent = (PFN_alcIsExtensionPresent)::GetProcAddress(Module, "alcIsExtensionPresent");
            if (!pfnIsDeviceExtensionPresent)
                return false;
        }
        return pfnIsDeviceExtensionPresent(Device, name) == 1;
    }
    bool GetPreferredDeviceName(char* deviceName);
};

bool CAudioContext::GetPreferredDeviceName(char* deviceName)
{
    unsigned long flags;
    unsigned int deviceId;
    WAVEOUTCAPSA caps;
    __asm pushad
    waveOutMessage(WAVE_MAPPER, DRVM_MAPPER_PREFERRED_GET, (unsigned long)&deviceId, (unsigned long)&flags);
    waveOutGetDevCapsA(deviceId, &caps, sizeof(caps));
    __asm popad

    const char* devices = GetDeviceString(ALC_DEFAULT_DEVICE_SPECIFIER);
    bool enumeration = IsDeviceExtensionPresent("ALC_ENUMERATION_EXT");
    if (enumeration) {
        while (devices) {
            if (strncmp(devices, caps.szPname, 255) == 0)
                break;
            devices += strlen(devices);
            if (*devices == 0) {
                if (devices[1] == 0)
                    devices = 0;
                else
                    ++devices;
            }
        }
        if (!devices)
            return false;
    } else if (!devices || strncmp(devices, caps.szPname, 255) != 0)
        return false;
    strncpy(deviceName, devices, 0x103);
    deviceName[0x103] = 0;
    return true;
}