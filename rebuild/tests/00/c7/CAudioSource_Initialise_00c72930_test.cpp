// behaviour test for 0x00C72930 CAudioSource::Initialise (copy of the logic over a stub OpenAL context)
#include <cstdio>
#include <cstring>
typedef void (*PFN_alcSuspendContext)(void* context);
typedef void (*PFN_alcProcessContext)(void* context);
typedef void (*PFN_alGenSources)(long count, unsigned long* sources);
typedef long (*PFN_alGetError)();

static int g_log[16]; static int g_logCount = 0;
static void* g_context = (void*)0x1234;
static void* g_lastContext = 0;
static unsigned long g_nextSource = 7;
static long g_alError = 0;
static void stubSuspend(void* c) { g_lastContext = c; g_log[g_logCount++] = 2; }
static void stubProcess(void* c) { g_lastContext = c; g_log[g_logCount++] = 4; }
static void stubGenSources(long n, unsigned long* s) { for (long i = 0; i < n; ++i) s[i] = g_nextSource++; g_log[g_logCount++] = 3; }
static long stubGetError() { g_log[g_logCount++] = 1; long e = g_alError; g_alError = 0; return e; }
static int g_moduleLookups = 0, g_alLookups = 0;
static void* ModuleGetProcAddress(void*, const char* name)
{
    ++g_moduleLookups;
    if (!std::strcmp(name, "alcSuspendContext")) return (void*)stubSuspend;
    if (!std::strcmp(name, "alcProcessContext")) return (void*)stubProcess;
    if (!std::strcmp(name, "alGenSources")) return (void*)stubGenSources;
    if (!std::strcmp(name, "alGetError")) return (void*)stubGetError;
    return 0;
}
struct CAudioContext {
    void* Module;
    PFN_alcSuspendContext pfnSuspendContext;
    PFN_alcProcessContext pfnProcessContext;
    PFN_alGenSources pfnGenSources;
    PFN_alGetError pfnGetError;
    void* Context;
    bool UseALGetProcAddress;
    bool Suspended;
    void* GetProcAddress(const char* name) { ++g_alLookups; --g_moduleLookups; return ModuleGetProcAddress(0, name); }
    long GetError()
    {
        if (!pfnGetError)
            pfnGetError = (PFN_alGetError)(UseALGetProcAddress ? GetProcAddress("alGetError") : ModuleGetProcAddress(Module, "alGetError"));
        return pfnGetError();
    }
    void Suspend()
    {
        if (!pfnSuspendContext) pfnSuspendContext = (PFN_alcSuspendContext)ModuleGetProcAddress(Module, "alcSuspendContext");
        pfnSuspendContext(Context);
        Suspended = true;
    }
    void Process()
    {
        if (!pfnProcessContext) pfnProcessContext = (PFN_alcProcessContext)ModuleGetProcAddress(Module, "alcProcessContext");
        pfnProcessContext(Context);
        Suspended = false;
    }
    void GenSources(long count, unsigned long* sources)
    {
        if (!pfnGenSources)
            pfnGenSources = (PFN_alGenSources)(UseALGetProcAddress ? GetProcAddress("alGenSources") : ModuleGetProcAddress(Module, "alGenSources"));
        pfnGenSources(count, sources);
    }
    long CheckError()
    {
        long error = GetError();
        if (error != 0) return error == 0xA005 ? -5 : -9;
        return 0;
    }
};
static CAudioContext g_AudioContext;
struct CAudioSound { long baseResult; };
struct CVtable_012B5C10 {
    virtual ~CVtable_012B5C10() {}
    virtual long Initialise(CAudioSound* sound) { m_initialised = true; return sound->baseResult; }
    bool m_initialised;
};
struct CAudioSource : CVtable_012B5C10 {
    unsigned long SourceId;
    virtual long Initialise(CAudioSound* sound);
};
long CAudioSource::Initialise(CAudioSound* sound)
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
int main()
{
    std::memset(&g_AudioContext, 0, sizeof(g_AudioContext));
    g_AudioContext.Context = g_context;
    CAudioSource src; src.SourceId = 0; src.m_initialised = false;
    CAudioSound bad = { -3 };
    if (src.Initialise(&bad) != -3 || g_logCount != 0 || !src.m_initialised) { std::printf("00c72930_TEST FAIL\n"); return 1; }
    CAudioSound ok = { 0 };
    g_alError = 0xA001;                                  // a stale error is cleared first
    long r = src.Initialise(&ok);
    int want[5] = { 1, 2, 3, 4, 1 };
    if (r != 0 || src.SourceId != 7 || g_logCount != 5 || std::memcmp(g_log, want, sizeof(want)) != 0 ||
        g_AudioContext.Suspended || g_lastContext != g_context || g_moduleLookups != 4)
    { std::printf("00c72930_TEST FAIL\n"); return 1; }
    // already suspended: stays suspended (no Process); al entry points through alGetProcAddress are cached already
    g_logCount = 0; g_AudioContext.Suspended = true;
    r = src.Initialise(&ok);
    int want2[4] = { 1, 2, 3, 1 };
    if (r != 0 || src.SourceId != 8 || g_logCount != 4 || std::memcmp(g_log, want2, sizeof(want2)) != 0 || !g_AudioContext.Suspended)
    { std::printf("00c72930_TEST FAIL\n"); return 1; }
    std::printf("00c72930_TEST PASS\n");
    return 0;
}