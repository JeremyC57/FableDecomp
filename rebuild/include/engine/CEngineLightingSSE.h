// CEngineLightingSSE.h -- the hand-written SSE vertex-lighting kernels of CEngineLightingManager, isolated as macros:
//   GF_SSE_CALC_SW_LIGHTING_NO_CLIP                       CEngineLightingManager::CalcSWLightingNoClipVector 0x00B44820
//   GF_SSE_CALC_SW_LIGHTING_NO_CLIP_SEPARATE_MAIN_LIGHT   ...::CalcSWLightingNoClipSeparateMainLightVector  0x00B44A60
// Each row keeps its C++ (`count /= 4`, the `this` home) and calls one macro, which names the row's own parameters (an asm
// operand that names a C variable reads its frame slot, as retail's block does).
// Retail-asm-helper: GF_SSE_CALC_SW_LIGHTING_NO_CLIP, GF_SSE_CALC_SW_LIGHTING_NO_CLIP_SEPARATE_MAIN_LIGHT
// Retail-asm: 0x00B44839 `mov esi,0` + 0x00B44853 `movaps xmm0,[eax+esi]` + 0x00B44857 `prefetcht0 [eax+esi+20h]` + 0x00B44867
//   `mulps xmm0,[edx+20h]` .. 0x00B448EA `push ebx ; push edi` (inside the vertex loop, around the light loop) .. 0x00B449AF
//   `rsqrtps xmm1,xmm3` + 0x00B449C4 `rcpps xmm3,xmm3` .. 0x00B44A14 `pop edi ; pop ebx` (0x00B44820); 0x00B44A79 `mov esi,0` +
//   0x00B44A94 `prefetcht0 [eax+esi+20h]` + `rsqrtps` / `rcpps` in the light loop + 0x00B44B55 `push ebx` / 0x00B44C88 `pop
//   ebx` (0x00B44A60). VC7.1 cannot emit these functions from C++ or intrinsics (plain C++: no packed SSE at all; intrinsics:
//   every SIMD function with a stack frame gets `and esp,-16`, loads from C pointers are never folded into `mulps` / `maxps`,
//   the allocator picks its own registers, never pushes inside a loop and zeroes with xor, not `mov esi,0`): the records are
//   rebuild/candidates/manual/naming/2026-11-05_asm_helpers_r2/ATTEMPTS_LIGHTING.tsv (intrinsic and plain-C++ spellings,
//   each with what VC7.1 emitted) and ATTEMPTS_UNIVERSAL.tsv (the realignment and folding probes).
// Operands (the caller's own variables, never expressions): self -- the CEngineLightingManager* (`this`; the block reads the
//   SIMD var block pointer at +0x4690); colours / mainLightColours -- CRGBFloatColour arrays, 16-byte aligned, 4 floats per
//   vertex; positions -- C4DVector array, 16-byte aligned; normalX / Y / Z -- 16-byte aligned float streams; count -- the
//   number of four-vertex quads (already divided by 4; the loop is do-while, so 0 runs 2^32 times, as retail); lights --
//   CEngineLocalLightArray& (a long count, then lights of 0x34 bytes). Clobbers eax, ebx, ecx, edx, esi, edi, xmm0..xmm7
//   (the row's prologue saves ebx / esi / edi, as retail's does).
// Semantics, per vertex lane (the SIMD var block is 17 rows of 4 floats: 0 floor, 1 ceiling, 2..4 a direction X / Y / Z, 5..7
//   a colour R / G / B scaled by k, 8..10 a colour scaled by d, 11..13 a colour added, 14..16 scratch: the quad's positions
//   transposed, written only when the light count is non-zero; each light: X Y Z at +0, R G B at +0x18, the distance-squared
//   scale at +0x28, the bias at +0x2C):
//     d = (nx*dirX + ny*dirY) + nz*dirZ;  t = ny*dirY;  k = (t - t) - d;  d = max(d, floor);  k = min(max(k, floor)^2, ceiling)
//     NoClip:          C = (row8..10*d + row5..7*k) + row11..13
//     SeparateMain:    main = row5..7*k (written to mainLightColours);  C = row8..10*d + row11..13
//     every light:     v = L.pos - P;  q = (vx*vx + vy*vy) + vz*vz;  n = max(((vx*nx + vy*ny) + vz*nz) * rsqrt(q), floor)
//                      a = min(max((rcp(q*L.scale) + L.bias) * n, floor), ceiling);  C += L.colour * a
//     written as R, G, B, B (the fourth float repeats blue) per vertex. max / min are the SSE forms (the second operand when
//     unordered or equal). Every other operation is one IEEE single-precision op under MXCSR (round to nearest by default).
// Portable twin (FABLE_PORTABLE): plain C++ in fable_portable. rsqrtps / rcpps are approximate instructions whose low bits
//   differ between processor families (docs/pipeline/ASM_RULES.md, "Approximate instructions"): the twin computes the exact
//   values 1/sqrt(q) and 1/x (in double, rounded once to float). TOLERANCE: each approximate instruction is within Intel's
//   documented relative error |e| <= 1.5 * 2^-12 of the exact value; the dual-path test checks the asm path against the twin
//   with that bound, composed for an output that went through k approximate instructions ((1 + 1.5*2^-12)^k - 1, plus 8
//   single-precision ulps for the roundings after them), and checks bit-identical results where no approximate instruction
//   reaches the output. Everything else in the twin is identical to the asm. Test: rebuild/tests/portable/test_CEngineLightingSSE.cpp.
// Use: write each macro call as a statement on a line of its own: the retail branch expands to `__asm` statements, and an
//   `__asm` statement runs to the end of its source line (anything after it on that line is read as assembler).
#ifndef FABLE_ENGINE_CENGINELIGHTINGSSE_H
#define FABLE_ENGINE_CENGINELIGHTINGSSE_H

#if defined(FABLE_PORTABLE) || defined(FABLE_DUAL_PATH_TEST)
#include <math.h>
namespace fable_portable
{
    // Tolerance of the approximate SSE reciprocals against the exact twin (Intel: |relative error| <= 1.5 * 2^-12).
    static const double kLightingApproxRelError = 1.5 / 4096.0;
    inline float LightingSseMax(float a, float b) { return a > b ? a : b; }   // maxps: b when unordered or equal
    inline float LightingSseMin(float a, float b) { return a < b ? a : b; }   // minps: b when unordered or equal
    inline float LightingExactRsqrt(float x) { return (float)(1.0 / sqrt((double)x)); }   // rsqrtps, exact
    inline float LightingExactRcp(float x) { return (float)(1.0 / (double)x); }           // rcpps, exact
    // block: the 17-row SIMD var block (rows 14..16 are written); lights: {long count; 13-float lights}; quads: 32-bit count,
    // do-while as retail. mainColours == 0 selects the NoClip kernel, else the SeparateMainLight kernel.
    inline void CalcSWLightingNoClipSSE(float* block, float* colours, float* mainColours, const float* positions,
                                        const float* normalX, const float* normalY, const float* normalZ,
                                        unsigned int quads, const void* lights)
    {
        float* const b = block;
        const float* const floor_ = b + 0;
        const float* const ceiling = b + 4;
        unsigned int q = 0;
        do {
            const float* nx = normalX + 4 * q;
            const float* ny = normalY + 4 * q;
            const float* nz = normalZ + 4 * q;
            const float* p = positions + 16 * q;
            float r[4], g[4], bl[4];
            for (int l = 0; l < 4; ++l) {
                float t0 = nx[l] * b[8 + l];
                float t1 = ny[l] * b[12 + l];
                float t2 = nz[l] * b[16 + l];
                float d = t0 + t1;
                d = d + t2;
                float z = t1 - t1;
                float k = z - d;
                d = LightingSseMax(d, floor_[l]);
                k = LightingSseMax(k, floor_[l]);
                k = k * k;
                k = LightingSseMin(k, ceiling[l]);
                float kR = b[20 + l] * k, kG = b[24 + l] * k, kB = b[28 + l] * k;
                float dR = b[32 + l] * d, dG = b[36 + l] * d, dB = b[40 + l] * d;
                if (mainColours) {
                    mainColours[16 * q + 4 * l + 0] = kR;
                    mainColours[16 * q + 4 * l + 1] = kG;
                    mainColours[16 * q + 4 * l + 2] = kB;
                    mainColours[16 * q + 4 * l + 3] = kB;
                    r[l] = dR + b[44 + l];
                    g[l] = dG + b[48 + l];
                    bl[l] = dB + b[52 + l];
                } else {
                    float sr = dR + kR, sg = dG + kG, sb = dB + kB;
                    r[l] = sr + b[44 + l];
                    g[l] = sg + b[48 + l];
                    bl[l] = sb + b[52 + l];
                }
            }
            const int n = *(const int*)lights;          // a 32-bit count (the light array's long on Win32)
            if (n != 0) {
                for (int l = 0; l < 4; ++l) {
                    b[56 + l] = p[4 * l + 0];           // transposed positions: X row, Y row, Z row
                    b[60 + l] = p[4 * l + 1];
                    b[64 + l] = p[4 * l + 2];
                }
                const float* L = (const float*)((const char*)lights + 4);
                unsigned int left = (unsigned int)n;    // `dec ebx ; jne`: a 32-bit count, do-while
                do {
                    for (int l = 0; l < 4; ++l) {
                        float vx = L[0] - b[56 + l];
                        float vy = L[1] - b[60 + l];
                        float vz = L[2] - b[64 + l];
                        float qd = vx * vx;
                        float qy = vy * vy;
                        qd = qd + qy;
                        float qz = vz * vz;
                        qd = qd + qz;
                        float dx = vx * nx[l], dy = vy * ny[l], dz = vz * nz[l];
                        float dot = dx + dy;
                        dot = dot + dz;
                        dot = dot * LightingExactRsqrt(qd);
                        dot = LightingSseMax(dot, floor_[l]);
                        float a = qd * L[10];
                        a = LightingExactRcp(a);
                        a = a + L[11];
                        a = a * dot;
                        a = LightingSseMax(a, floor_[l]);
                        a = LightingSseMin(a, ceiling[l]);
                        float cr = L[6] * a, cg = L[7] * a, cb = L[8] * a;
                        r[l] = r[l] + cr;
                        g[l] = g[l] + cg;
                        bl[l] = bl[l] + cb;
                    }
                    L += 13;
                } while (--left != 0);
            }
            for (int l = 0; l < 4; ++l) {
                colours[16 * q + 4 * l + 0] = r[l];
                colours[16 * q + 4 * l + 1] = g[l];
                colours[16 * q + 4 * l + 2] = bl[l];
                colours[16 * q + 4 * l + 3] = bl[l];
            }
            ++q;
        } while (--quads != 0);
    }
}
#define FABLE_PORTABLE_SSE_CALC_SW_LIGHTING_NO_CLIP(self, colours, positions, normalX, normalY, normalZ, count, lights) \
    fable_portable::CalcSWLightingNoClipSSE(*(float**)((char*)(self) + 0x4690), (float*)(colours), (float*)0, \
        (const float*)(positions), (const float*)(normalX), (const float*)(normalY), (const float*)(normalZ), \
        (unsigned int)(count), (const void*)&(lights))
#define FABLE_PORTABLE_SSE_CALC_SW_LIGHTING_NO_CLIP_SEPARATE_MAIN_LIGHT(self, colours, mainLightColours, positions, normalX, \
                                                                        normalY, normalZ, count, lights) \
    fable_portable::CalcSWLightingNoClipSSE(*(float**)((char*)(self) + 0x4690), (float*)(colours), (float*)(mainLightColours), \
        (const float*)(positions), (const float*)(normalX), (const float*)(normalY), (const float*)(normalZ), \
        (unsigned int)(count), (const void*)&(lights))
#endif

#ifdef FABLE_PORTABLE
#define GF_SSE_CALC_SW_LIGHTING_NO_CLIP(self, colours, positions, normalX, normalY, normalZ, count, lights) \
    FABLE_PORTABLE_SSE_CALC_SW_LIGHTING_NO_CLIP(self, colours, positions, normalX, normalY, normalZ, count, lights)
#define GF_SSE_CALC_SW_LIGHTING_NO_CLIP_SEPARATE_MAIN_LIGHT(self, colours, mainLightColours, positions, normalX, normalY, \
                                                            normalZ, count, lights) \
    FABLE_PORTABLE_SSE_CALC_SW_LIGHTING_NO_CLIP_SEPARATE_MAIN_LIGHT(self, colours, mainLightColours, positions, normalX, \
                                                                    normalY, normalZ, count, lights)
#else
#define GF_SSE_CALC_SW_LIGHTING_NO_CLIP(self, colours, positions, normalX, normalY, normalZ, count, lights) \
    __asm mov esi, 0 \
    __asm mov eax, self \
    __asm mov edx, dword ptr [eax+4690h] \
    __asm mov ecx, normalZ \
    __asm mov edi, colours \
    __asm mov ebx, count \
    __asm gf_swl_VertexQuadLoop: \
    __asm mov eax, normalX \
    __asm movaps xmm0, xmmword ptr [eax+esi] \
    __asm prefetcht0 byte ptr [eax+esi+20h] \
    __asm mov eax, normalY \
    __asm movaps xmm1, xmmword ptr [eax+esi] \
    __asm prefetcht0 byte ptr [eax+esi+20h] \
    __asm mov eax, normalZ \
    __asm movaps xmm2, xmmword ptr [eax+esi] \
    __asm prefetcht0 byte ptr [eax+esi+20h] \
    __asm mulps xmm0, xmmword ptr [edx+20h] \
    __asm mulps xmm1, xmmword ptr [edx+30h] \
    __asm mulps xmm2, xmmword ptr [edx+40h] \
    __asm addps xmm0, xmm1 \
    __asm addps xmm0, xmm2 \
    __asm subps xmm1, xmm1 \
    __asm subps xmm1, xmm0 \
    __asm maxps xmm0, xmmword ptr [edx] \
    __asm maxps xmm1, xmmword ptr [edx] \
    __asm mulps xmm1, xmm1 \
    __asm minps xmm1, xmmword ptr [edx+10h] \
    __asm movaps xmm2, xmmword ptr [edx+50h] \
    __asm movaps xmm3, xmmword ptr [edx+60h] \
    __asm movaps xmm4, xmmword ptr [edx+70h] \
    __asm mulps xmm2, xmm1 \
    __asm mulps xmm3, xmm1 \
    __asm mulps xmm4, xmm1 \
    __asm movaps xmm5, xmmword ptr [edx+80h] \
    __asm movaps xmm6, xmmword ptr [edx+90h] \
    __asm movaps xmm7, xmmword ptr [edx+0A0h] \
    __asm mulps xmm5, xmm0 \
    __asm mulps xmm6, xmm0 \
    __asm mulps xmm7, xmm0 \
    __asm addps xmm5, xmm2 \
    __asm addps xmm6, xmm3 \
    __asm addps xmm7, xmm4 \
    __asm addps xmm5, xmmword ptr [edx+0B0h] \
    __asm addps xmm6, xmmword ptr [edx+0C0h] \
    __asm addps xmm7, xmmword ptr [edx+0D0h] \
    __asm push ebx \
    __asm push edi \
    __asm mov eax, lights \
    __asm mov ebx, dword ptr [eax] \
    __asm lea edi, [eax+4] \
    __asm or ebx, ebx \
    __asm je gf_swl_LocalLightsDone \
    __asm mov eax, positions \
    __asm movaps xmm0, xmmword ptr [eax+esi*4] \
    __asm shufps xmm0, xmmword ptr [eax+esi*4+10h], 44h \
    __asm movaps xmm1, xmmword ptr [eax+esi*4] \
    __asm shufps xmm1, xmmword ptr [eax+esi*4+10h], 0EEh \
    __asm movaps xmm2, xmmword ptr [eax+esi*4+20h] \
    __asm shufps xmm2, xmmword ptr [eax+esi*4+30h], 44h \
    __asm movaps xmm3, xmmword ptr [eax+esi*4+20h] \
    __asm shufps xmm3, xmmword ptr [eax+esi*4+30h], 0EEh \
    __asm movaps xmm4, xmm0 \
    __asm shufps xmm4, xmm2, 88h \
    __asm shufps xmm0, xmm2, 0DDh \
    __asm shufps xmm1, xmm3, 88h \
    __asm movaps xmmword ptr [edx+0E0h], xmm4 \
    __asm movaps xmmword ptr [edx+0F0h], xmm0 \
    __asm movaps xmmword ptr [edx+100h], xmm1 \
    __asm gf_swl_LocalLightLoop: \
    __asm movss xmm0, dword ptr [edi] \
    __asm movss xmm1, dword ptr [edi+4] \
    __asm movss xmm2, dword ptr [edi+8] \
    __asm shufps xmm0, xmm0, 0 \
    __asm shufps xmm1, xmm1, 0 \
    __asm shufps xmm2, xmm2, 0 \
    __asm subps xmm0, xmmword ptr [edx+0E0h] \
    __asm subps xmm1, xmmword ptr [edx+0F0h] \
    __asm subps xmm2, xmmword ptr [edx+100h] \
    __asm movaps xmm3, xmm0 \
    __asm mulps xmm3, xmm3 \
    __asm movaps xmm4, xmm1 \
    __asm mulps xmm4, xmm4 \
    __asm addps xmm3, xmm4 \
    __asm movaps xmm4, xmm2 \
    __asm mulps xmm4, xmm4 \
    __asm addps xmm3, xmm4 \
    __asm mov eax, normalX \
    __asm mulps xmm0, xmmword ptr [eax+esi] \
    __asm mov eax, normalY \
    __asm mulps xmm1, xmmword ptr [eax+esi] \
    __asm mov eax, normalZ \
    __asm mulps xmm2, xmmword ptr [eax+esi] \
    __asm addps xmm0, xmm1 \
    __asm addps xmm0, xmm2 \
    __asm rsqrtps xmm1, xmm3 \
    __asm mulps xmm0, xmm1 \
    __asm maxps xmm0, xmmword ptr [edx] \
    __asm movss xmm1, dword ptr [edi+28h] \
    __asm shufps xmm1, xmm1, 0 \
    __asm mulps xmm3, xmm1 \
    __asm rcpps xmm3, xmm3 \
    __asm movss xmm1, dword ptr [edi+2Ch] \
    __asm shufps xmm1, xmm1, 0 \
    __asm addps xmm3, xmm1 \
    __asm mulps xmm3, xmm0 \
    __asm maxps xmm3, xmmword ptr [edx] \
    __asm minps xmm3, xmmword ptr [edx+10h] \
    __asm movss xmm0, dword ptr [edi+18h] \
    __asm movss xmm1, dword ptr [edi+1Ch] \
    __asm movss xmm2, dword ptr [edi+20h] \
    __asm shufps xmm0, xmm0, 0 \
    __asm shufps xmm1, xmm1, 0 \
    __asm shufps xmm2, xmm2, 0 \
    __asm mulps xmm0, xmm3 \
    __asm mulps xmm1, xmm3 \
    __asm mulps xmm2, xmm3 \
    __asm addps xmm5, xmm0 \
    __asm addps xmm6, xmm1 \
    __asm addps xmm7, xmm2 \
    __asm add edi, 34h \
    __asm dec ebx \
    __asm jne gf_swl_LocalLightLoop \
    __asm gf_swl_LocalLightsDone: \
    __asm pop edi \
    __asm pop ebx \
    __asm movaps xmm0, xmm5 \
    __asm shufps xmm0, xmm6, 44h \
    __asm shufps xmm5, xmm6, 0EEh \
    __asm movaps xmm2, xmm0 \
    __asm shufps xmm2, xmm7, 8 \
    __asm movaps xmmword ptr [edi], xmm2 \
    __asm shufps xmm0, xmm7, 5Dh \
    __asm movaps xmmword ptr [edi+10h], xmm0 \
    __asm movaps xmm2, xmm5 \
    __asm shufps xmm2, xmm7, 0A8h \
    __asm movaps xmmword ptr [edi+20h], xmm2 \
    __asm shufps xmm5, xmm7, 0FDh \
    __asm movaps xmmword ptr [edi+30h], xmm5 \
    __asm add esi, 10h \
    __asm add edi, 40h \
    __asm dec ebx \
    __asm jne gf_swl_VertexQuadLoop
#define GF_SSE_CALC_SW_LIGHTING_NO_CLIP_SEPARATE_MAIN_LIGHT(self, colours, mainLightColours, positions, normalX, normalY, normalZ, count, lights) \
    __asm mov esi, 0 \
    __asm mov eax, self \
    __asm mov edx, dword ptr [eax+4690h] \
    __asm mov ecx, normalZ \
    __asm mov ebx, count \
    __asm gf_swlsm_VertexQuadLoop: \
    __asm mov eax, normalX \
    __asm movaps xmm0, xmmword ptr [eax+esi] \
    __asm prefetcht0 byte ptr [eax+esi+20h] \
    __asm mov eax, normalY \
    __asm movaps xmm1, xmmword ptr [eax+esi] \
    __asm prefetcht0 byte ptr [eax+esi+20h] \
    __asm mov eax, normalZ \
    __asm movaps xmm2, xmmword ptr [eax+esi] \
    __asm prefetcht0 byte ptr [eax+esi+20h] \
    __asm mulps xmm0, xmmword ptr [edx+20h] \
    __asm mulps xmm1, xmmword ptr [edx+30h] \
    __asm mulps xmm2, xmmword ptr [edx+40h] \
    __asm addps xmm0, xmm1 \
    __asm addps xmm0, xmm2 \
    __asm subps xmm1, xmm1 \
    __asm subps xmm1, xmm0 \
    __asm maxps xmm0, xmmword ptr [edx] \
    __asm maxps xmm1, xmmword ptr [edx] \
    __asm mulps xmm1, xmm1 \
    __asm minps xmm1, xmmword ptr [edx+10h] \
    __asm movaps xmm5, xmmword ptr [edx+50h] \
    __asm movaps xmm6, xmmword ptr [edx+60h] \
    __asm movaps xmm7, xmmword ptr [edx+70h] \
    __asm mulps xmm5, xmm1 \
    __asm mulps xmm6, xmm1 \
    __asm mulps xmm7, xmm1 \
    __asm mov eax, mainLightColours \
    __asm movaps xmm3, xmm5 \
    __asm shufps xmm3, xmm6, 44h \
    __asm shufps xmm5, xmm6, 0EEh \
    __asm movaps xmm2, xmm3 \
    __asm shufps xmm2, xmm7, 8 \
    __asm movaps xmmword ptr [eax+esi*4], xmm2 \
    __asm shufps xmm3, xmm7, 5Dh \
    __asm movaps xmmword ptr [eax+esi*4+10h], xmm3 \
    __asm movaps xmm2, xmm5 \
    __asm shufps xmm2, xmm7, 0A8h \
    __asm movaps xmmword ptr [eax+esi*4+20h], xmm2 \
    __asm shufps xmm5, xmm7, 0FDh \
    __asm movaps xmmword ptr [eax+esi*4+30h], xmm5 \
    __asm movaps xmm5, xmmword ptr [edx+80h] \
    __asm movaps xmm6, xmmword ptr [edx+90h] \
    __asm movaps xmm7, xmmword ptr [edx+0A0h] \
    __asm mulps xmm5, xmm0 \
    __asm mulps xmm6, xmm0 \
    __asm mulps xmm7, xmm0 \
    __asm addps xmm5, xmmword ptr [edx+0B0h] \
    __asm addps xmm6, xmmword ptr [edx+0C0h] \
    __asm addps xmm7, xmmword ptr [edx+0D0h] \
    __asm push ebx \
    __asm mov eax, lights \
    __asm mov ebx, dword ptr [eax] \
    __asm or ebx, ebx \
    __asm je gf_swlsm_LocalLightsDone \
    __asm lea edi, [eax+4] \
    __asm mov eax, positions \
    __asm movaps xmm0, xmmword ptr [eax+esi*4] \
    __asm shufps xmm0, xmmword ptr [eax+esi*4+10h], 44h \
    __asm movaps xmm1, xmmword ptr [eax+esi*4] \
    __asm shufps xmm1, xmmword ptr [eax+esi*4+10h], 0EEh \
    __asm movaps xmm2, xmmword ptr [eax+esi*4+20h] \
    __asm shufps xmm2, xmmword ptr [eax+esi*4+30h], 44h \
    __asm movaps xmm3, xmmword ptr [eax+esi*4+20h] \
    __asm shufps xmm3, xmmword ptr [eax+esi*4+30h], 0EEh \
    __asm prefetcht0 byte ptr [eax+esi*4+40h] \
    __asm prefetcht0 byte ptr [eax+esi*4+60h] \
    __asm movaps xmm4, xmm0 \
    __asm shufps xmm4, xmm2, 88h \
    __asm shufps xmm0, xmm2, 0DDh \
    __asm shufps xmm1, xmm3, 88h \
    __asm movaps xmmword ptr [edx+0E0h], xmm4 \
    __asm movaps xmmword ptr [edx+0F0h], xmm0 \
    __asm movaps xmmword ptr [edx+100h], xmm1 \
    __asm gf_swlsm_LocalLightLoop: \
    __asm movss xmm0, dword ptr [edi] \
    __asm movss xmm1, dword ptr [edi+4] \
    __asm movss xmm2, dword ptr [edi+8] \
    __asm shufps xmm0, xmm0, 0 \
    __asm shufps xmm1, xmm1, 0 \
    __asm shufps xmm2, xmm2, 0 \
    __asm subps xmm0, xmmword ptr [edx+0E0h] \
    __asm subps xmm1, xmmword ptr [edx+0F0h] \
    __asm subps xmm2, xmmword ptr [edx+100h] \
    __asm movaps xmm3, xmm0 \
    __asm mulps xmm3, xmm3 \
    __asm movaps xmm4, xmm1 \
    __asm mulps xmm4, xmm4 \
    __asm addps xmm3, xmm4 \
    __asm movaps xmm4, xmm2 \
    __asm mulps xmm4, xmm4 \
    __asm addps xmm3, xmm4 \
    __asm mov eax, normalX \
    __asm mulps xmm0, xmmword ptr [eax+esi] \
    __asm mov eax, normalY \
    __asm mulps xmm1, xmmword ptr [eax+esi] \
    __asm mov eax, normalZ \
    __asm mulps xmm2, xmmword ptr [eax+esi] \
    __asm addps xmm0, xmm1 \
    __asm addps xmm0, xmm2 \
    __asm rsqrtps xmm1, xmm3 \
    __asm mulps xmm0, xmm1 \
    __asm maxps xmm0, xmmword ptr [edx] \
    __asm movss xmm1, dword ptr [edi+28h] \
    __asm shufps xmm1, xmm1, 0 \
    __asm mulps xmm3, xmm1 \
    __asm rcpps xmm3, xmm3 \
    __asm movss xmm1, dword ptr [edi+2Ch] \
    __asm shufps xmm1, xmm1, 0 \
    __asm addps xmm3, xmm1 \
    __asm mulps xmm3, xmm0 \
    __asm maxps xmm3, xmmword ptr [edx] \
    __asm minps xmm3, xmmword ptr [edx+10h] \
    __asm movss xmm0, dword ptr [edi+18h] \
    __asm movss xmm1, dword ptr [edi+1Ch] \
    __asm movss xmm2, dword ptr [edi+20h] \
    __asm shufps xmm0, xmm0, 0 \
    __asm shufps xmm1, xmm1, 0 \
    __asm shufps xmm2, xmm2, 0 \
    __asm mulps xmm0, xmm3 \
    __asm mulps xmm1, xmm3 \
    __asm mulps xmm2, xmm3 \
    __asm addps xmm5, xmm0 \
    __asm addps xmm6, xmm1 \
    __asm addps xmm7, xmm2 \
    __asm add edi, 34h \
    __asm dec ebx \
    __asm jne gf_swlsm_LocalLightLoop \
    __asm gf_swlsm_LocalLightsDone: \
    __asm pop ebx \
    __asm mov eax, colours \
    __asm movaps xmm0, xmm5 \
    __asm shufps xmm0, xmm6, 44h \
    __asm shufps xmm5, xmm6, 0EEh \
    __asm movaps xmm2, xmm0 \
    __asm shufps xmm2, xmm7, 8 \
    __asm movaps xmmword ptr [eax+esi*4], xmm2 \
    __asm shufps xmm0, xmm7, 5Dh \
    __asm movaps xmmword ptr [eax+esi*4+10h], xmm0 \
    __asm movaps xmm2, xmm5 \
    __asm shufps xmm2, xmm7, 0A8h \
    __asm movaps xmmword ptr [eax+esi*4+20h], xmm2 \
    __asm shufps xmm5, xmm7, 0FDh \
    __asm movaps xmmword ptr [eax+esi*4+30h], xmm5 \
    __asm add esi, 10h \
    __asm dec ebx \
    __asm jne gf_swlsm_VertexQuadLoop
#endif

#endif  // FABLE_ENGINE_CENGINELIGHTINGSSE_H
