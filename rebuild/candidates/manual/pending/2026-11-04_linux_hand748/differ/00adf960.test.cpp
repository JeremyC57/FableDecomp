// Behaviour test for CParticleSimulation::SatisfyDistanceConstraints (0x00ADF960): constrained pairs pulled toward rest length by their
// alphas, W cleared, MaximumAdjustment the largest stretch;
// both the SSE path (the shared kernel macro, its C++ twin under -DFABLE_PORTABLE) and the x87 path are run on dyadic
// inputs, so they agree bit for bit. A model of the row over engine/ helper headers (collaborators are stubs).
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "engine/CParticleSimulationSSE.h"
#if defined(_MSC_VER)
#define ALIGN16 __declspec(align(16))
#else
#define ALIGN16 __attribute__((aligned(16)))
#endif
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float k) const { return C3DVector(X * k, Y * k, Z * k); }
    C3DVector& operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; return *this; }
    float GetMagnitude() const { return (float)sqrt((double)(X * X + Y * Y + Z * Z)); }
};
struct ALIGN16 C4DVector {
    float X, Y, Z, W;
    C4DVector() {}
    C4DVector(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}
    C3DVector ToC3DVector() const { return C3DVector(X, Y, Z); }
    void Initialise(const C3DVector& v, float w) { X = v.X; Y = v.Y; Z = v.Z; W = w; }
};
struct CParticleDistanceConstraint { unsigned int Index1, Index2; float Distance, Strength; };   // 16 bytes
static bool g_SSEAvailable;
static const float PARTICLE_CONSTRAINT_EPSILON = 1e-4f;
struct CParticleSimulation {
    C4DVector* Positions;
    float* SimulationAlphas;
    float MaximumAdjustment;
    void SatisfyDistanceConstraints(const CParticleDistanceConstraint* constraints, unsigned long count)
    {
        if (g_SSEAvailable) {
            float* alphas = SimulationAlphas;
            unsigned long stride = sizeof(CParticleDistanceConstraint);
            unsigned long remaining = count;
            ALIGN16 C4DVector epsilon(1e-4f, 1e-4f, 1e-4f, 1e-4f);
            ALIGN16 C4DVector max_adjustment(MaximumAdjustment, 0.0f, 0.0f, 0.0f);
            C4DVector* positions = Positions;
            ALIGN16 C4DVector temp;
            GF_SSE_SATISFY_DISTANCE_CONSTRAINTS(constraints, positions, alphas, stride, remaining, temp, epsilon, max_adjustment);
            MaximumAdjustment = max_adjustment.X;
            return;
        }
        for (unsigned long i = 0; i < count; ++i) {
            const CParticleDistanceConstraint& c = constraints[i];
            C3DVector p1 = Positions[c.Index1].ToC3DVector();
            C3DVector p2 = Positions[c.Index2].ToC3DVector();
            float a1 = SimulationAlphas[c.Index1];
            float a2 = SimulationAlphas[c.Index2];
            C3DVector d = p2 - p1;
            float length = d.GetMagnitude();
            float den = (a2 + a1) * length;
            if (den < PARTICLE_CONSTRAINT_EPSILON)
                den = PARTICLE_CONSTRAINT_EPSILON;
            float stretch = length - c.Distance;
            float k = c.Strength / den * stretch;
            p1 += d * (a1 * k);
            C3DVector d2 = d * (k * a2);
            p2.X -= d2.X; p2.Y -= d2.Y; p2.Z -= d2.Z;
            MaximumAdjustment = MaximumAdjustment > stretch ? MaximumAdjustment : stretch;
            Positions[c.Index1].Initialise(p1, 0.0f);
            Positions[c.Index2].Initialise(p2, 0.0f);
        }
    }
};
static int g_bad;
static void Expect(const char* what, int pass, float got, float want)
{
    if (got != want) { printf("  pass %d %s: %.9g, expected %.9g\n", pass, what, got, want); ++g_bad; }
}
int main()
{
    // exact cases: 3-4-5 pair (den 5, strength 5 -> f = k = 5), an axis pair (den 1), zero alphas (den clamps, no move),
    // a self pair (length 0), and a stiffness-0 pair (no move, irrational stretch below the running maximum)
    static const float start[6][3] = { { 0, 0, 0 }, { 3, 0, 4 }, { 0, 3, 4 }, { 8, 1, 2 }, { 4, 1, 2 }, { 1, 1, 1 } };
    float alphas[6] = { 0.5f, 0.5f, 0.25f, 0.25f, 0.0f, 0.0f };
    const CParticleDistanceConstraint c[5] = {
        { 0, 1, 0.0f, 5.0f }, { 3, 4, 2.0f, 0.5f }, { 4, 5, 0.0f, 0.0f }, { 2, 2, 0.0f, 1.0f }, { 2, 5, 1.0f, 0.0f } };
    ALIGN16 C4DVector result[2][6];
    float maxadj[2];
    for (int pass = 0; pass < 2; ++pass) {
        g_SSEAvailable = pass == 0;
        ALIGN16 C4DVector p[6];
        for (int i = 0; i < 6; ++i) p[i] = C4DVector(start[i][0], start[i][1], start[i][2], 7.0f);
        CParticleSimulation sim;
        sim.Positions = p; sim.SimulationAlphas = alphas; sim.MaximumAdjustment = -1.0f;
        sim.SatisfyDistanceConstraints(c, 5);
        // pair 0: d = (3,0,4), k = 5 -> p0 += 0.5 * d * 5 = (7.5,0,10), p1 -= (7.5,0,10)
        Expect("p0.X", pass, p[0].X, 7.5f); Expect("p0.Z", pass, p[0].Z, 10.0f);
        Expect("p1.X", pass, p[1].X, -4.5f); Expect("p1.Z", pass, p[1].Z, -6.0f);
        // pair 1: d = (-4,0,0), den 1, k 1 -> p3 += 0.25 * (-4) = 7; p4 unchanged (alpha 0)
        Expect("p3.X", pass, p[3].X, 7.0f); Expect("p4.X", pass, p[4].X, 4.0f);
        Expect("p5.X", pass, p[5].X, 1.0f);
        for (int i = 0; i < 6; ++i) Expect("W cleared", pass, p[i].W, 0.0f);   // every particle is in a pair
        Expect("maxadj", pass, sim.MaximumAdjustment, 5.0f);
        memcpy(result[pass], p, sizeof(p));
        maxadj[pass] = sim.MaximumAdjustment;
    }
    for (int i = 0; i < 6; ++i) {                   // the SSE kernel and the x87 loop agree bit for bit
        Expect("sse==x87 X", 1, result[0][i].X, result[1][i].X);
        Expect("sse==x87 Y", 1, result[0][i].Y, result[1][i].Y);
        Expect("sse==x87 Z", 1, result[0][i].Z, result[1][i].Z);
    }
    Expect("sse==x87 maxadj", 1, maxadj[0], maxadj[1]);
    printf(g_bad ? "00adf960_TEST FAIL\n" : "00adf960_TEST PASS\n");
    return g_bad ? 1 : 0;
}
