// Behaviour test for CCoda3DSource::CalcEnvFxGain (0x00C41BB0): 0 when the max-distance-muted status bit (0x80000) is set and the
// distance has reached the maximum; otherwise powf(ref / clamp(distance, min, max), fRolloff * the source's EnvFx rolloff factor).
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
class CCoda3DSource { friend int main(); unsigned char raw[0x1DC]; float CalcEnvFxGain(float fRolloff) const; };
static double Want(double ref, double d, double mn, double mx, double e) { double c = d < mn ? mn : (d > mx ? mx : d); return pow(ref / c, e); }
int main()
{
    static CCoda3DSource s; memset(&s, 0, sizeof(s));
    SRC_F(s, 0x17C) = 10.0f; SRC_F(s, 0x180) = 2.0f; SRC_F(s, 0x184) = 8.0f; SRC_F(s, 0x1C4) = 0.5f;
    SRC_F(s, 0x188) = 7.0f; CODA_F(0xA0) = 9.0f;                     // the distance rolloffs are not used here
    SRC_U(s, 0x10) = 0x80000; SRC_F(s, 0x12C) = 10.0f;
    Expect(1, "muted at max", s.CalcEnvFxGain(2.0f), 0.0);
    SRC_F(s, 0x12C) = 4.0f; Expect(2, "muted below max", s.CalcEnvFxGain(2.0f), 2.0);
    SRC_U(s, 0x10) = 0x7FFFF;
    SRC_F(s, 0x12C) = 10.0f; Expect(3, "at max", s.CalcEnvFxGain(2.0f), 0.8);
    SRC_F(s, 0x12C) = 1.0f; Expect(4, "below min", s.CalcEnvFxGain(2.0f), 4.0);
    SRC_F(s, 0x12C) = 20.0f; Expect(5, "above max", s.CalcEnvFxGain(2.0f), 0.8);
    SRC_F(s, 0x12C) = 4.0f; Expect(6, "exponent 2", s.CalcEnvFxGain(4.0f), 4.0);
    SRC_F(s, 0x1C4) = 1.5f; SRC_F(s, 0x12C) = 5.0f; Expect(7, "exponent 1.5", s.CalcEnvFxGain(1.0f), Want(8, 5, 2, 10, 1.5));
    printf(g_bad ? "00c41bb0_TEST FAIL\n" : "00c41bb0_TEST PASS\n");
    return g_bad ? 1 : 0;
}