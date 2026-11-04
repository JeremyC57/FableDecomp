// CParticleSimulationSSE.h -- the hand-written SSE kernel of CParticleSimulation::SatisfyDistanceConstraints (0x00ADF960),
// isolated as a macro. The macro names the caller's variables directly (an `__asm` block reads each C local or parameter
// from its frame slot): retail's block reloads the constraint pointer from its argument slot (`mov esi,[ebx+8]`, the
// realigned ebx frame), the positions / alphas / stride / count from ebp frame words, keeps the loop count in memory
// (`dec dword ptr [ebp-4]`) and borrows esi around the alpha reads (`push esi ; mov esi,[ebp-0Ch] ; ... ; pop esi`).
// Retail-asm-helper: GF_SSE_SATISFY_DISTANCE_CONSTRAINTS
// Retail-asm: 0x00ADF9DD `mov esi,[ebx+8]` .. 0x00ADFA3F `sqrtps xmm4,xmm4` + 0x00ADFA4C `maxss xmm6,xmm5` + 0x00ADFA57
//   `push esi` .. 0x00ADFAAB `dec dword ptr [ebp-4]` (CParticleSimulation::SatisfyDistanceConstraints). A loop whose
//   counter lives in a frame word, an `add esi,[ebp-10h]` stride from memory and a push/pop of a register the compiler
//   already holds are not VC7.1 code generation for any C++ or xmmintrin spelling (sqrtps / maxss / maxps / divps exist
//   as intrinsics, but the compiler allocates the loop registers itself and never spills a live esi with push/pop).
// Semantics (per constraint k, i = index A, j = index B, positions 16-byte aligned with W cleared first):
//   d = Pj - Pi; len = sqrt((dx*dx + dz*dz) + (dy*dy + 0)) (the 4Eh / 11h shuffle tree);
//   maxadj.X = maxss(maxadj.X, len - rest) (second operand when either is NaN); diff = (len - rest) * stiffness;
//   den = max((alpha_i + alpha_j) * len, epsilon); f = diff / den; Pi += alpha_i * (d * f); Pj -= alpha_j * (d * f).
//   Every operation is one IEEE single-precision operation (round to nearest).
// Operands: constraints -- const CParticleDistanceConstraint* (16 bytes: index A, index B, rest length, stiffness);
//   positions -- C4DVector* (16-aligned); alphas -- const float*; stride -- unsigned long local = 16; count -- unsigned
//   long local (> 0, decremented to 0); temp -- an aligned 16-byte scratch local; epsilon -- an aligned four-float local
//   (retail: 1e-4f x4); maxadj -- an aligned four-float local whose X is the running maximum adjustment.
// Clobbers: eax, ecx, edx, esi, edi, xmm0..xmm7, temp, count.
// Portable twin (FABLE_PORTABLE): plain C++ in fable_portable; tests in tests/portable/test_CParticleSimulationSSE.cpp.
#ifndef FABLE_ENGINE_CPARTICLESIMULATIONSSE_H
#define FABLE_ENGINE_CPARTICLESIMULATIONSSE_H

#if defined(FABLE_PORTABLE) || defined(FABLE_DUAL_PATH_TEST)
#include <math.h>
namespace fable_portable
{
    inline float SseMaxss(float a, float b) { return a > b ? a : b; }   // maxss: b unless a > b (NaN -> b)
    inline void SatisfyDistanceConstraintsSSE(const void* constraints, float* positions, const float* alphas,
                                              unsigned long stride, unsigned long count, const float* epsilon, float* maxadj)
    {
        const unsigned char* c = (const unsigned char*)constraints;
        for (; count; --count, c += stride) {
            const unsigned int a = ((const unsigned int*)c)[0], b = ((const unsigned int*)c)[1];   // 32-bit indices
            const float rest = ((const float*)c)[2], stiffness = ((const float*)c)[3];
            float* pi = positions + a * 4;
            float* pj = positions + b * 4;
            pi[3] = 0.0f; pj[3] = 0.0f;
            float d[4], sq[4];
            for (int l = 0; l < 4; ++l) { d[l] = pj[l] - pi[l]; sq[l] = d[l] * d[l]; }
            float s02 = sq[0] + sq[2], s13 = sq[1] + sq[3];
            float len = (float)sqrt((double)(float)(s02 + s13));
            float stretch = len - rest;
            maxadj[0] = SseMaxss(maxadj[0], stretch);
            float diff = stretch * stiffness;
            float ai = alphas[a], aj = alphas[b];
            float den = (ai + aj) * len;
            den = den > epsilon[0] ? den : epsilon[0];
            float f = diff / den;
            for (int l = 0; l < 4; ++l) {
                float g = d[l] * f;
                pi[l] = pi[l] + ai * g;
                pj[l] = pj[l] - aj * g;
            }
        }
    }
}
#define FABLE_PORTABLE_SSE_SATISFY_DISTANCE_CONSTRAINTS(constraints, positions, alphas, stride, count, temp, epsilon, maxadj) \
    ((void)(temp), fable_portable::SatisfyDistanceConstraintsSSE((constraints), (float*)(positions), (alphas), (stride), \
                                                                 (count), (const float*)&(epsilon), (float*)&(maxadj)))
#endif

#ifdef FABLE_PORTABLE
#define GF_SSE_SATISFY_DISTANCE_CONSTRAINTS(constraints, positions, alphas, stride, count, temp, epsilon, maxadj) \
    FABLE_PORTABLE_SSE_SATISFY_DISTANCE_CONSTRAINTS(constraints, positions, alphas, stride, count, temp, epsilon, maxadj)
#else
#define GF_SSE_SATISFY_DISTANCE_CONSTRAINTS(constraints, positions, alphas, stride, count, temp, epsilon, maxadj) \
    __asm mov esi, constraints \
    __asm mov edi, positions \
    __asm gf_sse_distance_constraint_next: \
    __asm mov eax, [esi+8] \
    __asm mov edx, [esi+0Ch] \
    __asm mov dword ptr temp, eax \
    __asm mov dword ptr temp+4, edx \
    __asm movaps xmm0, temp \
    __asm movaps xmm1, temp \
    __asm shufps xmm0, xmm0, 00h \
    __asm shufps xmm1, xmm1, 55h \
    __asm mov eax, [esi] \
    __asm mov edx, [esi+4] \
    __asm shl eax, 2 \
    __asm shl edx, 2 \
    __asm mov dword ptr [edi+eax*4+0Ch], 0 \
    __asm mov dword ptr [edi+edx*4+0Ch], 0 \
    __asm movaps xmm2, [edi+eax*4] \
    __asm movaps xmm3, [edi+edx*4] \
    __asm movaps xmm4, xmm3 \
    __asm subps xmm4, xmm2 \
    __asm mulps xmm4, xmm4 \
    __asm movaps xmm5, xmm4 \
    __asm shufps xmm5, xmm5, 4Eh \
    __asm addps xmm4, xmm5 \
    __asm movaps xmm5, xmm4 \
    __asm shufps xmm5, xmm5, 11h \
    __asm addps xmm4, xmm5 \
    __asm sqrtps xmm4, xmm4 \
    __asm movaps xmm6, maxadj \
    __asm movaps xmm5, xmm4 \
    __asm subps xmm5, xmm0 \
    __asm maxss xmm6, xmm5 \
    __asm mulps xmm5, xmm1 \
    __asm movaps maxadj, xmm6 \
    __asm push esi \
    __asm mov esi, alphas \
    __asm mov ecx, [esi+eax] \
    __asm mov dword ptr temp, ecx \
    __asm mov ecx, [esi+edx] \
    __asm mov dword ptr temp+4, ecx \
    __asm pop esi \
    __asm movaps xmm6, temp \
    __asm movaps xmm7, temp \
    __asm shufps xmm6, xmm6, 00h \
    __asm shufps xmm7, xmm7, 55h \
    __asm movaps xmm0, xmm6 \
    __asm addps xmm0, xmm7 \
    __asm mulps xmm0, xmm4 \
    __asm maxps xmm0, epsilon \
    __asm movaps xmm1, xmm5 \
    __asm divps xmm1, xmm0 \
    __asm movaps xmm0, xmm3 \
    __asm subps xmm0, xmm2 \
    __asm mulps xmm0, xmm1 \
    __asm mulps xmm6, xmm0 \
    __asm mulps xmm7, xmm0 \
    __asm addps xmm2, xmm6 \
    __asm subps xmm3, xmm7 \
    __asm movaps [edi+eax*4], xmm2 \
    __asm movaps [edi+edx*4], xmm3 \
    __asm add esi, stride \
    __asm dec count \
    __asm jne gf_sse_distance_constraint_next
#endif

#endif  // FABLE_ENGINE_CPARTICLESIMULATIONSSE_H
