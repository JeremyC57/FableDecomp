// Behaviour test for row 0x00A229E0 CRightHandedSet::CRightHandedSet(const C3DAngles&) (links the row's object). It supplies the
// fast inverse-square-root mantissa table the row reads (GFastOneOverSqrtTable, 128 entries, built here from 1/sqrt at each bucket
// centre) and checks Up / Forward against an independent double model: Forward from the XY / YZ angles (in turns), Up = the
// vector perpendicular to Forward in the vertical plane rotated about Forward by the XZ angle (Rodrigues), both unit length. Angle
// sets with every angle non-zero and of different signs make any sign / term mutant in the nine rotation entries, the up vector or
// a swapped sin / cos visible; a zero XZ angle checks the unrotated up vector.
#include <stdio.h>
#include <math.h>

class C3DVector { public: float X, Y, Z; };
class C3DAngles { public: float AngleXY, AngleYZ, AngleXZ; };
class CRightHandedSet {
public:
    C3DVector Up, Forward;
    CRightHandedSet(const C3DAngles& angles);
};

unsigned long GFastOneOverSqrtTable[128];

static void build_table()
{
    for (int i = 0; i < 128; ++i) {
        int e = (i >> 6) ? 127 : 126;                       // exponent field parity bit
        double f = ((i & 63) + 0.5) / 64.0;
        double v = ldexp(1.0 + f, e - 127);
        double r = 1.0 / sqrt(v);
        int ee = (0x17c - e) >> 1;
        double t = (r / ldexp(1.0, ee - 127) - 1.0) * 256.0;
        long ti = (long)(t + 0.5);
        if (ti < 0) ti = 0;
        if (ti > 255) ti = 255;
        GFastOneOverSqrtTable[i] = (unsigned long)ti;
    }
}

static int g_fail = 0;
static void check(const char* what, double got, double want)
{
    if (fabs(got - want) > 2e-3) { printf("FAIL %s: got %f want %f\n", what, got, want); g_fail = 1; }
}

static void model_and_check(float xy, float yz, float xz)
{
    const double turn = 6.283185307179586;
    double sxy = sin(xy * turn), cxy = cos(xy * turn), syz = sin(yz * turn), cyz = cos(yz * turn);
    double s = sin(xz * turn), c = cos(xz * turn);
    double f[3] = { sxy * cyz, cxy * cyz, syz };
    double u0[3] = { -sxy * syz, -cxy * syz, cyz };
    // Rodrigues rotation of u0 about the unit axis f by angle xz: u0*c + (f x u0)*s + f*(f.u0)*(1-c)
    double fxu[3] = { f[1] * u0[2] - f[2] * u0[1], f[2] * u0[0] - f[0] * u0[2], f[0] * u0[1] - f[1] * u0[0] };
    double fd = f[0] * u0[0] + f[1] * u0[1] + f[2] * u0[2];
    double u[3];
    for (int k = 0; k < 3; ++k) u[k] = u0[k] * c - fxu[k] * s + f[k] * fd * (1.0 - c);
    C3DAngles a; a.AngleXY = xy; a.AngleYZ = yz; a.AngleXZ = xz;
    CRightHandedSet set(a);
    check("forward x", set.Forward.X, f[0]); check("forward y", set.Forward.Y, f[1]); check("forward z", set.Forward.Z, f[2]);
    check("up x", set.Up.X, u[0]); check("up y", set.Up.Y, u[1]); check("up z", set.Up.Z, u[2]);
}

int main()
{
    build_table();
    model_and_check(0.1f, 0.07f, 0.0f);
    model_and_check(0.1f, 0.07f, 0.13f);
    model_and_check(-0.31f, 0.21f, -0.09f);
    model_and_check(0.42f, -0.18f, 0.27f);
    model_and_check(0.77f, 0.03f, 0.61f);
    if (!g_fail) printf("00a229e0_TEST PASS\n");
    return g_fail;
}