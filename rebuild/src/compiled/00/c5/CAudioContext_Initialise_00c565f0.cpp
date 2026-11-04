// CAudioContext::Initialise(bool force) -- start OpenAL once (again only when forced): try OpenAL32.dll then wrap_oal.dll from the current directory on the DirectSound3D device, else the system OpenAL32.dll (when it exists) on the Windows preferred device; keep the first that opens a device, creates a context and supports EAX 3, unwinding (destroy context, close device, free the DLL) after each failure
// Leaf: ?Initialise@CAudioContext@@QAE_N_N@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C565F0, 535 bytes, thiscall `ret 4`, /GS.
// Identity: libpk OpenAL wrapper (descriptive CAudioContext family 0x00C558A0..0x00C57730); strings "/OpenAL32.dll",
//   "/wrap_oal.dll", "DirectSound3D".
// Callees: GetCurrentDirectoryA / GetSystemDirectoryA / LoadLibraryA / FreeLibrary / FindFirstFileA / FindClose (IAT),
//   StringCatWorkerA 0x00C56090 (strsafe StringCchCatA inline), the CAudioContext members it chains.
// Shape: two DLL searches, each unwinding on failure; one shared `return false`.
typedef unsigned int size_t;
extern "C" __declspec(dllimport) unsigned long __stdcall GetCurrentDirectoryA(unsigned long length, char* buffer);   // IAT 0x0143FD78
extern "C" __declspec(dllimport) unsigned int __stdcall GetSystemDirectoryA(char* buffer, unsigned int size);       // IAT 0x0143FEBC
extern "C" __declspec(dllimport) void* __stdcall LoadLibraryA(const char* name);                                    // IAT 0x0143FE40
extern "C" __declspec(dllimport) int __stdcall FreeLibrary(void* module);                                           // IAT 0x0143FE3C
struct WIN32_FIND_DATAA {                               // winbase.h, 0x140 bytes
    unsigned long dwFileAttributes;
    unsigned long ftCreationTime[2];
    unsigned long ftLastAccessTime[2];
    unsigned long ftLastWriteTime[2];
    unsigned long nFileSizeHigh;
    unsigned long nFileSizeLow;
    unsigned long dwReserved0;
    unsigned long dwReserved1;
    char          cFileName[260];
    char          cAlternateFileName[14];
};
extern "C" __declspec(dllimport) void* __stdcall FindFirstFileA(const char* name, WIN32_FIND_DATAA* data);          // IAT 0x0143FEB8
extern "C" __declspec(dllimport) int __stdcall FindClose(void* find);                                               // IAT 0x0143FEB4
#define INVALID_HANDLE_VALUE ((void*)-1)
#define MAX_PATH 260
#define STRSAFE_MAX_CCH 2147483647
extern "C" long __stdcall StringCatWorkerA(char* pszDest, unsigned int cchDest, const char* pszSrc);               // 0x00C56090
__inline long StringCchCatA(char* pszDest, unsigned int cchDest, const char* pszSrc)   // strsafe.h
{
    long hr;
    if (cchDest > STRSAFE_MAX_CCH)
        hr = 0x80070057L;
    else
        hr = StringCatWorkerA(pszDest, cchDest, pszSrc);
    return hr;
}

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
    bool LoadEntryPoints();                    // 0x00C55980
    void DestroyContext();                     // 0x00C559C0
    bool OpenDevice(const char* name);         // 0x00C55AA0
    bool CreateContext();                      // 0x00C55B00
    void ClearEntryPoints();                   // 0x00C55C80
    bool GetPreferredDeviceName(char* deviceName);   // 0x00C55DA0
    void CloseDevice();                        // 0x00C56290
    bool SupportsEAX3();                       // 0x00C562F0
    bool Initialise(bool force);
};

bool CAudioContext::Initialise(bool force)
{
    char path[MAX_PATH + 1];
    char deviceName[MAX_PATH];
    WIN32_FIND_DATAA findData;
    if (m_10c && !force)
        return false;
    m_10c = true;
    if (GetCurrentDirectoryA(MAX_PATH + 1, path)) {
        StringCchCatA(path, MAX_PATH, "/OpenAL32.dll");
        Module = LoadLibraryA(path);
        if (!Module && GetCurrentDirectoryA(MAX_PATH + 1, path)) {
            StringCchCatA(path, MAX_PATH, "/wrap_oal.dll");
            Module = LoadLibraryA(path);
        }
        if (Module) {
            if (LoadEntryPoints() && OpenDevice("DirectSound3D")) {
                if (CreateContext()) {
                    if (SupportsEAX3())
                        return true;
                    DestroyContext();
                }
                CloseDevice();
            }
            FreeLibrary(Module);
            Module = 0;
        }
        ClearEntryPoints();
    }
    if (GetSystemDirectoryA(path, MAX_PATH + 1)) {
        StringCchCatA(path, MAX_PATH, "/OpenAL32.dll");
        void* find = FindFirstFileA(path, &findData);
        if (find != INVALID_HANDLE_VALUE) {
            FindClose(find);
            Module = LoadLibraryA(path);
            if (Module) {
                if (LoadEntryPoints() && OpenDevice(0)) {
                    bool found = GetPreferredDeviceName(deviceName);
                    CloseDevice();
                    if (found && OpenDevice(deviceName)) {
                        if (CreateContext()) {
                            if (SupportsEAX3())
                                return true;
                            DestroyContext();
                        }
                        CloseDevice();
                    }
                }
                FreeLibrary(Module);
                Module = 0;
            }
        }
    }
    return false;
}