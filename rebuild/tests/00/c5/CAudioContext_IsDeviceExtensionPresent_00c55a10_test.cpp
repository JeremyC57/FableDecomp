#include <cstdio>
#include <cstring>

typedef void* (*PFN_alcOpenDevice)(const char* deviceName);
typedef void* (*PFN_alcCreateContext)(void* device, const long* attributes);
typedef unsigned char (*PFN_alcMakeContextCurrent)(void* context);
typedef void (*PFN_alcCloseDevice)(void* device);
typedef void (*PFN_alcDestroyContext)(void* context);
typedef void (*PFN_alcSuspendContext)(void* context);
typedef void (*PFN_alcProcessContext)(void* context);
typedef unsigned char (*PFN_alcIsExtensionPresent)(void* device, const char* name);
typedef const char* (*PFN_alcGetString)(void* device, long param);
typedef void (*PFN_alBufferData)(unsigned int buffer, long format, const void* data, long size, long frequency);
typedef void (*PFN_alSourceQueueBuffers)(unsigned int source, long count, const unsigned int* buffers);
typedef const char* (*PFN_alGetString)(long param);
typedef long (*PFN_alGetError)();
typedef unsigned char (*PFN_alIsExtensionPresent)(const char* name);
typedef void* (*PFN_alGetProcAddress)(const char* name);
typedef long (*PFN_EAXGet)(const void* guid, unsigned int property, unsigned int source, void* value, unsigned int size);

enum { AL_NO_ERROR = 0, AL_INVALID_NAME = 0xA001, AL_INVALID_ENUM = 0xA002, AL_INVALID_VALUE = 0xA003,
       AL_INVALID_OPERATION = 0xA004, AL_OUT_OF_MEMORY = 0xA005, AL_VENDOR = 0xB001 };
enum { AUDIO_OK = 0, AUDIO_INVALID_ARGUMENT = -3, AUDIO_OUT_OF_MEMORY = -5, AUDIO_FAILED = -9 };

// ---- mock OpenAL DLL ----
static int  g_moduleCalls = 0;      // GetProcAddress(module, ...) lookups
static int  g_alCalls = 0;          // alGetProcAddress lookups
static const char* g_missing = "";  // a name the mock DLL does not export
static int  g_freed = 0;
static void* g_lastContextArg = 0;
static int  g_suspends = 0, g_processes = 0;
static long g_nextError = 0;
static unsigned int g_lastBuffer = 0; static long g_lastFormat = 0, g_lastSize = 0, g_lastFreq = 0; static const void* g_lastData = 0;
static unsigned int g_lastSource = 0; static long g_lastCount = 0; static const unsigned int* g_lastBuffers = 0;
static long g_eaxResult = 0; static unsigned int g_eaxSource = 99;
static const char* g_vendor = "Creative Labs Inc.";
static int g_hasEAX3 = 1;
static int g_deviceFails = 0, g_contextFails = 0;
static void* g_madeCurrent = (void*)1;
static int g_destroyed = 0, g_closed = 0;
static char g_device[4], g_context[4];

static void* m_alcOpenDevice(const char* name) { (void)name; return g_deviceFails ? 0 : (void*)g_device; }
static void* m_alcCreateContext(void* device, const long* attr) { (void)device; (void)attr; return g_contextFails ? 0 : (void*)g_context; }
static unsigned char m_alcMakeContextCurrent(void* c) { g_madeCurrent = c; return 1; }
static void m_alcCloseDevice(void* d) { (void)d; ++g_closed; }
static void m_alcDestroyContext(void* c) { g_lastContextArg = c; ++g_destroyed; }
static void m_alcSuspendContext(void* c) { g_lastContextArg = c; ++g_suspends; }
static void m_alcProcessContext(void* c) { g_lastContextArg = c; ++g_processes; }
static unsigned char m_alcIsExtensionPresent(void* d, const char* n) { (void)d; return std::strcmp(n, "ALC_ENUMERATION_EXT") == 0 ? 1 : 0; }
static const char* m_alcGetString(void* d, long p) { (void)d; return p == 0x1004 ? "Generic Hardware" : ""; }
static void m_alBufferData(unsigned int b, long f, const void* data, long s, long q) { g_lastBuffer = b; g_lastFormat = f; g_lastData = data; g_lastSize = s; g_lastFreq = q; }
static void m_alSourceQueueBuffers(unsigned int s, long n, const unsigned int* b) { g_lastSource = s; g_lastCount = n; g_lastBuffers = b; }
static const char* m_alGetString(long p) { return p == AL_VENDOR ? g_vendor : 0; }
static long m_alGetError() { return g_nextError; }
static unsigned char m_alIsExtensionPresent(const char* n) { return g_hasEAX3 && std::strcmp(n, "EAX3.0") == 0 ? 1 : 0; }
static long m_EAXGet(const void* g, unsigned int p, unsigned int s, void* v, unsigned int z) { (void)g; (void)p; (void)v; (void)z; g_eaxSource = s; return g_eaxResult; }
static void* m_lookup(const char* name)
{
    if (std::strcmp(name, g_missing) == 0) return 0;
    if (!std::strcmp(name, "alcOpenDevice")) return (void*)m_alcOpenDevice;
    if (!std::strcmp(name, "alcCreateContext")) return (void*)m_alcCreateContext;
    if (!std::strcmp(name, "alcMakeContextCurrent")) return (void*)m_alcMakeContextCurrent;
    if (!std::strcmp(name, "alcCloseDevice")) return (void*)m_alcCloseDevice;
    if (!std::strcmp(name, "alcDestroyContext")) return (void*)m_alcDestroyContext;
    if (!std::strcmp(name, "alcSuspendContext")) return (void*)m_alcSuspendContext;
    if (!std::strcmp(name, "alcProcessContext")) return (void*)m_alcProcessContext;
    if (!std::strcmp(name, "alcIsExtensionPresent")) return (void*)m_alcIsExtensionPresent;
    if (!std::strcmp(name, "alcGetString")) return (void*)m_alcGetString;
    if (!std::strcmp(name, "alBufferData")) return (void*)m_alBufferData;
    if (!std::strcmp(name, "alSourceQueueBuffers")) return (void*)m_alSourceQueueBuffers;
    if (!std::strcmp(name, "alGetString")) return (void*)m_alGetString;
    if (!std::strcmp(name, "alGetError")) return (void*)m_alGetError;
    if (!std::strcmp(name, "alIsExtensionPresent")) return (void*)m_alIsExtensionPresent;
    if (!std::strcmp(name, "alGenSources")) return (void*)m_alGetError;
    if (!std::strcmp(name, "EAXGet")) return (void*)m_EAXGet;
    return 0;
}
static void* MockGetProcAddress(void* module, const char* name) { (void)module; ++g_moduleCalls; return m_lookup(name); }
static int MockFreeLibrary(void* module) { (void)module; ++g_freed; return 1; }

struct CAudioContext {
    void* Module;
    PFN_alcOpenDevice pfnOpenDevice;
    PFN_alcCreateContext pfnCreateContext;
    PFN_alcMakeContextCurrent pfnMakeContextCurrent;
    PFN_alcCloseDevice pfnCloseDevice;
    PFN_alcDestroyContext pfnDestroyContext;
    PFN_alcSuspendContext pfnSuspendContext;
    PFN_alcProcessContext pfnProcessContext;
    PFN_alcIsExtensionPresent pfnIsDeviceExtensionPresent;
    PFN_alcGetString pfnGetDeviceString;
    PFN_alBufferData pfnBufferData;
    PFN_alSourceQueueBuffers pfnSourceQueueBuffers;
    PFN_alGetString pfnGetString;
    PFN_alGetError pfnGetError;
    PFN_alIsExtensionPresent pfnIsExtensionPresent;
    PFN_EAXGet pfnEAXGet;
    void* Device;
    void* Context;
    bool UseALGetProcAddress;
    long PendingUpdates;
    bool m_10c;
    const char* DeviceName;
    bool Suspended;
    long RefCount;
    int resets; bool lastResetForce;

    CAudioContext() { std::memset(this, 0, sizeof(*this)); Module = (void*)g_device; }
    void* GetProcAddress(const char* name) { ++g_alCalls; return m_lookup(name); }
    void Reset(bool force) { ++resets; lastResetForce = force; if (force || UseALGetProcAddress) { pfnBufferData = 0; pfnGetError = 0; pfnEAXGet = 0; } }
    void CloseDevice() { if (Device) { ++g_closed; Device = 0; DeviceName = 0; Reset(false); } }

    void Suspend()
    {
        if (!pfnSuspendContext)
            pfnSuspendContext = (PFN_alcSuspendContext)MockGetProcAddress(Module, "alcSuspendContext");
        pfnSuspendContext(Context);
        Suspended = true;
    }
    void SuspendContext() { Suspend(); }
    void Process()
    {
        if (!pfnProcessContext)
            pfnProcessContext = (PFN_alcProcessContext)MockGetProcAddress(Module, "alcProcessContext");
        pfnProcessContext(Context);
        Suspended = false;
    }
    void BufferData(unsigned int buffer, long format, const void* data, long size, long frequency)
    {
        if (!pfnBufferData)
            pfnBufferData = (PFN_alBufferData)(UseALGetProcAddress ? GetProcAddress("alBufferData") : MockGetProcAddress(Module, "alBufferData"));
        pfnBufferData(buffer, format, data, size, frequency);
    }
    void SourceQueueBuffers(unsigned int source, long count, const unsigned int* buffers)
    {
        if (!pfnSourceQueueBuffers)
            pfnSourceQueueBuffers = (PFN_alSourceQueueBuffers)(UseALGetProcAddress ? GetProcAddress("alSourceQueueBuffers") : MockGetProcAddress(Module, "alSourceQueueBuffers"));
        pfnSourceQueueBuffers(source, count, buffers);
    }
    long GetError()
    {
        if (!pfnGetError)
            pfnGetError = (PFN_alGetError)(UseALGetProcAddress ? GetProcAddress("alGetError") : MockGetProcAddress(Module, "alGetError"));
        return pfnGetError();
    }
    static long ConvertALError(long error)
    {
        if (error != AL_NO_ERROR)
            return error == AL_OUT_OF_MEMORY ? AUDIO_OUT_OF_MEMORY : AUDIO_FAILED;
        return AUDIO_OK;
    }
    long CheckError() { return ConvertALError(GetError()); }
    long EAXGetGlobal(const void* g, unsigned int p, void* v, unsigned int z)
    {
        if (!pfnEAXGet)
            pfnEAXGet = (PFN_EAXGet)GetProcAddress("EAXGet");
        return pfnEAXGet(g, p, 0, v, z);
    }
    long GetEAXGlobalProperty(const void* g, unsigned int p, void* v, unsigned int z)
    {
        long error = EAXGetGlobal(g, p, v, z);
        switch (error) {
        case AL_NO_ERROR: return AUDIO_OK;
        case AL_INVALID_NAME:
        case AL_INVALID_VALUE: return AUDIO_INVALID_ARGUMENT;
        case AL_INVALID_OPERATION: return AUDIO_INVALID_ARGUMENT;
        default: return ConvertALError(error);
        }
    }
    bool LoadEntryPoints()
    {
        if (!MockGetProcAddress(Module, "alcOpenDevice"))
            return false;
        UseALGetProcAddress = GetProcAddress("alGenSources") != 0;
        return true;
    }
    void DestroyContext()
    {
        if (Context) {
            pfnMakeContextCurrent(0);
            if (!pfnDestroyContext)
                pfnDestroyContext = (PFN_alcDestroyContext)MockGetProcAddress(Module, "alcDestroyContext");
            pfnDestroyContext(Context);
            Context = 0;
        }
    }
    bool IsDeviceExtensionPresent(const char* name)
    {
        if (!pfnIsDeviceExtensionPresent) {
            pfnIsDeviceExtensionPresent = (PFN_alcIsExtensionPresent)MockGetProcAddress(Module, "alcIsExtensionPresent");
            if (!pfnIsDeviceExtensionPresent)
                return false;
        }
        return pfnIsDeviceExtensionPresent(Device, name) == 1;
    }
    const char* GetDeviceString(long param)
    {
        if (!pfnGetDeviceString)
            pfnGetDeviceString = (PFN_alcGetString)MockGetProcAddress(Module, "alcGetString");
        return pfnGetDeviceString(Device, param);
    }
    bool OpenDevice(const char* name)
    {
        if (!pfnOpenDevice)
            pfnOpenDevice = (PFN_alcOpenDevice)MockGetProcAddress(Module, "alcOpenDevice");
        if (!pfnOpenDevice)
            return false;
        Device = pfnOpenDevice(name);
        if (!Device)
            return false;
        DeviceName = name;
        return true;
    }
    bool CreateContext()
    {
        if (!pfnCreateContext)
            pfnCreateContext = (PFN_alcCreateContext)MockGetProcAddress(Module, "alcCreateContext");
        if (!pfnMakeContextCurrent)
            pfnMakeContextCurrent = (PFN_alcMakeContextCurrent)MockGetProcAddress(Module, "alcMakeContextCurrent");
        if (pfnCreateContext && pfnMakeContextCurrent) {
            Context = pfnCreateContext(Device, 0);
            if (Context) {
                pfnMakeContextCurrent(Context);
                return true;
            }
        }
        return false;
    }
    bool IsExtensionPresent(const char* name)
    {
        if (!pfnIsExtensionPresent) {
            pfnIsExtensionPresent = (PFN_alIsExtensionPresent)(UseALGetProcAddress ? GetProcAddress("alIsExtensionPresent") : MockGetProcAddress(Module, "alIsExtensionPresent"));
            if (!pfnIsExtensionPresent)
                return false;
        }
        return pfnIsExtensionPresent(name) == 1;
    }
    const char* GetString(long param)
    {
        if (!pfnGetString)
            pfnGetString = (PFN_alGetString)(UseALGetProcAddress ? GetProcAddress("alGetString") : MockGetProcAddress(Module, "alGetString"));
        return pfnGetString(param);
    }
    bool SupportsEAX3()
    {
        const char* vendor = GetString(AL_VENDOR);
        if (!vendor)
            return false;
        return IsExtensionPresent("EAX3.0") && std::strncmp(vendor, "Creative", 8) == 0;
    }
    void Release()
    {
        if (--RefCount == 0) {
            DestroyContext();
            CloseDevice();
            if (Module)
                MockFreeLibrary(Module);
            Module = 0;
            m_10c = false;
            Reset(true);
            pfnOpenDevice = 0; pfnCreateContext = 0; pfnMakeContextCurrent = 0; pfnCloseDevice = 0; pfnDestroyContext = 0;
            pfnSuspendContext = 0; pfnProcessContext = 0; pfnIsDeviceExtensionPresent = 0; pfnGetDeviceString = 0;
            Device = 0; DeviceName = 0; UseALGetProcAddress = false; Suspended = false;
        }
    }
};

#define CHECK(x) do { if (!(x)) { std::printf("%s_TEST FAIL\n", "00c55a10"); return 1; } } while (0)

int main()
{
    CAudioContext c;
    CHECK(c.IsDeviceExtensionPresent("ALC_ENUMERATION_EXT") && !c.IsDeviceExtensionPresent("ALC_EXT_foo") && g_moduleCalls == 1);
    g_missing = "alcIsExtensionPresent"; CAudioContext d; CHECK(!d.IsDeviceExtensionPresent("ALC_ENUMERATION_EXT") && d.pfnIsDeviceExtensionPresent == 0);
    std::printf("00c55a10_TEST PASS\n");
    return 0;
}