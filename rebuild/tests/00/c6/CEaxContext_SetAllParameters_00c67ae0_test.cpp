// Behaviour test for 0x00C67AE0 CEaxContext::SetAllParameters: reject air absorption outside -100..0, HF reference outside
// 1000..20000 or a primary FX slot that is not FXSlot0..3 / null (last error -2, result -3); else apply the four setters
// in order, stopping at the first negative result.
#include <cstdio>
#include <cstring>

struct SGuid { unsigned int d1; unsigned short d2, d3; unsigned char d4[8]; };
static bool operator==(const SGuid& a, const SGuid& b) { return std::memcmp(&a, &b, sizeof(a)) == 0; }
static const SGuid FXSlot[4] = { { 0xC4D79F1E, 0xF1AC, 0x436B, { 0xA8, 0x1D, 0xA7, 0x38, 0xE7, 0x04, 0x54, 0x69 } },
                                 { 0x08C00E96, 0x74BE, 0x4491, { 0x93, 0xAA, 0xE8, 0xAD, 0x35, 0xA4, 0x91, 0x17 } },
                                 { 0x1D433B88, 0xF0F6, 0x4637, { 0x91, 0x9F, 0x60, 0xE7, 0xE0, 0x6B, 0x5E, 0xDD } },
                                 { 0xEFFF08EA, 0xC7D8, 0x44AB, { 0x93, 0xAD, 0x6D, 0xBD, 0x5F, 0x91, 0x00, 0x64 } } };
static const SGuid NullGuid = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };
struct Props { SGuid guidPrimaryFXSlotID; float flDistanceFactor, flAirAbsorptionHF, flHFReference; };

struct Ctx {
    long m_lastError; int calls; long failAt;
    long Step(int n) { ++calls; return n == failAt ? -5 : 0; }
    long SetAllParameters(const Props* v)
    {
        bool slotOk = v->guidPrimaryFXSlotID == NullGuid;
        for (int i = 0; i < 4; ++i) if (v->guidPrimaryFXSlotID == FXSlot[i]) slotOk = true;
        if (v->flAirAbsorptionHF < -100.0f || v->flAirAbsorptionHF > 0.0f || v->flHFReference < 1000.0f
            || v->flHFReference > 20000.0f || !slotOk) { m_lastError = -2; return -3; }
        for (int n = 1; n <= 3; ++n) { long r = Step(n); if (r < 0) return r; }
        return Step(4);
    }
};

static int fail() { std::printf("00c67ae0_TEST FAIL\n"); return 1; }

int main()
{
    Props p; p.guidPrimaryFXSlotID = FXSlot[2]; p.flDistanceFactor = 1.0f; p.flAirAbsorptionHF = -5.0f; p.flHFReference = 5000.0f;
    Ctx c; c.m_lastError = 0; c.calls = 0; c.failAt = 0;
    if (c.SetAllParameters(&p) != 0 || c.calls != 4 || c.m_lastError != 0) return fail();
    c.calls = 0; c.failAt = 2;
    if (c.SetAllParameters(&p) != -5 || c.calls != 2) return fail();
    c.calls = 0; c.failAt = 0; p.flAirAbsorptionHF = 0.5f;
    if (c.SetAllParameters(&p) != -3 || c.m_lastError != -2 || c.calls != 0) return fail();
    p.flAirAbsorptionHF = -100.0f; p.flHFReference = 20001.0f; c.m_lastError = 0;
    if (c.SetAllParameters(&p) != -3 || c.m_lastError != -2) return fail();
    p.flHFReference = 1000.0f; p.guidPrimaryFXSlotID = NullGuid;
    if (c.SetAllParameters(&p) != 0 || c.calls != 4) return fail();
    p.guidPrimaryFXSlotID.d1 = 7; c.calls = 0;
    if (c.SetAllParameters(&p) != -3 || c.calls != 0) return fail();
    std::printf("00c67ae0_TEST PASS\n");
    return 0;
}