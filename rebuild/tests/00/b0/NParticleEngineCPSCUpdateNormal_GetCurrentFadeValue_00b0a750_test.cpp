// Behaviour test for 0x00B0A750 NParticleEngine::CPSCUpdateNormal::GetCurrentFadeValue (links the REAL row object, NF261 q12742; the HEAD
// test carried its own copy of the body with a made-up table scale of 2048 where retail multiplies by the pooled 0.5f and 1024.0f). The
// layout class is the HEAD test's local model (in place); the row's data and its one callee are defined here.
#include <cstdio>
#include <math.h>
float GFastCosTable[1025];                       // the row's ?GFastCosTable@@3PAMA (0x013CD550): cos(2 pi k / 1024)
extern const float g_fadeScale = 0.001f;         // the row's ?g_fadeScale@@3MB (0x01266300 holds 0.001f)
static float g_clock = 1.0f;
namespace NParticleEngine {
float __fastcall GetOneOverTickRate() { return g_clock; }   // 0x00AEAA00 (stub)
class CPSCUpdateNormal { public:
    unsigned char _pad00[0x0c]; unsigned long FadeBits; unsigned char _pad10[0x08]; float Lifetime;
    unsigned char _pad1c[0x98 - 0x1c]; long Frame;
    protected: float GetCurrentFadeValue() const; public: float TestFade() const { return GetCurrentFadeValue(); }   // the row (PDB IBE) + a test-only accessor
};
}
static double Expected(double t)                 // 0.5 - cos(2 pi * (t * 0.5 * 1024) / 1024) / 2, the table read interpolated
{
    double scaled = t * 0.5 * 1024.0;
    return 0.5 - 0.5 * cos(scaled * 3.14159265358979 * 2.0 / 1024.0);
}
int main()
{
    for (int i = 0; i <= 1024; ++i) GFastCosTable[i] = (float)cos(i * 3.14159265358979 * 2.0 / 1024.0);
    NParticleEngine::CPSCUpdateNormal u; u.Lifetime = 10.0f; u.Frame = 5; u.FadeBits = 0;
    bool ok = u.TestFade() == 1.0f;                                 // fade disabled
    u.FadeBits = 0x2000000 | (1023 << 10) | 511;                               // fade in over 0.511 of the life, fade out from 1.023 (never)
    float v = u.TestFade();                                         // age 0.5 < fadeIn: eased in
    ok = ok && fabs(v - Expected(0.5 / 0.511)) < 0.002;
    u.Frame = 1; v = u.TestFade();                                  // age 0.1: 0.0915 (a 2048 or 512 scale gives 0.33 / 0.024)
    ok = ok && fabs(v - Expected(0.1 / 0.511)) < 0.002 && v > 0.08f && v < 0.10f;
    u.Frame = 0; v = u.TestFade(); ok = ok && fabs(v) < 0.01f;      // age 0 -> 0
    u.FadeBits = 0x2000000 | (300 << 10) | 0;                                  // no fade in; fade out from 0.3
    u.Frame = 8; v = u.TestFade();                                  // age 0.8: TableCos01((1 - 0.8) / 0.7)
    ok = ok && fabs(v - Expected(0.2 / 0.7)) < 0.002;
    std::printf(ok ? "00b0a750_TEST PASS\n" : "00b0a750_TEST FAIL\n"); return ok ? 0 : 1;
}
