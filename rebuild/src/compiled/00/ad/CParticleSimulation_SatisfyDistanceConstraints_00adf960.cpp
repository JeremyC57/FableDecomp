// CParticleSimulation::SatisfyDistanceConstraints(const CParticleDistanceConstraint* list, unsigned long size) -- pull each constrained particle pair toward its rest distance, weighted by the particles' alphas, scaled by the constraint's strength, and record the largest stretch in MaximumAdjustment; the SSE kernel when the CPU has it, else the x87 loop
// Leaf: ?SatisfyDistanceConstraints@CParticleSimulation@@AAEXPBVCParticleDistanceConstraint@@K@Z
// Flags: /Zi
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADF960, 766 bytes, `ret 8`, realigned ebx/ebp frame (/Zi: the return-address copy).
// Retail-asm: 0x00ADF9DD `mov esi,[ebx+8]` .. 0x00ADFA3F `sqrtps xmm4,xmm4` .. 0x00ADFA57 `push esi` .. 0x00ADFAAB `dec dword ptr [ebp-4]`
//   (GF_SSE_SATISFY_DISTANCE_CONSTRAINTS, engine/CParticleSimulationSSE.h). This row carries no asm of its own.
// Identity: FableWin.pdb + Ego_r.pdb ?SatisfyDistanceConstraints@CParticleSimulation@@AAEXPBVCParticleDistanceConstraint@@K@Z (private).
//   Locals = Ego_d.pdb (debug length 891): list, size, limit; SSE scope { pos, max_adjust, alphas, temp, list_size, list_stride,
//   divisor_limit }; loop { c1 { vect, dist, inv_mass1, constraint, error, inv_mass2, divisor, pos1, pos2 } } (Ego_d types: pos
//   C4DVector*, alphas const float*, list_stride const unsigned long, constraint const CParticleDistanceConstraint&; `limit` is a plain
//   float, type 0x40, not const).
// Data: GEnableSIMD 0x013D2880; 1e-4f 0x0129F0BC (pool literal = `limit`).
// Shape: Ego_d.pdb's C13 line table (ref/00adf960/egod_lines.tsv, lib_particle_simulation.cpp 352..466; VC7.1 /Od sizes of this text
//   beside it, tools/odl2.py). SSE scope in the table's order: list_stride (7), alphas (9), list_size (6), divisor_limit (36),
//   max_adjust (36), temp (8), pos (9) -- byte for byte. x87 loop: constraint 12, pos1 23, pos2 24, inv_mass1 17, inv_mass2 18, vect 19,
//   dist 17, divisor 18 (all exact), the clamp on ONE line (27 v 25), then
//     line 458 = 30 bytes: `error` is the whole correction factor, (dist - Distance) / divisor * Strength (30 exact; a plain
//       `dist - constraint.Distance` is 18), so there is no `k` local, as Ego_d's locals say;
//     line 461 = 71 bytes: the maximum compares and selects `dist - constraint.Distance` itself, evaluated twice = a max() macro
//       expansion (69 here; VC10 adds 2 to every float compare line, as on the clamp). Written through the macro itself (audit h3c2b):
//       windef.h's `max`, the SDK text in engine/Win32Types.h (FABLE_RETAIL_WIN32_MINMAX; the lane has no Platform SDK). The hand-expanded
//       ternary and the CRT's `__max` (<stdlib.h>, same expansion) are RELOCATION_MATCH too; the clamp is NOT the macro
//       (`divisor = max(divisor, limit)`: DIFFER(766v766)), it is the one-line `if`.
//   Those two lines are what the row was missing for 28 grades: with the stretch mentioned three times and `error` holding the factor,
//   VC7.1 biases the strength-reduced constraint pointer list+8 (0x00ADFAD6 `add edx,8`; Index1 / Index2 at [edx-8] / [edx-4]) and
//   keeps the stretch in the dead `dist` word ([ebp-4], 0x00ADFB7A) for the maximum. The earlier text (`error = dist - Distance`, a
//   second factor statement, `max(MaximumAdjustment, error)`) gave list+4, DIFFER(764v766).
//   Lines 459 / 460 are 52 bytes in Ego_d and 40 here: `vect * inv_mass1 * error` has 52 but compiles to six multiplies (790 bytes);
//   retail's four multiplies (0x00ADFB8A `fmul [ebp-8]` then three `fmul st(1)`) are the scalar-first form below.
// Inferred: which macro: Ego_d shows only the double evaluation; `max` (windef.h) is the SDK's name for it in a C++ TU.
// Inferred: the operand order inside `error`: `(dist - constraint.Distance) * constraint.Strength / divisor` matches too (same 30
//   bytes /Od, same retail bytes: VC7.1 reassociates to retail's `fld Strength ; fdiv st(1) ; fmul [ebp-4]`, 0x00ADFB7D); `constraint.
//   Strength / divisor * (dist - constraint.Distance)` matches retail but is 32 bytes /Od. The divisor sum is the natural
//   `(inv_mass1 + inv_mass2) * dist` (`(inv_mass2 + inv_mass1)` and `dist * (..)` compile alike).
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SUBTRACT_ASSIGN
#define FABLE_RETAIL_CPARTICLESIMULATION
#include "engine/CParticleSimulation.h"
#include "engine/CParticleDistanceConstraint.h"
#include "engine/CParticleSimulationSSE.h"
#include <math.h>
#define FABLE_RETAIL_WIN32_MINMAX
#include "engine/Win32Types.h"
#pragma intrinsic(sqrt)
extern bool GEnableSIMD;                                     // 0x013D2880 (Ego_d.pdb ?GEnableSIMD@@3_NA)
void CParticleSimulation::SatisfyDistanceConstraints(const CParticleDistanceConstraint* list, unsigned long size)
{
    float limit = 0.0001f;
    if (GEnableSIMD)
    {
        const unsigned long list_stride = sizeof(CParticleDistanceConstraint);
        const float* alphas = SimulationAlphas;
        unsigned long list_size = size;
        __declspec(align(16)) C4DVector divisor_limit(limit, limit, limit, limit);
        __declspec(align(16)) C4DVector max_adjust(MaximumAdjustment, 0.0f, 0.0f, 0.0f);
        __declspec(align(16)) C4DVector temp;
        C4DVector* pos = Positions;
        GF_SSE_SATISFY_DISTANCE_CONSTRAINTS(list, pos, alphas, list_stride, list_size, temp, divisor_limit, max_adjust);
        MaximumAdjustment = max_adjust.X;
        return;
    }
    for (unsigned long c1 = 0; c1 < size; ++c1)
    {
        const CParticleDistanceConstraint& constraint = list[c1];
        C3DVector pos1 = Positions[constraint.Index1].ToC3DVector();
        C3DVector pos2 = Positions[constraint.Index2].ToC3DVector();
        float inv_mass1 = SimulationAlphas[constraint.Index1];
        float inv_mass2 = SimulationAlphas[constraint.Index2];
        C3DVector vect = pos2 - pos1;
        float dist = vect.GetMagnitude();
        float divisor = (inv_mass1 + inv_mass2) * dist;
        if (divisor < limit) divisor = limit;
        float error = (dist - constraint.Distance) / divisor * constraint.Strength;
        pos1 += vect * (inv_mass1 * error);
        pos2 -= vect * (inv_mass2 * error);
        MaximumAdjustment = max(MaximumAdjustment, dist - constraint.Distance);
        Positions[constraint.Index1].Initialise(pos1, 0.0f);
        Positions[constraint.Index2].Initialise(pos2, 0.0f);
    }
}