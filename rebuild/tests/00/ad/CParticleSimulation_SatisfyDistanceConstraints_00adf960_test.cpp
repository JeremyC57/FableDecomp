// Behaviour test for 0x00ADF960 CParticleSimulation::SatisfyDistanceConstraints (links the REAL row object: the shared SSE kernel
// GF_SSE_SATISFY_DISTANCE_CONSTRAINTS and the x87 loop with its expanded C3DVector / C4DVector helpers run as compiled).
// Fixtures (raw native layout, every offset read by an instruction of the row): CParticleSimulation Positions +0x1C (0x00ADFAE3),
// SimulationAlphas +0x2C (0x00ADFB17), MaximumAdjustment +0x30 (0x00ADFBF2); CParticleDistanceConstraint Index1 +0, Index2 +4,
// Distance +8, Strength +0xC (16 bytes); C4DVector 16 bytes, 16-byte aligned (the kernel uses movaps); GEnableSIMD (bool) picks the
// path. No out-of-line callee.
// Reference model (independent, double precision): per constraint, in list order -- p1 / p2 = the two positions, v = p2 - p1,
// dist = |v|, divisor = max((alpha1 + alpha2) * dist, 1e-4f), stretch = dist - Distance, factor = stretch / divisor * Strength,
// p1 += v * (alpha1 * factor), p2 -= v * (alpha2 * factor), MaximumAdjustment = max(MaximumAdjustment, stretch), both W = 0;
// positions no constraint names are untouched (W kept). Checks: (1) exact results on dyadic 3-4-5 cases (both paths, bit for bit);
// (2) 4000 seeded random lists per path against the model within 2e-5 relative; (3) the constraint list, the alphas and the bytes
// around the simulation are not written.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

class CParticleDistanceConstraint { public: unsigned long Index1, Index2; float Distance, Strength; };
class CParticleSimulation {
    void SatisfyDistanceConstraints(const CParticleDistanceConstraint* list, unsigned long size);
public:
    static void Call(CParticleSimulation* s, const CParticleDistanceConstraint* list, unsigned long size) { s->SatisfyDistanceConstraints(list, size); }
};
bool GEnableSIMD;

struct V4 { float X, Y, Z, W; };
struct Sim { unsigned char b[0x40]; };
template<class T> static T& At(void* base, int off) { return *(T*)((unsigned char*)base + off); }

enum { NP = 12, NC = 16 };
static __declspec(align(16)) V4 g_pos[NP + 2];
static float g_alpha[NP + 2];
static CParticleDistanceConstraint g_list[NC + 2];
static __declspec(align(16)) Sim g_sim;
static int g_fail, g_cases;

static unsigned long g_seed = 12345;
static unsigned long Rnd() { g_seed = g_seed * 1664525u + 1013904223u; return g_seed >> 8; }
static float RndF(float lo, float hi) { return lo + (hi - lo) * (float)(Rnd() & 0xFFFF) / 65536.0f; }

struct MV { double X, Y, Z, W; };
static double Model(MV* p, const float* alpha, const CParticleDistanceConstraint* list, unsigned long size, double maxadj)
{
    for (unsigned long c = 0; c < size; ++c)
    {
        const CParticleDistanceConstraint& k = list[c];
        MV p1 = p[k.Index1], p2 = p[k.Index2];
        double a1 = alpha[k.Index1], a2 = alpha[k.Index2];
        double vx = p2.X - p1.X, vy = p2.Y - p1.Y, vz = p2.Z - p1.Z;
        double dist = sqrt(vx * vx + vy * vy + vz * vz);
        double divisor = (a1 + a2) * dist;
        if (divisor < (double)1e-4f) divisor = (double)1e-4f;
        double stretch = dist - k.Distance;
        double factor = stretch / divisor * k.Strength;
        p1.X += vx * (a1 * factor); p1.Y += vy * (a1 * factor); p1.Z += vz * (a1 * factor);
        p2.X -= vx * (a2 * factor); p2.Y -= vy * (a2 * factor); p2.Z -= vz * (a2 * factor);
        if (!(maxadj > stretch)) maxadj = stretch;
        p1.W = 0; p2.W = 0;
        p[k.Index1] = p1; p[k.Index2] = p2;
    }
    return maxadj;
}
static bool Near(double got, double want, double tol)
{
    double m = fabs(want) > 1.0 ? fabs(want) : 1.0;
    return fabs(got - want) <= tol * m;
}
static void Fail(const char* what, int i, double got, double want)
{
    if (g_fail++ < 12) printf("FAIL case %d (simd %d): %s[%d] got %.9g want %.9g\n", g_cases, (int)GEnableSIMD, what, i, got, want);
}
static void Run(unsigned long size, float maxadj, double tol)
{
    ++g_cases;
    MV m[NP + 2]; int i;
    for (i = 0; i < NP + 2; ++i) { m[i].X = g_pos[i].X; m[i].Y = g_pos[i].Y; m[i].Z = g_pos[i].Z; m[i].W = g_pos[i].W; }
    double want = Model(m, g_alpha, g_list, size, maxadj);
    float alpha0[NP + 2]; CParticleDistanceConstraint list0[NC + 2];
    memcpy(alpha0, g_alpha, sizeof alpha0); memcpy(list0, g_list, sizeof list0);
    memset(&g_sim, 0xA5, sizeof g_sim);
    At<V4*>(&g_sim, 0x1C) = g_pos; At<float*>(&g_sim, 0x2C) = g_alpha; At<float>(&g_sim, 0x30) = maxadj;
    Sim before = g_sim;
    CParticleSimulation::Call((CParticleSimulation*)&g_sim, g_list, size);
    float got = At<float>(&g_sim, 0x30);
    At<float>(&before, 0x30) = got;
    if (memcmp(&before, &g_sim, sizeof g_sim)) Fail("simulation bytes", 0, 0, 0);
    if (memcmp(alpha0, g_alpha, sizeof alpha0)) Fail("alphas written", 0, 0, 0);
    if (memcmp(list0, g_list, sizeof list0)) Fail("list written", 0, 0, 0);
    if (!Near(got, want, tol)) Fail("MaximumAdjustment", 0, got, want);
    for (i = 0; i < NP + 2; ++i)
    {
        if (!Near(g_pos[i].X, m[i].X, tol)) Fail("X", i, g_pos[i].X, m[i].X);
        if (!Near(g_pos[i].Y, m[i].Y, tol)) Fail("Y", i, g_pos[i].Y, m[i].Y);
        if (!Near(g_pos[i].Z, m[i].Z, tol)) Fail("Z", i, g_pos[i].Z, m[i].Z);
        if (g_pos[i].W != (float)m[i].W) Fail("W", i, g_pos[i].W, m[i].W);
    }
}
static void SetPos(int i, float x, float y, float z, float w) { g_pos[i].X = x; g_pos[i].Y = y; g_pos[i].Z = z; g_pos[i].W = w; }
static void SetC(int i, unsigned long a, unsigned long b, float distance, float strength) { g_list[i].Index1 = a; g_list[i].Index2 = b; g_list[i].Distance = distance; g_list[i].Strength = strength; }

static void Exact()
{
    // dyadic cases: every intermediate is exact, so the tolerance is zero
    int i;
    for (i = 0; i < NP + 2; ++i) { SetPos(i, 100.0f + i, 50.0f, 25.0f, 7.0f); g_alpha[i] = 0.5f; }
    SetPos(0, 0, 0, 0, 7); SetPos(1, 3, 0, 4, 7);            // 3-4-5: dist 5, divisor 5 (alphas 0.5 + 0.5)
    SetPos(3, 8, 1, 2, 7); SetPos(4, 4, 1, 2, 7);            // axis pair: dist 4
    g_alpha[3] = 0.25f; g_alpha[4] = 0.0f;                   // divisor 1
    SetPos(5, 1, 1, 1, 7); SetPos(6, 1, 1, 1, 7);            // coincident: dist 0, divisor clamps to 1e-4, stretch -2 * 0 moves nothing
    g_alpha[5] = 0.0f; g_alpha[6] = 0.0f;
    SetC(0, 0, 1, 0.0f, 5.0f);                               // factor 5: p0 += (7.5, 0, 10), p1 -= the same
    SetC(1, 3, 4, 2.0f, 0.5f);                               // stretch 2, factor 1: p3 += 0.25 * (-4, 0, 0); p4 fixed
    SetC(2, 5, 6, 2.0f, 0.0f);                               // strength 0
    SetC(3, 1, 0, 25.0f, 0.0f);                              // Index order swapped, strength 0: only the maximum and W
    Run(4, -1.0f, 0.0);
    if (!GEnableSIMD) Run(0, 3.5f, 0.0);                     // empty list: nothing changes (x87 loop only: the SSE kernel is a do-while,
                                                             // retail never hands it an empty list)
    Run(1, 9.0f, 0.0);                                       // the running maximum is kept when it is larger
    // audit: a small POSITIVE divisor under the limit is clamped up too (alphas 2^-16, dist 1: divisor 2^-15 < 1e-4)
    SetPos(0, 0, 0, 0, 7); SetPos(1, 1, 0, 0, 7); g_alpha[0] = 1.0f / 65536.0f; g_alpha[1] = 1.0f / 65536.0f;
    SetC(0, 0, 1, 0.5f, 1.0f);
    Run(1, -1.0f, 2e-5);
}

int main()
{
    for (int simd = 0; simd < 2; ++simd)
    {
        GEnableSIMD = simd != 0;
        Exact();
        g_seed = 12345;
        for (int n = 0; n < 4000; ++n)
        {
            int i;
            for (i = 0; i < NP + 2; ++i)
            {
                SetPos(i, RndF(-8.0f, 8.0f), RndF(-8.0f, 8.0f), RndF(-8.0f, 8.0f), RndF(1.0f, 2.0f));
                g_alpha[i] = (Rnd() % 5 == 0) ? 0.0f : RndF(0.125f, 1.0f);
            }
            unsigned long size = Rnd() % (NC + 1);
            if (GEnableSIMD && size == 0) size = 1;
            for (i = 0; i < NC + 2; ++i)
            {
                unsigned long a = Rnd() % NP, b = Rnd() % NP;
                if (a == b) b = (a + 1 + Rnd() % (NP - 1)) % NP;
                SetC(i, a, b, RndF(0.5f, 6.0f), RndF(0.0f, 0.75f));
            }
            Run(size, (n & 1) ? RndF(-4.0f, 4.0f) : -100.0f, 2e-5);
        }
    }
    if (g_fail) { printf("00adf960_TEST FAIL (%d of %d)\n", g_fail, g_cases); return 1; }
    printf("00adf960_TEST PASS (%d cases)\n", g_cases);
    return 0;
}