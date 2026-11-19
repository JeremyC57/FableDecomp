// CParticleSimulation::SatisfyDistanceConstraints(const CParticleDistanceConstraint*, unsigned long) -- pull each constrained particle pair
// Grade: DIFFER(764v766) -- one residue: the x87 loop walks the constraint array with edx biased to +4 (Index2) where retail biases it to +8 (Distance), 9 lines; the SSE arm (GF_SSE_SATISFY_DISTANCE_CONSTRAINTS), its C++ set-up, the /Zi realigned ebx frame and the x87 maths all match; tried index, pointer, count-down and macro-access loops
//   toward its rest distance weighted by the particles' alphas and record the largest stretch in MaximumAdjustment; retail 0x00ADF960,
//   766 bytes, `ret 8`, realigned ebx/ebp frame (/Zi: the return-address copy).
// Retail-asm: 0x00ADF9DD `mov esi,[ebx+8]` .. 0x00ADFA3F `sqrtps xmm4,xmm4` .. 0x00ADFA57 `push esi` .. 0x00ADFAAB `dec dword ptr [ebp-4]` (GF_SSE_SATISFY_DISTANCE_CONSTRAINTS; NOTES site 4)
// Leaf: ?SatisfyDistanceConstraints@CParticleSimulation@@AAEXPBVCParticleDistanceConstraint@@K@Z
// Flags: /Zi
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CPARTICLESIMULATION
#include "engine/CParticleSimulation.h"
#include "engine/CParticleDistanceConstraint.h"
#include "engine/CParticleSimulationSSE.h"
#include <math.h>
#pragma intrinsic(sqrt)
extern bool g_SSEAvailable;                                  // 0x013D2880
extern const float PARTICLE_CONSTRAINT_EPSILON;              // 0x0129F0BC (1e-4f)
void CParticleSimulation::SatisfyDistanceConstraints(const CParticleDistanceConstraint* constraints, unsigned long count)
{
    if (g_SSEAvailable) {
        float* alphas = SimulationAlphas;
        unsigned long stride = sizeof(CParticleDistanceConstraint);
        unsigned long remaining = count;
        __declspec(align(16)) C4DVector epsilon(1e-4f, 1e-4f, 1e-4f, 1e-4f);
        __declspec(align(16)) C4DVector max_adjustment(MaximumAdjustment, 0.0f, 0.0f, 0.0f);
        C4DVector* positions = Positions;
        __declspec(align(16)) C4DVector temp;
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
