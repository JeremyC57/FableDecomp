// CAudioContext::Open(unsigned long reserved, const SAudioDeviceDesc* desc) -- vtable slot 1 of libpk's OpenAL context: refuse any device GUID but GUID_NULL (-9); start OpenAL when no DLL is loaded yet (Initialise, inlined); then set up the listener, select distance exponent 1 through slot 7, clear +0x108, open the system mixer of the chosen OpenAL device at full volume, count the open and return 0 (-9 when no OpenAL DLL could be loaded)
// Leaf: ?Open@CAudioContext@@UAEJKPBUSAudioDeviceDesc@@@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C56A40, 656 bytes, thiscall `ret 8`, /GS (cookie 0x0139C8A8, check 0x00BFE9F9),
// frame 0x350 (the inlined Initialise's path, device name and WIN32_FIND_DATAA).
// Identity: slot 1 of the CAudioContext vtable 0x012B8C18 (data ref 0x012B8C1C; slot 0 the deleting destructor 0x00C565D0,
// slot 7 the landed SetDistanceExponent 0x00C55D70); the landed InitialiseListener 0x00C568F0 names this routine as its only
// caller. The middle 0x1B0 bytes are the landed CAudioContext::Initialise 0x00C565F0 expanded with force = false (same
// strings "/OpenAL32.dll" / "/wrap_oal.dll" / "DirectSound3D", same import and member chain). Method and descriptor names
// are descriptive (libpk, no PDB/RTTI): the first argument is never read; the second only for the device GUID at +0x10,
// compared with uuid.lib's GUID_NULL 0x012C0694 by the memcmp intrinsic (`repe cmpsd`). The global 0x0143BF64 is
// g_AudioContext.RefCount (0x0143BDB0 + 0x1B4), bumped through the global as in the landed CAudioBuffer::Create 0x00C4DC40.
// Callees: the Initialise chain (LoadEntryPoints 0x00C55980, OpenDevice 0x00C55AA0, CreateContext 0x00C55B00, SupportsEAX3
// 0x00C562F0, DestroyContext 0x00C559C0, CloseDevice 0x00C56290, ClearEntryPoints 0x00C55C80, GetPreferredDeviceName
// 0x00C55DA0, StringCatWorkerA 0x00C56090, KERNEL32 imports), InitialiseListener 0x00C568F0, slot 7 SetDistanceExponent,
// CAudioMixer::EnumerateAndOpenDevice 0x00C71430 / SetVolume 0x00C71640 on the +0x110 mixer.
// Shape: Initialise is the landed body, defined `inline` in the TU: VC7.1 expands it at this one call (no flag). The two
// -9 exits share one block at the function end only in the nested form (GUID ok -> load -> module ok -> return 0, then a
// single `return -9`); two early `return -9`s keep the second one on the fall-through (grade 1).
#include <string.h>
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

typedef struct _GUID { unsigned long Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; } GUID;
extern "C" const GUID GUID_NULL;                    // 0x012C0694 (uuid.lib)

struct SAudioDeviceDesc {
    unsigned char _pad00[0x10];
    const GUID*   pDeviceGuid;                      // +0x10 (0 or GUID_NULL = the default OpenAL device)
};

struct CAudioMixer {                                // the +0x110 member (landed 0x00C71430 / 0x00C71640)
    void*         Mixer;
    unsigned char _body[0x98];
    bool EnumerateAndOpenDevice(const char* deviceName);    // 0x00C71430
    void SetVolume(float level);                            // 0x00C71640
};

struct CAudioContext {                              // vtable 0x012B8C18
    virtual ~CAudioContext();                       // slot 0 (0x00C565D0 deleting)
    virtual long Open(unsigned long reserved, const SAudioDeviceDesc* desc);    // slot 1
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void SetDistanceExponent(long exponent);  // slot 7 (0x00C55D70)
    void*                     Module;                    // +0x04
    unsigned char             _pad08[0x100 - 0x08];
    void*                     Context;                   // +0x100
    bool                      UseALGetProcAddress;       // +0x104
    unsigned char             _pad105[3];
    long                      m_distanceModel;           // +0x108
    bool                      m_10c;                     // +0x10c
    unsigned char             _pad10d[3];
    CAudioMixer               m_mixer;                   // +0x110
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
    void InitialiseListener();                 // 0x00C568F0
    bool Initialise(bool force);               // 0x00C565F0 (expanded below)
};
extern CAudioContext g_AudioContext;                // 0x0143BDB0

inline bool CAudioContext::Initialise(bool force)
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
long CAudioContext::Open(unsigned long reserved, const SAudioDeviceDesc* desc)
{
    if (!desc->pDeviceGuid || memcmp(desc->pDeviceGuid, &GUID_NULL, sizeof(GUID)) == 0) {
        if (!Module)
            Initialise(false);
        if (Module) {
            InitialiseListener();
            SetDistanceExponent(1);
            m_distanceModel = 0;
            if (DeviceName && m_mixer.EnumerateAndOpenDevice(DeviceName))
                m_mixer.SetVolume(1.0f);
            ++g_AudioContext.RefCount;
            return 0;
        }
    }
    return -9;
}