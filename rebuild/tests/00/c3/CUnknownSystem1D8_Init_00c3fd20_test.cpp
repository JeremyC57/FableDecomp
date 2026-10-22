#include <cstdio>
#include <cstring>
// behaviour test: CUnknownSystem1D8::Init -- copy or default the parameters, default the window to the foreground
// window, fail with -24 without a performance counter, derive the ticks per 44.1 kHz sample, create the device
// (dropping it when its init fails), wipe each sub-system's globals and clear its flag bit when it is disabled or
// fails, and return 0 when anything came up, else -9
#ifdef _MSC_VER
typedef __int64 Int64;
#else
typedef long long Int64;
#endif
struct SSystemInitParams { int Field00, Field04, Field08, DisableSubSystemA, DeviceParam, Window, Field18; };
static int g_frequencyOk = 1, g_foreground = 0x55, g_deviceOk = 1, g_aOk = 1, g_bOk = 1, g_devicesAlive = 0, g_seenWindow = 0;
static Int64 g_frequency = 7159090;
static int g_slotA0[6], g_slotA1[6], g_slotB0[6], g_slotB1[6], g_extraB[5], g_a0c, g_a10, g_a1c, g_b14, g_b18;
static unsigned int g_flags;
static void GetDefaultSystemInitParams(SSystemInitParams* p) { std::memset(p, 0, sizeof(*p)); p->DeviceParam = 7; }
struct CSystemDevice {
    int Field04, Field08;
    CSystemDevice() : Field04((int)0x80000000), Field08(0) { ++g_devicesAlive; }
    ~CSystemDevice() { --g_devicesAlive; }
    bool Init(int param, int window, int) { g_seenWindow = window; return g_deviceOk && param == 7; }
};
static void Dirty()
{
    std::memset(g_slotA0, 1, sizeof(g_slotA0)); std::memset(g_slotA1, 1, sizeof(g_slotA1)); std::memset(g_slotB0, 1, sizeof(g_slotB0));
    std::memset(g_slotB1, 1, sizeof(g_slotB1)); std::memset(g_extraB, 1, sizeof(g_extraB));
    g_a0c = g_a10 = g_a1c = g_b14 = g_b18 = 1; g_flags = 0x30;
}
struct CUnknownSystem1D8 {
    int Field17C, Field180, Field1D4; CSystemDevice* Device; Int64 StartCounter, LastCounter, TicksPerSample;
    bool InitSubSystemA(void*, SSystemInitParams*) { return g_aOk != 0; }
    bool InitSubSystemB(void*, SSystemInitParams*) { return g_bOk != 0; }
    int Init(void* owner, const SSystemInitParams* params);
};
int CUnknownSystem1D8::Init(void* owner, const SSystemInitParams* params)
{
    SSystemInitParams p;
    if (params == 0) GetDefaultSystemInitParams(&p); else p = *params;
    if (p.Window == 0 || p.Window == -1) p.Window = g_foreground;
    Field17C = 0;
    if (!g_frequencyOk) return -24;
    TicksPerSample = g_frequency / 44100;
    StartCounter = 1234; LastCounter = StartCounter;
    Device = new CSystemDevice;
    if (Device && !Device->Init(p.DeviceParam, p.Window, 0)) { delete Device; Device = 0; }
    bool ok = false;
    if (!p.DisableSubSystemA) ok = InitSubSystemA(owner, &p);
    if (p.DisableSubSystemA || !ok) {
        g_a0c = 0; g_a10 = 0; g_flags &= ~0x10u;
        std::memset(g_slotA0, 0, sizeof(g_slotA0)); std::memset(g_slotA1, 0, sizeof(g_slotA1));
        g_a1c = 0;
    }
    ok = InitSubSystemB(owner, &p);
    if (!ok) {
        g_flags &= ~0x20u; g_b14 = 0; g_b18 = 0;
        std::memset(g_slotB0, 0, sizeof(g_slotB0)); std::memset(g_slotB1, 0, sizeof(g_slotB1)); std::memset(g_extraB, 0, sizeof(g_extraB));
    }
    if (Field1D4 != 0 || Field180 != 0) ok = true;
    return ok ? 0 : -9;
}
int main()
{
    CUnknownSystem1D8 s; s.Field180 = 0; s.Field1D4 = 0; s.Field17C = 9; s.Device = 0;
    Dirty();
    bool ok = s.Init(0, 0) == 0 && s.Field17C == 0 && s.TicksPerSample == 162 && s.LastCounter == 1234 && s.Device != 0
           && g_seenWindow == 0x55 && g_flags == 0x30 && g_slotA0[0] != 0 && g_extraB[4] != 0;
    delete s.Device;
    SSystemInitParams p; std::memset(&p, 0, sizeof(p)); p.DisableSubSystemA = 1; p.DeviceParam = 7; p.Window = 0x99;
    Dirty(); g_bOk = 0;
    int r = s.Init(0, &p);                              // A disabled, B fails: both wiped, -9
    ok = ok && r == -9 && g_flags == 0 && g_slotA0[5] == 0 && g_slotA1[0] == 0 && g_slotB1[5] == 0 && g_extraB[4] == 0
            && g_a1c == 0 && g_b18 == 0 && g_seenWindow == 0x99;
    delete s.Device;
    Dirty(); s.Field180 = 1; g_deviceOk = 0;
    r = s.Init(0, &p);                                  // still B failing, but the owner already has +0x180: success
    ok = ok && r == 0 && s.Device == 0 && g_devicesAlive == 0;
    g_frequencyOk = 0;
    ok = ok && s.Init(0, 0) == -24;
    if (!ok) { std::printf("00c3fd20_TEST FAIL\n"); return 1; }
    std::printf("00c3fd20_TEST PASS\n");
    return 0;
}