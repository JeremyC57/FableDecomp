// ReverbFnsSSEFilterOrder2Quad.h -- FABLE_PORTABLE twin of GameCODA 2.10 CReverbFnsSSE::ApplyFilterOrder2_quad (0x00C88E60), the
// library row graded toolchain_library (rebuild/src/compiled/00/c8/..._00c88e60.cpp: byte-identical to GameCODA 2.10 CODAApi.lib
// member ReverbFns_SSE.obj ?ApplyFilterOrder2_quad@CReverbFnsSSE@@QAEXPAMPBMPAPAMPAPAVCFloatFilterParams@@I@Z). The matching build
// compiles nothing from this header; it holds only plain C++ (owner rule 2026-11-19: a library-graded GameCODA row needs a portable
// twin, because CODAApi.lib is binary-only Win32 x86).
//
// What the member does (retail order): gather the four filters into a block of ten four-lane rows, a1 (0), a2 (4),
// c2 = 1 - (a1 + a2) (8), b0 (12), b1 (16), b2 (20), c6 = -(b0 + b1 + b2) (24), h0 = apHistory[l][0] (28), h1 = apHistory[l][1] (32),
// g = gain (36); run the kernel; store h0 / h1 back to apHistory[l][0] / [1], lane 0 to 3, [0] before [1]. Kernel, per sample
// n = 0 .. uiSamples-1, lanes at l*128: all four inputs x[l] = pfSrc[l*128] are read before any output is written, then
//   w = (x + c2) + (a2*h1 + a1*h0);  y = ((b0*w + (b2*h1 + b1*h0)) + c6) * g;  h1 = h0;  h0 = w;  pfDest[l*128] = y
// and the loop is `dec ecx ; jne` (uiSamples == 0 runs 2^32 times; the SDK's debug build asserts uiSamples != 0 and % 4 == 0).
//
// Portability statement. Measured (tests/portable/test_ReverbFnsSSEFilterOrder2Quad.cpp and rows/00c88e60_..._driver.cpp, the
// retail side being the CODAApi.lib member):
// - Kernel: every retail step is one SSE single-precision operation under the default MXCSR. On inexact samples / histories / gains
//   the member matches numpy float32 goldens (one rounding per operation, the kernel's order), and the twin matches the same
//   goldens where float is evaluated in single precision (g++ x86-64). Exact (dyadic) data agrees bit for bit in every build,
//   VC7.1's x87 portable build included (rows driver: 1002 lines identical).
// - Gather: retail computes c2 and c6 with x87 instructions (fld / fadd / fsubr / fchs / fstp), so ITS bits depend on the
//   calling thread's x87 precision control. Measured on the member: under PC_24 every step rounds to single precision; under PC_53
//   the sum rounds once, at the store (on the test values: c2 0xBED815B0 v 0xBED815B2; c6 differs by lane under PC_24, not under
//   PC_53). VC7.1 also re-associated the three-term sum per unrolled lane (retail +0x059, +0x0B5, +0x10F, +0x169): lanes 0 and 1
//   (b0 + b1) + b2, lane 2 (b1 + b2) + b0, lane 3 (b2 + b0) + b1. The twin computes the gather in float with those per-lane
//   orders, so it is bit-identical to retail running under PC_24; under PC_53 retail it can differ in the last bit of c2 / c6
//   for inexact coefficients. Which precision control retail's audio code runs under is NOT established (Direct3D 9 sets PC_24
//   on the thread that creates the device unless D3DCREATE_FPU_PRESERVE; the CRT default is PC_53). PC_64 was not measured.
// - Overlap: as retail, the twin reads all four lanes of a sample before writing any (measured with pfDest == pfSrc and with
//   pfDest one lane above / below pfSrc). Alignment is not needed by the twin (retail needs only its own aligned block).
// Derived from the instructions, not measured: the twin needs single-precision evaluation with no contraction (FLT_EVAL_METHOD 0,
//   -ffp-contract=off where FMA exists) and the default FP environment (no FTZ / DAZ); with x87 evaluation (VC7.1, g++ -m32
//   -mfpmath=387) inexact data can differ in the last bit. NaN-ness agrees, but a NaN's payload / sign may not (SSE returns its
//   first NaN operand; C++ leaves the choice to the compiler). uiSamples == 0 runs 2^32 samples in both (`dec ecx ; jne`).
#ifndef FABLE_ENGINE_REVERBFNSSSEFILTERORDER2QUAD_H
#define FABLE_ENGINE_REVERBFNSSSEFILTERORDER2QUAD_H

#if defined(FABLE_PORTABLE) || defined(FABLE_DUAL_PATH_TEST)
namespace fable_portable
{
    // The kernel over the gathered block pfData (float[40], ten four-lane rows as above); h0 / h1 rows are updated in place.
    inline void ApplyFilterOrder2QuadKernel(float* pfDest, const float* pfSrc, unsigned int uiSamples, float* pfData)
    {
        float h0[4], h1[4], x[4];
        int l;
        for (l = 0; l < 4; ++l) { h0[l] = pfData[28 + l]; h1[l] = pfData[32 + l]; }
        do {
            for (l = 0; l < 4; ++l) x[l] = pfSrc[l * 128];
            for (l = 0; l < 4; ++l) {
                const float fb = pfData[4 + l] * h1[l] + pfData[0 + l] * h0[l];      // a2*h1 + a1*h0
                const float ff = pfData[20 + l] * h1[l] + pfData[16 + l] * h0[l];    // b2*h1 + b1*h0
                const float w = (x[l] + pfData[8 + l]) + fb;
                const float y = ((pfData[12 + l] * w + ff) + pfData[24 + l]) * pfData[36 + l];
                h1[l] = h0[l];
                h0[l] = w;
                x[l] = y;
            }
            for (l = 0; l < 4; ++l) pfDest[l * 128] = x[l];
            ++pfSrc;
            ++pfDest;
        } while (--uiSamples);
        for (l = 0; l < 4; ++l) { pfData[28 + l] = h0[l]; pfData[32 + l] = h1[l]; }
    }
}
#endif

#ifdef FABLE_PORTABLE
// The member itself, for the portable build only. Included only by the row stub (and by its tests), which first define
// FABLE_RETAIL_PASS22_FLOATREVERB_CREATE and FABLE_RETAIL_CREVERBFNSSSE_APPLYFILTERORDER2_QUAD (the CFloatFilterParams friend:
// the SDK reads the private coefficients directly, as its /Od body shows; the class has public getters only for gain, a0, b0).
#if !defined(FABLE_RETAIL_PASS22_FLOATREVERB_CREATE) || !defined(FABLE_RETAIL_CREVERBFNSSSE_APPLYFILTERORDER2_QUAD)
#error "define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE and FABLE_RETAIL_CREVERBFNSSSE_APPLYFILTERORDER2_QUAD first (see the 0x00C88E60 row stub)"
#endif
#include "engine/CReverbFnsSSE.h"
#include "engine/CFloatFilterParams.h"
void CReverbFnsSSE::ApplyFilterOrder2_quad(float *pfDest, const float *pfSrc, float **apHistory, CFloatFilterParams **aFilterParams, unsigned int uiSamples)
{
    float fData[40];
    int i;

    for (i = 0; i < 4; i++)
    {
        const CFloatFilterParams *p = aFilterParams[i];
        fData[i] = p->m_fA[1];
        fData[i + 4] = p->m_fA[2];
        fData[i + 8] = 1.0f - (p->m_fA[1] + p->m_fA[2]);
        fData[i + 12] = p->m_fB[0];
        fData[i + 16] = p->m_fB[1];
        fData[i + 20] = p->m_fB[2];
        if (i == 2)
            fData[i + 24] = -((p->m_fB[1] + p->m_fB[2]) + p->m_fB[0]);      // retail +0x10F
        else if (i == 3)
            fData[i + 24] = -((p->m_fB[2] + p->m_fB[0]) + p->m_fB[1]);      // retail +0x169
        else
            fData[i + 24] = -((p->m_fB[0] + p->m_fB[1]) + p->m_fB[2]);      // retail +0x059, +0x0B5
        fData[i + 28] = apHistory[i][0];
        fData[i + 32] = apHistory[i][1];
        fData[i + 36] = p->m_fGain;
    }

    fable_portable::ApplyFilterOrder2QuadKernel(pfDest, pfSrc, uiSamples, fData);

    for (i = 0; i < 4; i++)
    {
        apHistory[i][0] = fData[i + 28];
        apHistory[i][1] = fData[i + 32];
    }
}
#endif

#endif
