// GF_SSE_MATRIX_ALIGNED_TRANSFORM_TRANSPOSED_ASSUME_W(self, expanded, blocks, src, dest, temp, wwww) -- the hand-written SSE kernel
// of CMatrix4x4::AlignedTransformTransposedAssumeW (0x00A5CB90, ?AlignedTransformTransposedAssumeW@CMatrix4x4@@QBEXPBVC4DVector@@PAV2@KM@Z;
// 16-byte aligned matrix and vectors), isolated as a macro: four vectors per pass, dest[i] = (row1 . v, row2 . v, row3 . v, w)
// with v = (src[i].X, src[i].Y, src[i].Z, w).
// Retail-asm-helper: GF_SSE_MATRIX_ALIGNED_TRANSFORM_TRANSPOSED_ASSUME_W
// Retail-asm: 0x00A5CC1B `mov eax,[ebp-24h]` (the `self` copy of `this` re-read from its frame word) + 0x00A5CC1E `mov esi,[ebp-28h]`
//   + 0x00A5CC21 `mov ecx,[ebp-4]` + 0x00A5CC24 `movaps xmm0,[eax]` .. 0x00A5CCEE `movaps [esi+0F0h],xmm7` (the sixteen matrix
//   entries broadcast to sixteen 4-float rows, fixed xmm4..xmm7 schedule) + 0x00A5CCF5 `mov eax,[ebx+8]` / `mov edx,[ebx+0Ch]` /
//   `mov edi,[ebp-2Ch]` + the loop 0x00A5CD03 `movlps xmm0,[eax]` / `movhps xmm0,[eax+10h]` .. 0x00A5CE04 `dec ecx` /
//   0x00A5CE05 `jne`: movlps / movhps / shufps 88h / 0DDh structure-of-arrays shuffles on fixed registers, every pointer
//   (array addresses included: `lea` to a frame word, then `mov` from it) loaded from a frame word, esi / edi / ecx used
//   without the compiler knowing (the function spills `this` at entry and reloads it after the block). No <xmmintrin.h>
//   spelling loads a local array's address through a frame word or keeps this register schedule.
// Operands: self -- const CMatrix4x4* (16 floats, 16-byte aligned); expanded -- pointer to 16 aligned 4-float rows (written);
//   blocks -- unsigned long, number of 4-vector passes (must be non-zero); src -- const C4DVector* (aligned, 4 * blocks
//   vectors); dest -- C4DVector* (aligned; may equal src); temp -- pointer to 3 aligned 4-float rows (written); wwww -- an
//   aligned 4-float local holding (w, w, w, w). Clobbers eax, ecx, edx, esi, edi, xmm0..xmm7.
// Semantics: for each vector, dest.K = (E_k1*X + E_k2*Y) + (E_k3*Z + E_k4*w) for k = 1..3 (each product and sum one IEEE
//   single-precision op), dest.W = w.
// Portable twin (FABLE_PORTABLE): plain C++; dual-path test test_CMatrix4x4TransformSSE.cpp.
// Use: write the macro call as a statement on a line of its own (an `__asm` statement runs to the end of its source line).
#ifndef FABLE_ENGINE_CMATRIX4X4TRANSFORMSSE_H
#define FABLE_ENGINE_CMATRIX4X4TRANSFORMSSE_H

#if defined(FABLE_PORTABLE) || defined(FABLE_DUAL_PATH_TEST)
namespace fable_portable
{
    inline void MatrixAlignedTransformTransposedAssumeWSSE(const float* self, float* expanded, unsigned long blocks,
                                                           const float* src, float* dest, float* temp, const float* wwww)
    {
        for (int e = 0; e < 16; ++e) for (int k = 0; k < 4; ++k) expanded[e * 4 + k] = self[e];
        do {
            for (int r = 0; r < 3; ++r) {
                for (int k = 0; k < 4; ++k) {
                    float a = expanded[(r * 4 + 0) * 4 + k] * src[k * 4 + 0];
                    float b = expanded[(r * 4 + 1) * 4 + k] * src[k * 4 + 1];
                    float c = expanded[(r * 4 + 2) * 4 + k] * src[k * 4 + 2];
                    float d = expanded[(r * 4 + 3) * 4 + k] * wwww[k];
                    float ab = a + b; float cd = c + d;
                    temp[r * 4 + k] = ab + cd;
                }
            }
            for (int k = 0; k < 4; ++k) {
                dest[k * 4 + 0] = temp[k]; dest[k * 4 + 1] = temp[4 + k]; dest[k * 4 + 2] = temp[8 + k]; dest[k * 4 + 3] = wwww[k & 1];
            }
            src += 16; dest += 16;
        } while (--blocks);
    }
}
#define FABLE_PORTABLE_SSE_MATRIX_ALIGNED_TRANSFORM_TRANSPOSED_ASSUME_W(self, expanded, blocks, src, dest, temp, wwww) \
    fable_portable::MatrixAlignedTransformTransposedAssumeWSSE((const float*)(self), (float*)(expanded), (blocks), \
        (const float*)(src), (float*)(dest), (float*)(temp), (const float*)&(wwww))
#endif

#ifdef FABLE_PORTABLE
#define GF_SSE_MATRIX_ALIGNED_TRANSFORM_TRANSPOSED_ASSUME_W(self, expanded, blocks, src, dest, temp, wwww) \
    FABLE_PORTABLE_SSE_MATRIX_ALIGNED_TRANSFORM_TRANSPOSED_ASSUME_W(self, expanded, blocks, src, dest, temp, wwww)
#else
#define GF_SSE_MATRIX_ALIGNED_TRANSFORM_TRANSPOSED_ASSUME_W(self, expanded, blocks, src, dest, temp, wwww) \
    __asm mov eax, self \
    __asm mov esi, expanded \
    __asm mov ecx, blocks \
    __asm movaps xmm0, [eax] \
    __asm movaps xmm1, [eax+10h] \
    __asm movaps xmm2, [eax+20h] \
    __asm movaps xmm3, [eax+30h] \
    __asm movaps xmm4, xmm0 \
    __asm movaps xmm5, xmm0 \
    __asm movaps xmm6, xmm0 \
    __asm movaps xmm7, xmm0 \
    __asm shufps xmm4, xmm4, 0 \
    __asm shufps xmm5, xmm5, 55h \
    __asm shufps xmm6, xmm6, 0AAh \
    __asm shufps xmm7, xmm7, 0FFh \
    __asm movaps [esi], xmm4 \
    __asm movaps [esi+10h], xmm5 \
    __asm movaps [esi+20h], xmm6 \
    __asm movaps [esi+30h], xmm7 \
    __asm movaps xmm4, xmm1 \
    __asm movaps xmm5, xmm1 \
    __asm movaps xmm6, xmm1 \
    __asm movaps xmm7, xmm1 \
    __asm shufps xmm4, xmm4, 0 \
    __asm shufps xmm5, xmm5, 55h \
    __asm shufps xmm6, xmm6, 0AAh \
    __asm shufps xmm7, xmm7, 0FFh \
    __asm movaps [esi+40h], xmm4 \
    __asm movaps [esi+50h], xmm5 \
    __asm movaps [esi+60h], xmm6 \
    __asm movaps [esi+70h], xmm7 \
    __asm movaps xmm4, xmm2 \
    __asm movaps xmm5, xmm2 \
    __asm movaps xmm6, xmm2 \
    __asm movaps xmm7, xmm2 \
    __asm shufps xmm4, xmm4, 0 \
    __asm shufps xmm5, xmm5, 55h \
    __asm shufps xmm6, xmm6, 0AAh \
    __asm shufps xmm7, xmm7, 0FFh \
    __asm movaps [esi+80h], xmm4 \
    __asm movaps [esi+90h], xmm5 \
    __asm movaps [esi+0A0h], xmm6 \
    __asm movaps [esi+0B0h], xmm7 \
    __asm movaps xmm4, xmm3 \
    __asm movaps xmm5, xmm3 \
    __asm movaps xmm6, xmm3 \
    __asm movaps xmm7, xmm3 \
    __asm shufps xmm4, xmm4, 0 \
    __asm shufps xmm5, xmm5, 55h \
    __asm shufps xmm6, xmm6, 0AAh \
    __asm shufps xmm7, xmm7, 0FFh \
    __asm movaps [esi+0C0h], xmm4 \
    __asm movaps [esi+0D0h], xmm5 \
    __asm movaps [esi+0E0h], xmm6 \
    __asm movaps [esi+0F0h], xmm7 \
    __asm mov eax, src \
    __asm mov edx, dest \
    __asm mov edi, temp \
    __asm gf_sse_transform_transposed_assume_w_next: \
    __asm movlps xmm0, [eax] \
    __asm movhps xmm0, [eax+10h] \
    __asm movaps xmm1, xmm0 \
    __asm movlps xmm4, [eax+20h] \
    __asm movhps xmm4, [eax+30h] \
    __asm shufps xmm0, xmm4, 88h \
    __asm shufps xmm1, xmm4, 0DDh \
    __asm movlps xmm2, [eax+8] \
    __asm movhps xmm2, [eax+18h] \
    __asm movlps xmm4, [eax+28h] \
    __asm movhps xmm4, [eax+38h] \
    __asm shufps xmm2, xmm4, 88h \
    __asm movaps xmm3, wwww \
    __asm movaps xmm4, [esi] \
    __asm mulps xmm4, xmm0 \
    __asm movaps xmm5, [esi+10h] \
    __asm mulps xmm5, xmm1 \
    __asm movaps xmm6, [esi+20h] \
    __asm mulps xmm6, xmm2 \
    __asm movaps xmm7, [esi+30h] \
    __asm mulps xmm7, xmm3 \
    __asm addps xmm4, xmm5 \
    __asm addps xmm6, xmm7 \
    __asm addps xmm4, xmm6 \
    __asm movaps [edi], xmm4 \
    __asm movaps xmm4, [esi+40h] \
    __asm mulps xmm4, xmm0 \
    __asm movaps xmm5, [esi+50h] \
    __asm mulps xmm5, xmm1 \
    __asm movaps xmm6, [esi+60h] \
    __asm mulps xmm6, xmm2 \
    __asm movaps xmm7, [esi+70h] \
    __asm mulps xmm7, xmm3 \
    __asm addps xmm4, xmm5 \
    __asm addps xmm6, xmm7 \
    __asm addps xmm4, xmm6 \
    __asm movaps [edi+10h], xmm4 \
    __asm movaps xmm4, [esi+80h] \
    __asm mulps xmm4, xmm0 \
    __asm movaps xmm5, [esi+90h] \
    __asm mulps xmm5, xmm1 \
    __asm movaps xmm6, [esi+0A0h] \
    __asm mulps xmm6, xmm2 \
    __asm movaps xmm7, [esi+0B0h] \
    __asm mulps xmm7, xmm3 \
    __asm addps xmm4, xmm5 \
    __asm addps xmm6, xmm7 \
    __asm addps xmm4, xmm6 \
    __asm movaps [edi+20h], xmm4 \
    __asm movlps xmm0, [edi] \
    __asm movhps xmm0, [edi+10h] \
    __asm movaps xmm1, xmm0 \
    __asm movlps xmm4, [edi+20h] \
    __asm movhps xmm4, wwww \
    __asm shufps xmm0, xmm4, 88h \
    __asm shufps xmm1, xmm4, 0DDh \
    __asm movlps xmm2, [edi+8] \
    __asm movhps xmm2, [edi+18h] \
    __asm movaps xmm3, xmm2 \
    __asm movlps xmm4, [edi+28h] \
    __asm movhps xmm4, wwww \
    __asm shufps xmm2, xmm4, 88h \
    __asm shufps xmm3, xmm4, 0DDh \
    __asm movaps [edx], xmm0 \
    __asm movaps [edx+10h], xmm1 \
    __asm movaps [edx+20h], xmm2 \
    __asm movaps [edx+30h], xmm3 \
    __asm add eax, 40h \
    __asm add edx, 40h \
    __asm dec ecx \
    __asm jne gf_sse_transform_transposed_assume_w_next
#endif

#endif  // FABLE_ENGINE_CMATRIX4X4TRANSFORMSSE_H
