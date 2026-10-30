// Behaviour test for CAudioContext::GetPreferredDeviceName 0x00C55DA0 (body copied from retail_asm/00c55da0.cpp; the winmm
// calls, GetProcAddress and the OpenAL entry points are mocked; under g++ the pushad/popad statements compile away).
// Retail-asm: 0x00C55DB4 `pushad` and 0x00C55DDA `popad` bracket the two winmm calls, as in retail_asm/00c55da0.cpp (VC7.1 never emits pusha/popa from C++).
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define __asm
#define pushad
#define popad
#endif
static void* g_ptrs[2];
static int g_gpaCalls = 0;
static bool g_haveIsExt = true;
static const char* g_preferred = "SB Live! Wave Device";
static const char* g_deviceList = 0;
static unsigned char g_enumerate = 1;
static unsigned long g_msgArg = 0;
void* __stdcall GetProcAddress(void* module, const char* name);

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
extern "C" unsigned int __stdcall waveOutMessage(void* hwo, unsigned int msg, unsigned long dw1, unsigned long dw2);   // IAT 0x0144049C
extern "C" unsigned int __stdcall waveOutGetDevCapsA(unsigned int deviceId, WAVEOUTCAPSA* caps, unsigned int size);  // IAT 0x014404A0
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
    waveOutMessage(WAVE_MAPPER, DRVM_MAPPER_PREFERRED_GET, (unsigned long)(size_t)&deviceId, (unsigned long)(size_t)&flags);
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

extern "C" unsigned int __stdcall waveOutMessage(void* hwo, unsigned int msg, unsigned long dw1, unsigned long dw2)
{
    if (hwo != WAVE_MAPPER || msg != DRVM_MAPPER_PREFERRED_GET) return 1;
    *(unsigned int*)(size_t)dw1 = 3; *(unsigned long*)(size_t)dw2 = 0; g_msgArg = 3;
    return 0;
}
extern "C" unsigned int __stdcall waveOutGetDevCapsA(unsigned int deviceId, WAVEOUTCAPSA* caps, unsigned int size)
{
    if (deviceId != 3 || size != sizeof(WAVEOUTCAPSA)) return 1;
    std::memset(caps, 0, sizeof(*caps));
    std::strncpy(caps->szPname, g_preferred, 31);
    return 0;
}
static const char* __cdecl MockGetString(void* device, long param) { return param == ALC_DEFAULT_DEVICE_SPECIFIER ? g_deviceList : 0; }
static unsigned char __cdecl MockIsExt(void* device, const char* name) { return std::strcmp(name, "ALC_ENUMERATION_EXT") == 0 ? g_enumerate : 0; }
void* __stdcall GetProcAddress(void* module, const char* name)
{
    ++g_gpaCalls;
    if (std::strcmp(name, "alcGetString") == 0) return (void*)&MockGetString;
    if (std::strcmp(name, "alcIsExtensionPresent") == 0) return g_haveIsExt ? (void*)&MockIsExt : 0;
    return 0;
}

static bool Run(const char* list, unsigned char enumerate, bool haveIsExt, char* out)
{
    static CAudioContext context;
    std::memset(&context, 0, sizeof(context));
    g_deviceList = list; g_enumerate = enumerate; g_haveIsExt = haveIsExt;
    std::memset(out, 'x', 0x110);
    return context.GetPreferredDeviceName(out);
}

int main()
{
    char out[0x110];
    // enumeration list: the match is the second entry (prefix compare over 255 chars against the 31-char caps name)
    static const char list[] = "Generic Software\0SB Live! Wave Device\0Generic Hardware\0";
    if (!Run(list, 1, true, out) || std::strcmp(out, "SB Live! Wave Device") != 0 || out[0x103] != 0 || g_msgArg != 3) {
        std::printf("00c55da0_TEST FAIL enum match\n"); return 1;
    }
    if (g_gpaCalls != 2) { std::printf("00c55da0_TEST FAIL lazy resolve %d\n", g_gpaCalls); return 1; }
    static const char none[] = "Generic Software\0Generic Hardware\0";
    if (Run(none, 1, true, out) || out[0] != 'x') { std::printf("00c55da0_TEST FAIL enum none\n"); return 1; }
    // no enumeration extension: only the single device string is compared
    if (!Run("SB Live! Wave Device", 0, true, out) || std::strcmp(out, "SB Live! Wave Device") != 0) { std::printf("00c55da0_TEST FAIL single\n"); return 1; }
    if (Run(list, 0, true, out)) { std::printf("00c55da0_TEST FAIL single first only\n"); return 1; }
    // alcIsExtensionPresent missing: same as no extension
    if (!Run("SB Live! Wave Device", 1, false, out)) { std::printf("00c55da0_TEST FAIL no isext\n"); return 1; }
    // no device string at all
    if (Run(0, 1, true, out) || Run(0, 0, true, out)) { std::printf("00c55da0_TEST FAIL null list\n"); return 1; }
    std::printf("00c55da0_TEST PASS\n");
    return 0;
}