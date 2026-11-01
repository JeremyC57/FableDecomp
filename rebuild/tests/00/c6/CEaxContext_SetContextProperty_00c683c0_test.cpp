// Behaviour test for CEaxContext::SetContextProperty 0x00C683C0 (standalone copy of the model and logic).
#include <cstdio>
#include <cstring>
struct SGuid { unsigned int Data1; unsigned short Data2, Data3; unsigned char Data4[8]; };
struct EAXCONTEXTPROPERTIES { SGuid guidPrimaryFXSlotID; float flDistanceFactor; float flAirAbsorptionHF; float flHFReference; };
struct SEaxEvent { int id; };
static int g_calls[8];
static long g_ret = 0;
static long g_commit = 0;
static float g_lastFloat;
class CEaxContext {
public:
    SEaxEvent* m_pEvents;
    long m_lastError;
    int m_cleared;
    void ClearEvents() { ++m_cleared; }
    long SetPrimaryFXSlotID(SGuid) { ++g_calls[2]; return g_ret; }
    long SetDistanceFactor(float v) { ++g_calls[3]; g_lastFloat = v; return g_ret; }
    long SetAirAbsorptionHF(float v) { ++g_calls[4]; g_lastFloat = v; return g_ret; }
    long SetHFReference(float v) { ++g_calls[5]; g_lastFloat = v; return g_ret; }
    long SetAllParameters(const EAXCONTEXTPROPERTIES*) { ++g_calls[1]; return g_ret; }
    long CommitDeferredSettings() { ++g_calls[7]; return g_commit; }
    long SetContextProperty(unsigned long property, const void* value, unsigned long size, SEaxEvent** ppEvents);
};
long CEaxContext::SetContextProperty(unsigned long property, const void* value, unsigned long size, SEaxEvent** ppEvents)
{
    long result = -3;
    ClearEvents();
    switch (property & 0x7fffffff) {
    case 0: result = 0; break;
    case 1: if (value && size >= sizeof(EAXCONTEXTPROPERTIES)) result = SetAllParameters((const EAXCONTEXTPROPERTIES*)value); break;
    case 2: if (value && size >= sizeof(SGuid)) result = SetPrimaryFXSlotID(*(const SGuid*)value); break;
    case 3: if (value && size >= 4) result = SetDistanceFactor(*(const float*)value); break;
    case 4: if (value && size >= 4) result = SetAirAbsorptionHF(*(const float*)value); break;
    case 5: if (value && size >= 4) result = SetHFReference(*(const float*)value); break;
    case 6:
        if (!value || size < 4) break;
        m_lastError = -1;
    default:
        result = -3;
        break;
    }
    if (result >= 0 && !(property & 0x80000000)) {
        result = CommitDeferredSettings();
        if (result >= 0) *ppEvents = m_pEvents;
    }
    return result;
}
static int fail(int n) { std::printf("00c683c0_TEST FAIL\n"); std::printf("case %d\n", n); return 1; }
int main()
{
    SEaxEvent ev = { 7 };
    CEaxContext c; c.m_pEvents = &ev; c.m_lastError = 0; c.m_cleared = 0;
    SEaxEvent* out = 0;
    float f = 2.5f;
    if (c.SetContextProperty(3, &f, 4, &out) != 0 || out != &ev || g_calls[3] != 1 || g_calls[7] != 1 || g_lastFloat != 2.5f) return fail(1);
    out = 0;
    if (c.SetContextProperty(0x80000004, &f, 4, &out) != 0 || out != 0 || g_calls[4] != 1 || g_calls[7] != 1) return fail(2);
    if (c.SetContextProperty(5, &f, 3, &out) != -3 || g_calls[5] != 0) return fail(3);
    if (c.SetContextProperty(1, 0, 0x1c, &out) != -3 || g_calls[1] != 0) return fail(4);
    EAXCONTEXTPROPERTIES p; std::memset(&p, 0, sizeof p);
    g_ret = -5;
    if (c.SetContextProperty(1, &p, 0x1c, &out) != -5 || g_calls[1] != 1 || g_calls[7] != 1) return fail(5);
    g_ret = 0;
    SGuid gd; std::memset(&gd, 0, sizeof gd);
    if (c.SetContextProperty(2, &gd, 16, &out) != 0 || g_calls[2] != 1 || g_calls[7] != 2) return fail(6);
    long le = 0;
    if (c.SetContextProperty(6, &le, 4, &out) != -3 || c.m_lastError != -1) return fail(7);
    c.m_lastError = 0;
    if (c.SetContextProperty(6, 0, 4, &out) != -3 || c.m_lastError != 0) return fail(8);
    if (c.SetContextProperty(9, &le, 4, &out) != -3) return fail(9);
    out = 0; g_commit = -2;
    if (c.SetContextProperty(0, 0, 0, &out) != -2 || out != 0 || g_calls[7] != 3) return fail(10);
    if (c.m_cleared != 10) return fail(11);
    std::printf("00c683c0_TEST PASS\n");
    return 0;
}