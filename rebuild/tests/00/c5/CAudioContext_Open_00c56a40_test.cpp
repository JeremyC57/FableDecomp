// Behaviour test for 0x00C56A40 CAudioContext::Open: a non-null device GUID other than GUID_NULL is refused (-9); with no
// DLL loaded the inlined Initialise runs once (local OpenAL32.dll, wrap_oal.dll, then the system copy); success sets up the
// listener, selects exponent 1, clears +0x108, opens the mixer when a device name is known and bumps the global count.
#include <cstdio>
#include <cstring>
using std::memcmp;

#define MAX_PATH 260
#define INVALID_HANDLE_VALUE ((void*)-1)
struct WIN32_FIND_DATAA { char cFileName[MAX_PATH]; };
static int g_loadCalls = 0, g_freeCalls = 0, g_findOk = 1, g_okLib = 0;   // g_okLib: 1 local OpenAL32, 2 wrap_oal, 3 system
static char g_lastPath[MAX_PATH + 1];
static unsigned long GetCurrentDirectoryA(unsigned long, char* b) { std::strcpy(b, "C:/Game"); return 7; }
static unsigned int GetSystemDirectoryA(char* b, unsigned int) { std::strcpy(b, "C:/Win"); return 6; }
static void* LoadLibraryA(const char* p)
{
    ++g_loadCalls; std::strcpy(g_lastPath, p);
    if (g_okLib == 1 && !std::strcmp(p, "C:/Game/OpenAL32.dll")) return (void*)1;
    if (g_okLib == 2 && !std::strcmp(p, "C:/Game/wrap_oal.dll")) return (void*)2;
    if (g_okLib == 3 && !std::strcmp(p, "C:/Win/OpenAL32.dll")) return (void*)3;
    return 0;
}
static int FreeLibrary(void*) { ++g_freeCalls; return 1; }
static void* FindFirstFileA(const char*, WIN32_FIND_DATAA*) { return g_findOk ? (void*)5 : INVALID_HANDLE_VALUE; }
static int FindClose(void*) { return 1; }
static long StringCchCatA(char* d, unsigned int, const char* s) { std::strcat(d, s); return 0; }

struct GUID { unsigned int Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; };
static const GUID GUID_NULL = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };
struct SAudioDeviceDesc { const GUID* pDeviceGuid; };
struct CAudioMixer {
    int opened; float volume; int failOpen;
    bool EnumerateAndOpenDevice(const char*) { if (failOpen) return false; opened = 1; return true; }
    void SetVolume(float v) { volume = v; }
};
struct CAudioContext {
    void* Module; long m_distanceModel; bool m_10c; CAudioMixer m_mixer; const char* DeviceName; long RefCount;
    int listener, exponent, eaxOk;
    void SetDistanceExponent(long e) { exponent = (int)e; m_distanceModel = 2; }
    bool LoadEntryPoints() { return true; }
    void DestroyContext() { }
    bool OpenDevice(const char*) { return true; }
    bool CreateContext() { return true; }
    void ClearEntryPoints() { }
    bool GetPreferredDeviceName(char* n) { std::strcpy(n, "Generic Hardware"); return true; }
    void CloseDevice() { }
    bool SupportsEAX3() { return eaxOk != 0; }
    void InitialiseListener() { ++listener; }
    bool Initialise(bool force);
    long Open(unsigned int reserved, const SAudioDeviceDesc* desc);
};
static CAudioContext g_AudioContext;

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
long CAudioContext::Open(unsigned int reserved, const SAudioDeviceDesc* desc)
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

static int fail(int n) { std::printf("00c56a40_TEST FAIL %d\n", n); return 1; }
static void reset(int lib) { std::memset(&g_AudioContext, 0, sizeof(g_AudioContext)); g_AudioContext.eaxOk = 1; g_okLib = lib; g_loadCalls = g_freeCalls = 0; g_findOk = 1; }

int main()
{
    CAudioContext& c = g_AudioContext;
    GUID other = { 1, 2, 3, { 4, 5, 6, 7, 8, 9, 10, 11 } };
    SAudioDeviceDesc def = { 0 }, nul = { &GUID_NULL }, bad = { &other };
    reset(1);
    if (c.Open(0, &bad) != -9 || g_loadCalls != 0) return fail(1);
    if (c.Open(0, &def) != 0 || c.Module != (void*)1 || !c.m_10c || c.listener != 1 || c.exponent != 1 || c.m_distanceModel != 0 || c.RefCount != 1) return fail(2);
    if (c.m_mixer.opened) return fail(3);                        // no device name: no mixer
    c.DeviceName = "dev";
    if (c.Open(0, &nul) != 0 || g_loadCalls != 1 || c.RefCount != 2 || !c.m_mixer.opened || c.m_mixer.volume != 1.0f) return fail(4);
    reset(2);
    if (c.Open(0, &def) != 0 || c.Module != (void*)2 || g_loadCalls != 2) return fail(5);
    reset(3);
    if (c.Open(0, &def) != 0 || c.Module != (void*)3 || std::strcmp(g_lastPath, "C:/Win/OpenAL32.dll")) return fail(6);
    reset(3); g_findOk = 0;
    if (c.Open(0, &def) != -9 || c.Module != 0 || c.RefCount != 0 || c.listener) return fail(7);
    if (c.Open(0, &def) != -9 || g_loadCalls != 2) return fail(8);   // tried once: the m_10c guard skips the reload
    reset(1); c.eaxOk = 0;
    if (c.Open(0, &def) != -9 || g_freeCalls != 1 || c.Module != 0) return fail(9);
    std::printf("00c56a40_TEST PASS\n");
    return 0;
}