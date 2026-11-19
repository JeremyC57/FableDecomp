// Behaviour test for CCoda3DSource::CalcDoppler (0x00C41A20): 1 - (velocity . direction) * the source's Doppler factor * CODA's
// Doppler speed, clamped at zero from below.
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
class CCoda3DSource { friend int main(); unsigned char raw[0x1DC]; float CalcDoppler() const; };
int main()
{
    static CCoda3DSource s; memset(&s, 0, sizeof(s));
    CCodaVector v = { 1.0f, 2.0f, 3.0f }, d = { 0.5f, 0.25f, 0.125f };
    SRC_V(s, 0x108) = v; SRC_V(s, 0x120) = d; SRC_F(s, 0x18C) = 0.25f; CODA_F(0x98) = 0.5f;
    Expect(1, "approach", s.CalcDoppler(), 1.0 - (0.5 + 0.5 + 0.375) * 0.25 * 0.5);
    CCodaVector vn = { -1.0f, -2.0f, -3.0f }; SRC_V(s, 0x108) = vn;
    Expect(2, "recede (> 1)", s.CalcDoppler(), 1.0 + 1.375 * 0.125);
    SRC_F(s, 0x18C) = 4.0f; SRC_V(s, 0x108) = v;
    Expect(3, "clamped", s.CalcDoppler(), 0.0);
    SRC_F(s, 0x18C) = 1.0f; CODA_F(0x98) = 1.0f / 1.375f; CCodaVector dz = { 0.0f, 0.0f, 1.0f / 3.0f }; SRC_V(s, 0x120) = dz;
    Expect(4, "zero", s.CalcDoppler(), 1.0 - 1.0 / 1.375);
    CCodaVector dx = { 1.0f, 0.0f, 0.0f }, vy = { 0.0f, 7.0f, 0.0f }; SRC_V(s, 0x120) = dx; SRC_V(s, 0x108) = vy;
    Expect(5, "perpendicular", s.CalcDoppler(), 1.0);
    SRC_V(s, 0x108) = v; SRC_V(s, 0x120) = d; SRC_F(s, 0x18C) = 2.0f; CODA_F(0x98) = 0.5f;
    Expect(6, "clamped from -0.375", s.CalcDoppler(), 0.0);
    printf(g_bad ? "00c41a20_TEST FAIL\n" : "00c41a20_TEST PASS\n");
    return g_bad ? 1 : 0;
}