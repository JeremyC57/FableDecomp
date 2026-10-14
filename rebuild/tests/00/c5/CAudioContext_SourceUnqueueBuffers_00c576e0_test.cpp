#include <cstdio>
#include <cstring>
struct CAudioContext;
static void* g_module = (void*)0x10; static const char* g_lastName = 0; static int g_dllLookups = 0, g_alLookups = 0;
static void* Lookup(const char* name);
static void* __cdecl FakeALGetProc(const char* name) { ++g_alLookups; g_lastName = name; return Lookup(name); }
static void __cdecl FakeProcessContext(void* ctx) { g_lastName = (const char*)ctx; }
static float g_l[3]; static long g_param;
static void __cdecl FakeListener3f(long p, float a, float b, float c) { g_param = p; g_l[0] = a; g_l[1] = b; g_l[2] = c; }
static unsigned long g_src; static long g_cnt; static unsigned long* g_bufs;
static void __cdecl FakeUnqueue(unsigned long s, long n, unsigned long* b) { g_src = s; g_cnt = n; g_bufs = b; }
static long __cdecl FakeEnum(const char* name) { return (long)std::strlen(name); }
static void* __stdcall FakeGetProcAddress(void* module, const char* name) { ++g_dllLookups; return module == g_module ? Lookup(name) : 0; }
static void* Lookup(const char* name) {
    if (!std::strcmp(name, "alGetProcAddress")) return (void*)&FakeALGetProc;
    if (!std::strcmp(name, "alcProcessContext")) return (void*)&FakeProcessContext;
    if (!std::strcmp(name, "alListener3f")) return (void*)&FakeListener3f;
    if (!std::strcmp(name, "alSourceUnqueueBuffers")) return (void*)&FakeUnqueue;
    if (!std::strcmp(name, "alGetEnumValue")) return (void*)&FakeEnum;
    return 0;
}
typedef void* (__cdecl* PFN_alGetProcAddress)(const char* name);
typedef void (__cdecl* PFN_alcProcessContext)(void* context);
typedef void (__cdecl* PFN_alListener3f)(long param, float v1, float v2, float v3);
typedef void (__cdecl* PFN_alSourceUnqueueBuffers)(unsigned long source, long count, unsigned long* buffers);
typedef long (__cdecl* PFN_alGetEnumValue)(const char* name);
struct CAudioContext {
    void* __vftable; void* Module; unsigned char _pad0[0x18]; PFN_alcProcessContext pfnProcessContext; unsigned char _pad1[0x6c];
    PFN_alSourceUnqueueBuffers pfnSourceUnqueueBuffers; unsigned char _pad2[4]; PFN_alListener3f pfnListener3f; unsigned char _pad3[0x4c];
    PFN_alGetProcAddress pfnGetProcAddress; PFN_alGetEnumValue pfnGetEnumValue; unsigned char _pad4[0x10]; void* Context; bool UseALGetProcAddress;
    unsigned char _pad5[3]; long PendingUpdates; unsigned char _pad6[0xa4]; bool ContextDirty;
    CAudioContext() { std::memset(this, 0, sizeof *this); Module = g_module; }
    void* GetProcAddress(const char* name) {
        if (!pfnGetProcAddress) { pfnGetProcAddress = (PFN_alGetProcAddress)FakeGetProcAddress(Module, "alGetProcAddress"); if (UseALGetProcAddress) pfnGetProcAddress = (PFN_alGetProcAddress)pfnGetProcAddress("alGetProcAddress"); }
        return pfnGetProcAddress(name);
    }
    void ProcessContext() { if (!pfnProcessContext) pfnProcessContext = (PFN_alcProcessContext)FakeGetProcAddress(Module, "alcProcessContext"); pfnProcessContext(Context); ContextDirty = false; PendingUpdates = 0; }
    void Listener3f(long param, float v1, float v2, float v3) { if (!pfnListener3f) pfnListener3f = (PFN_alListener3f)(UseALGetProcAddress ? GetProcAddress("alListener3f") : FakeGetProcAddress(Module, "alListener3f")); pfnListener3f(param, v1, v2, v3); }
    void SourceUnqueueBuffers(unsigned long source, long count, unsigned long* buffers) { if (!pfnSourceUnqueueBuffers) pfnSourceUnqueueBuffers = (PFN_alSourceUnqueueBuffers)(UseALGetProcAddress ? GetProcAddress("alSourceUnqueueBuffers") : FakeGetProcAddress(Module, "alSourceUnqueueBuffers")); pfnSourceUnqueueBuffers(source, count, buffers); }
    long GetEnumValue(const char* name) { if (!pfnGetEnumValue) { pfnGetEnumValue = (PFN_alGetEnumValue)(UseALGetProcAddress ? GetProcAddress("alGetEnumValue") : FakeGetProcAddress(Module, "alGetEnumValue")); if (!pfnGetEnumValue) return 0; } return pfnGetEnumValue(name); }
};
int main()
{
    CAudioContext c; unsigned long bufs[2] = { 7, 8 };
    c.SourceUnqueueBuffers(9, 2, bufs);
    if (g_dllLookups != 1 || g_src != 9 || g_cnt != 2 || g_bufs != bufs || c.pfnSourceUnqueueBuffers != &FakeUnqueue) { std::printf("00c576e0_TEST FAIL\n"); return 1; }
    c.SourceUnqueueBuffers(1, 1, bufs);
    if (g_dllLookups != 1 || g_src != 1) { std::printf("00c576e0_TEST FAIL\n"); return 1; }
    CAudioContext d; d.UseALGetProcAddress = true; d.pfnGetProcAddress = &FakeALGetProc;
    d.SourceUnqueueBuffers(1, 1, bufs);
    if (d.pfnSourceUnqueueBuffers != &FakeUnqueue || g_alLookups != 1 || g_dllLookups != 1 || g_cnt != 1) { std::printf("00c576e0_TEST FAIL\n"); return 1; }
    std::printf("00c576e0_TEST PASS\n");
    return 0;
}