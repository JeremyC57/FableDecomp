// Behaviour test for CCoda3DSource::CalcConeGain (0x00C41A70): the cone angle 360 - 2 * (angle between the cone direction and the
// direction, in degrees), from atan2 of |cone - dir| and |cone + dir|; 1 at or inside the inner angle, the outer gain at or beyond
// the outer angle, linear in between (the unsigned angles converted to float).
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
class CCoda3DSource { friend int main(); public: unsigned char raw[0x1DC]; private: float CalcConeGain() const; };
static double Gain(double theta_deg, double inner, double outer, double g)
{
    double a = 360.0 - 2.0 * theta_deg;
    if (a <= inner) return 1.0;
    if (a >= outer) return g;
    return (g - 1.0) * (a - inner) / (outer - inner) + 1.0;
}
static void Set(CCoda3DSource& s, double theta_deg)
{
    double t = theta_deg * 3.14159265358979323846 / 180.0;
    CCodaVector c = { 1.0f, 0.0f, 0.0f }, d = { (float)cos(t), (float)sin(t), 0.0f };
    SRC_V(s, 0x114) = c; SRC_V(s, 0x120) = d;
}
int main()
{
    static CCoda3DSource s; memset(&s, 0, sizeof(s));
    SRC_U(s, 0x190) = 120; SRC_U(s, 0x194) = 240; SRC_F(s, 0x198) = 0.25f;
    Set(s, 180.0); Expect(1, "behind (0)", s.CalcConeGain(), 1.0);
    Set(s, 0.0); Expect(2, "aligned (360)", s.CalcConeGain(), 0.25);
    Set(s, 90.0); Expect(3, "90 deg (180)", s.CalcConeGain(), Gain(90.0, 120, 240, 0.25));
    Set(s, 110.0); Expect(4, "110 deg (140)", s.CalcConeGain(), Gain(110.0, 120, 240, 0.25));
    Set(s, 70.0); Expect(5, "70 deg (220)", s.CalcConeGain(), Gain(70.0, 120, 240, 0.25));
    Set(s, 150.0); Expect(6, "inside inner (60)", s.CalcConeGain(), 1.0);
    Set(s, 30.0); Expect(7, "beyond outer (300)", s.CalcConeGain(), 0.25);
    Set(s, 120.25); Expect(10, "just inside inner (119.5)", s.CalcConeGain(), 1.0);
    Set(s, 59.75); Expect(11, "just beyond outer (240.5)", s.CalcConeGain(), 0.25);
    SRC_U(s, 0x190) = 0; SRC_U(s, 0x194) = 360; SRC_F(s, 0x198) = 0.5f;
    Set(s, 45.0); Expect(8, "45 deg (270), 0..360", s.CalcConeGain(), Gain(45.0, 0, 360, 0.5));
    SRC_U(s, 0x190) = 0x80000000u; SRC_U(s, 0x194) = 0xFFFFFFFFu;    // unsigned conversion: both far above 360 -> inside inner
    Set(s, 10.0); Expect(9, "unsigned limits", s.CalcConeGain(), 1.0);
    printf(g_bad ? "00c41a70_TEST FAIL\n" : "00c41a70_TEST PASS\n");
    return g_bad ? 1 : 0;
}