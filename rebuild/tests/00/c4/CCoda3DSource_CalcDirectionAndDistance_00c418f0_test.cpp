// Behaviour test for CCoda3DSource::CalcDirectionAndDistance (0x00C418F0): a zero position gives direction (0, 0, 1) and distance 0
// (no Normalize call); otherwise the direction is a copy of the position normalised in place by CCodaVector::Normalize, the position
// is untouched and the distance is the position's magnitude.
#include <stdio.h>
#include <math.h>
#include <string.h>
// Mirrors of the GameCODA classes with the row's mangled names and the retail offsets (engine/CCoda3DSource.h, measured with VC7.1:
// m_ffStatus +0x10, m_3D +0xFC (vVelocity +0x108, vConeDirection +0x114, vDirection +0x120, fDistance +0x12C), m_fMaxDistance +0x17C,
// m_fMinDistance +0x180, m_fRefDistance +0x184, m_fRolloff +0x188, m_fDoppler +0x18C, m_uiConeInner +0x190, m_uiConeOuter +0x194,
// m_fConeOuterGain +0x198, m_EnvFx.fRolloffFactor +0x1C4, sizeof 0x1DC; CODA: m_fDopplerSpeed +0x98, m_fRolloffFactor +0xA0, sizeof 0xA8).
// The row object is linked: main calls the row's private member through the friend declaration.
class CCodaVector { public: float x, y, z; float Normalize(); };
static int g_normalize_calls; static CCodaVector* g_normalize_this;
float CCodaVector::Normalize()            // 0x00C402E0 stand-in: normalise in place, return the old magnitude
{
    ++g_normalize_calls; g_normalize_this = this;
    float m = (float)sqrt(x * x + y * y + z * z);
    if (m != 0.0f) { x /= m; y /= m; z /= m; }
    return m;
}
class CCodaContext { public: unsigned char raw[0xA8]; };
CCodaContext CODA;
#define CODA_F(off) (*(float*)(CODA.raw + (off)))
#define SRC_F(s, off) (*(float*)((s).raw + (off)))
#define SRC_U(s, off) (*(unsigned int*)((s).raw + (off)))
#define SRC_V(s, off) (*(CCodaVector*)((s).raw + (off)))
static int g_bad;
static bool Near(double a, double b) { double d = a - b; if (d < 0) d = -d; double m = b < 0 ? -b : b; return d <= 1e-5 * (m > 1 ? m : 1); }
static void Expect(int n, const char* what, double got, double want)
{
    if (!Near(got, want)) { printf("  case %d %s: %.9g, expected %.9g\n", n, what, got, want); ++g_bad; }
}
class CCoda3DSource { friend int main(); unsigned char raw[0x1DC];
    float CalcDirectionAndDistance(CCodaVector& vDirection, const CCodaVector& cvPosition); };
int main()
{
    static CCoda3DSource s; memset(&s, 0, sizeof(s));
    CCodaVector out = { 5.0f, 5.0f, 5.0f }, in = { 0.0f, 0.0f, 0.0f };
    float d = s.CalcDirectionAndDistance(out, in);
    Expect(1, "dist", d, 0.0); Expect(1, "x", out.x, 0.0); Expect(1, "y", out.y, 0.0); Expect(1, "z", out.z, 1.0); Expect(1, "calls", g_normalize_calls, 0);
    CCodaVector in2 = { 3.0f, 0.0f, 4.0f };
    d = s.CalcDirectionAndDistance(out, in2);
    Expect(2, "dist", d, 5.0); Expect(2, "x", out.x, 0.6); Expect(2, "y", out.y, 0.0); Expect(2, "z", out.z, 0.8);
    Expect(2, "in.x", in2.x, 3.0); Expect(2, "in.z", in2.z, 4.0); Expect(2, "calls", g_normalize_calls, 1); Expect(2, "this", g_normalize_this == &out, 1);
    CCodaVector in3 = { 0.0f, -2.0f, 0.0f };
    d = s.CalcDirectionAndDistance(out, in3);
    Expect(3, "dist", d, 2.0); Expect(3, "x", out.x, 0.0); Expect(3, "y", out.y, -1.0); Expect(3, "z", out.z, 0.0);
    CCodaVector in4 = { 0.0f, 0.0f, 2.0f };                      // only z non-zero: not the zero vector
    d = s.CalcDirectionAndDistance(out, in4);
    Expect(4, "dist", d, 2.0); Expect(4, "z", out.z, 1.0); Expect(4, "calls", g_normalize_calls, 3);
    CCodaVector in5 = { 1.0f, 2.0f, 2.0f };
    d = s.CalcDirectionAndDistance(out, in5);
    Expect(5, "dist", d, 3.0); Expect(5, "x", out.x, 1.0 / 3); Expect(5, "y", out.y, 2.0 / 3); Expect(5, "z", out.z, 2.0 / 3);
    printf(g_bad ? "00c418f0_TEST FAIL\n" : "00c418f0_TEST PASS\n");
    return g_bad ? 1 : 0;
}