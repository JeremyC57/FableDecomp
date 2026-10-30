// Behaviour test for CAudioContext::InitialiseListener (0x00C568F0): standalone copy of the model, OpenAL/EAX entry points faked.
#include <cstdio>
#include <cstring>
struct GUID { unsigned int Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; };
static const GUID DSPROPSETID_EAX30_ListenerProperties = { 0xa8fa6882, 0xb476, 0x11d3, { 0xbd, 0xb9, 0x00, 0xc0, 0xf0, 0x2d, 0xdf, 0x87 } };
typedef void (*PFN_alDistanceModel)(long value);
typedef void (*PFN_alListener3f)(long param, float v1, float v2, float v3);
typedef long (*PFN_EAXSet)(const GUID* propertySetGuid, unsigned int property, unsigned int source, void* value, unsigned int size);
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed at line %d\n", __LINE__); } } while (0)
static long g_model = 99; static int g_dm = 0, g_l3 = 0, g_eax = 0;
static long g_params[8]; static float g_vals[24];
static unsigned int g_props[8]; static int g_eaxInt[8]; static float g_eaxFloat[8]; static const GUID* g_guid = 0;
static void fake_alDistanceModel(long v) { g_dm++; g_model = v; }
static void fake_alListener3f(long p, float a, float b, float c) { g_params[g_l3] = p; g_vals[3 * g_l3] = a; g_vals[3 * g_l3 + 1] = b; g_vals[3 * g_l3 + 2] = c; g_l3++; }
static long fake_EAXSet(const GUID* g, unsigned int prop, unsigned int src, void* v, unsigned int size)
{ g_guid = g; g_props[g_eax] = prop; if (src != 0 || size != 4) g_fail = 1; std::memcpy(&g_eaxInt[g_eax], v, 4); std::memcpy(&g_eaxFloat[g_eax], v, 4); g_eax++; return 0; }
static int g_dllLookups = 0, g_alLookups = 0;
static void* lookup(const char* name)
{
    if (!std::strcmp(name, "alDistanceModel")) return (void*)fake_alDistanceModel;
    if (!std::strcmp(name, "alListener3f")) return (void*)fake_alListener3f;
    if (!std::strcmp(name, "EAXSet")) return (void*)fake_EAXSet;
    return 0;
}
static void* GetProcAddress(void* module, const char* name) { if (module != (void*)0x1234) return 0; g_dllLookups++; return lookup(name); }
class CAudioContext {
public:
    void* Module;
    PFN_alListener3f pfnListener3f;
    PFN_alDistanceModel pfnDistanceModel;
    PFN_EAXSet pfnEAXSet;
    bool UseALGetProcAddress;
    CAudioContext() : Module((void*)0x1234), pfnListener3f(0), pfnDistanceModel(0), pfnEAXSet(0), UseALGetProcAddress(false) {}
    void* GetProcAddress(const char* name) { g_alLookups++; return lookup(name); }
    void DistanceModel(long value)
    {
        if (!pfnDistanceModel)
            pfnDistanceModel = (PFN_alDistanceModel)(UseALGetProcAddress ? GetProcAddress("alDistanceModel") : ::GetProcAddress(Module, "alDistanceModel"));
        pfnDistanceModel(value);
    }
    void Listener3f(long param, float v1, float v2, float v3)
    {
        if (!pfnListener3f)
            pfnListener3f = (PFN_alListener3f)(UseALGetProcAddress ? GetProcAddress("alListener3f") : ::GetProcAddress(Module, "alListener3f"));
        pfnListener3f(param, v1, v2, v3);
    }
    long EAXSet(const GUID* g, unsigned int property, unsigned int source, void* value, unsigned int size)
    {
        if (!pfnEAXSet)
            pfnEAXSet = (PFN_EAXSet)GetProcAddress("EAXSet");
        return pfnEAXSet(g, property, source, value, size);
    }
    void InitialiseListener()
    {
        DistanceModel(0);
        Listener3f(0x1004, 0.0f, 0.0f, 0.0f);
        Listener3f(0x1006, 0.0f, 0.0f, 0.0f);
        int room = -10000;
        EAXSet(&DSPROPSETID_EAX30_ListenerProperties, 5 | 0x80000000u, 0, &room, sizeof(room));
        float roomRolloffFactor = 0.0f;
        EAXSet(&DSPROPSETID_EAX30_ListenerProperties, 24 | 0x80000000u, 0, &roomRolloffFactor, sizeof(roomRolloffFactor));
    }
};
int main()
{
    CAudioContext c;
    c.InitialiseListener();
    CHECK(g_dm == 1 && g_model == 0);
    CHECK(g_l3 == 2 && g_params[0] == 0x1004 && g_params[1] == 0x1006);
    for (int i = 0; i < 6; i++) CHECK(g_vals[i] == 0.0f);
    CHECK(g_eax == 2 && g_guid == &DSPROPSETID_EAX30_ListenerProperties);
    CHECK(g_props[0] == 0x80000005u && g_eaxInt[0] == -10000);
    CHECK(g_props[1] == 0x80000018u && g_eaxFloat[1] == 0.0f);
    // DLL export route resolves alDistanceModel and alListener3f once each; EAXSet always goes through alGetProcAddress
    CHECK(g_dllLookups == 2 && g_alLookups == 1);
    // second call: everything cached
    c.InitialiseListener();
    CHECK(g_dllLookups == 2 && g_alLookups == 1 && g_dm == 2 && g_l3 == 4 && g_eax == 4);
    // alGetProcAddress routing
    CAudioContext d; d.UseALGetProcAddress = true; d.InitialiseListener();
    CHECK(g_dllLookups == 2 && g_alLookups == 4 && g_dm == 3);
    if (g_fail) { std::printf("00c568f0_TEST FAIL\n"); return 1; }
    std::printf("00c568f0_TEST PASS\n");
    return 0;
}