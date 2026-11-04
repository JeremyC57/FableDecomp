// test_CParticleSimulationSSE.cpp -- dual-path test of engine/CParticleSimulationSSE.h (the distance-constraint SSE kernel
// of CParticleSimulation::SatisfyDistanceConstraints, 0x00ADF960). Every case is chosen so each step is exact in float
// (3-4-5 and axis lengths, dyadic alphas, quotients that are powers of two or small integers), so the SSE kernel, the
// portable twin and the reference below agree bit for bit. Build modes (tools/decomp_pipeline/portable_twins_check.py):
//   VC7.1 (no define)               -- the asm macro against the reference
//   VC7.1 /DFABLE_DUAL_PATH_TEST    -- the asm macro AND fable_portable::SatisfyDistanceConstraintsSSE against each other
//   VC7.1 or g++ /DFABLE_PORTABLE   -- the portable expansion only
#include <stdio.h>
#include "engine/CParticleSimulationSSE.h"

#if defined(_MSC_VER)
#define TEST_ALIGN16 __declspec(align(16))
#else
#define TEST_ALIGN16 __attribute__((aligned(16)))
#endif

struct TEST_ALIGN16 TestVector { float v[4]; };
struct TestConstraint { unsigned int A, B; float Rest, Stiffness; };   // 16 bytes, CParticleDistanceConstraint layout

static const int kParticles = 6;
static const int kConstraints = 5;

static void Setup(TestVector* p, float* alphas, TestConstraint* c)
{
    static const float pos[kParticles][3] = {
        { 0.0f, 0.0f, 0.0f }, { 3.0f, 0.0f, 4.0f }, { 0.0f, 3.0f, 4.0f }, { 8.0f, 1.0f, 2.0f }, { 4.0f, 1.0f, 2.0f }, { 1.0f, 1.0f, 1.0f } };
    static const float alpha[kParticles] = { 0.5f, 0.5f, 0.25f, 0.25f, 0.0f, 0.0f };
    for (int i = 0; i < kParticles; ++i) {
        p[i].v[0] = pos[i][0]; p[i].v[1] = pos[i][1]; p[i].v[2] = pos[i][2];
        p[i].v[3] = 7.0f;                 // the kernel clears W of both particles first
        alphas[i] = alpha[i];
    }
    // len 5, rest 0, stiffness 1: diff 5, den (0.5 + 0.5) * 5 = 5, f = 1
    c[0].A = 0; c[0].B = 1; c[0].Rest = 0.0f; c[0].Stiffness = 1.0f;
    // after c[0] particle 0 and 1 meet at (1.5, 0, 2); 3-4-5 from particle 2 is gone, so use an axis pair instead:
    // particles 3 (8,1,2) and 4 (4,1,2): len 4, rest 2, stiffness 0.5 -> diff 1, den (0.25 + 0) * 4 = 1, f = 1
    c[1].A = 3; c[1].B = 4; c[1].Rest = 2.0f; c[1].Stiffness = 0.5f;
    // both alphas 0 (4, 5): den clamps to epsilon, the moves are alpha * g = 0; stretch is recorded
    c[2].A = 4; c[2].B = 5; c[2].Rest = 0.0f; c[2].Stiffness = 0.0f;
    // particle 2 to itself: d = 0, len 0, den clamps, diff -rest * 1 ... rest 0 keeps it exact (f = 0)
    c[3].A = 2; c[3].B = 2; c[3].Rest = 0.0f; c[3].Stiffness = 1.0f;
    // particle 2 (0,3,4) to particle 0 now at (1.5,0,2): d = (1.5,-3,-2); len^2 = 2.25 + 9 + 4 = 15.25 (not a square):
    // stiffness 0 keeps the move exact (diff 0) while the stretch exercises sqrt rounding identically on both paths
    c[4].A = 2; c[4].B = 0; c[4].Rest = 1.0f; c[4].Stiffness = 0.0f;
}

// Reference: the kernel's operation order spelled out with each step stored to a float.
static void Reference(TestVector* p, const float* alphas, const TestConstraint* c, int n, float eps, float* maxadj);

static int Check(const char* what, const float* got, const float* want, int n)
{
    int bad = 0;
    for (int i = 0; i < n; ++i)
        if (got[i] != want[i]) {
            printf("  %s[%d]: %.9g, expected %.9g\n", what, i, got[i], want[i]);
            ++bad;
        }
    return bad;
}

static void RunKernel(TestVector* positionsBase, const float* alphasBase, const TestConstraint* constraints, float* maxOut)
{
    TEST_ALIGN16 TestVector temp;
    TEST_ALIGN16 TestVector epsilon;
    TEST_ALIGN16 TestVector maxadj;
    TestVector* positions = positionsBase;
    const float* alphas = alphasBase;
    unsigned long stride = 16;
    unsigned long count = kConstraints;
    for (int l = 0; l < 4; ++l) { epsilon.v[l] = 0.0001f; maxadj.v[l] = 0.0f; temp.v[l] = 0.0f; }
    maxadj.v[0] = -1.0f;
    GF_SSE_SATISFY_DISTANCE_CONSTRAINTS(constraints, positions, alphas, stride, count, temp, epsilon, maxadj);
    *maxOut = maxadj.v[0];
}

int main()
{
    int bad = 0;
    TEST_ALIGN16 TestVector p[kParticles], want[kParticles];
    float alphas[kParticles];
    TestConstraint c[kConstraints];
    Setup(p, alphas, c);
    Setup(want, alphas, c);
    float maxGot = 0.0f, maxWant = -1.0f;
    Reference(want, alphas, c, kConstraints, 0.0001f, &maxWant);
    RunKernel(p, alphas, c, &maxGot);
    bad += Check("positions", &p[0].v[0], &want[0].v[0], kParticles * 4);
    bad += Check("maxadj", &maxGot, &maxWant, 1);
    // spot values that do not depend on the reference: particles 0 and 1 meet at (1.5, 0, 2); 3 moves to (7,1,2)
    const float meet[4] = { 1.5f, 0.0f, 2.0f, 0.0f }, moved[4] = { 7.0f, 1.0f, 2.0f, 0.0f };
    bad += Check("meet0", p[0].v, meet, 4);
    bad += Check("meet1", p[1].v, meet, 4);
    bad += Check("moved3", p[3].v, moved, 4);
#if defined(FABLE_DUAL_PATH_TEST) && !defined(FABLE_PORTABLE)
    {
        TEST_ALIGN16 TestVector twin[kParticles];
        TEST_ALIGN16 TestVector eps;
        TEST_ALIGN16 TestVector maxadj;
        Setup(twin, alphas, c);
        for (int l = 0; l < 4; ++l) { eps.v[l] = 0.0001f; maxadj.v[l] = 0.0f; }
        maxadj.v[0] = -1.0f;
        fable_portable::SatisfyDistanceConstraintsSSE(c, &twin[0].v[0], alphas, 16, kConstraints, eps.v, maxadj.v);
        bad += Check("twin positions", &twin[0].v[0], &p[0].v[0], kParticles * 4);
        bad += Check("twin maxadj", &maxadj.v[0], &maxGot, 1);
    }
#endif
#if defined(FABLE_DUAL_PATH_TEST) && !defined(FABLE_PORTABLE)
    const char* mode = "asm+portable";
#elif defined(FABLE_PORTABLE)
    const char* mode = "portable";
#else
    const char* mode = "asm";
#endif
    printf("test_CParticleSimulationSSE %s: %s\n", mode, bad ? "FAIL" : "PASS");
    return bad ? 1 : 0;
}

#include <math.h>
static void Reference(TestVector* p, const float* alphas, const TestConstraint* c, int n, float eps, float* maxadj)
{
    for (int k = 0; k < n; ++k) {
        float* pi = p[c[k].A].v;
        float* pj = p[c[k].B].v;
        pi[3] = 0.0f; pj[3] = 0.0f;
        float d[4];
        for (int l = 0; l < 4; ++l) d[l] = pj[l] - pi[l];
        float sx = d[0] * d[0], sy = d[1] * d[1], sz = d[2] * d[2], sw = d[3] * d[3];
        float s02 = sx + sz, s13 = sy + sw;
        float sum = s02 + s13;
        float len = (float)sqrt((double)sum);
        float stretch = len - c[k].Rest;
        if (!(*maxadj > stretch)) *maxadj = stretch;
        float diff = stretch * c[k].Stiffness;
        float ai = alphas[c[k].A], aj = alphas[c[k].B];
        float as = ai + aj;
        float den = as * len;
        if (!(den > eps)) den = eps;
        float f = diff / den;
        for (int l = 0; l < 4; ++l) {
            float g = d[l] * f;
            float mi = ai * g, mj = aj * g;
            pi[l] = pi[l] + mi;
            pj[l] = pj[l] - mj;
        }
    }
}
