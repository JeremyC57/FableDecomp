// GF_SSE_ADD_TRIANGLE_NORMAL(positions, normals, index0, index1, index2) -- the hand-written SSE step of
// CEngineClothRenderer::CalculateVertexNormals (0x00BB1F50): the face normal (p2 - p0) x (p1 - p0) of one cloth triangle, added
// to the three corner normals, on 16-byte aligned C4DVector arrays; isolated as a macro (descriptive name: the original block is
// inline in CalculateVertexNormals, no PDB symbol).
// Retail-asm-helper: GF_SSE_ADD_TRIANGLE_NORMAL
// Retail-asm: 0x00BB2079 `mov esi,[esp+20h]` + 0x00BB207D `mov edi,[esp+40h]` + 0x00BB2081..0x00BB208F `mov eax/edx/ecx,[esp+88h/
//   8Ch/90h]` (the positions / normals pointers and the three corner indices re-read from their frame slots, where 0x00BB205E..
//   0x00BB206C stored them before the g_SSEAvailable test while the x87 path keeps them in edx / esi / edi) + `shl eax,4` x3 +
//   0x00BB209F `movaps xmm2,[esi+eax]` .. 0x00BB20B7 `shufps xmm0,xmm0,9` / `shufps xmm1,xmm1,12h` .. 0x00BB20CD `subps xmm0,xmm2` ..
//   0x00BB20ED `movaps [edi+ecx],xmm3` (CEngineClothRenderer::CalculateVertexNormals), in a function with no `and esp,-16` frame.
//   VC7.1 gives every function that holds an SSE value an aligned frame and keeps intrinsic operands in its own registers, never
//   re-reading values it already holds (ASM_RULES attempts: NOTES.md table for this site; the 2026-11-05 round-2 universal
//   proofs, rebuild/candidates/manual/naming/2026-11-05_asm_helpers_r2/ATTEMPTS_UNIVERSAL.tsv): the original block is inline asm.
// Operands: positions, normals -- C4DVector pointers (16-byte aligned arrays); index0..index2 -- unsigned long element indices
//   (each < the array sizes). Clobbers eax, ecx, edx, esi, edi, xmm0..xmm3.
// Semantics: a = p2 - p0, b = p1 - p0 per lane; n = (a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x, a.x*b.x - a.x*b.x),
//   each product and difference one IEEE single-precision op (round to nearest, the MXCSR default); normals[index0], [index1],
//   [index2] += n per lane (lane 3 adds +0, or NaN for non-finite inputs, exactly as the asm).
// Portable twin (FABLE_PORTABLE): plain C++ below; dual-path test test_CEngineClothRendererSSE.cpp (hand-back; for rebuild/tests/portable/).
// Use: write the macro call as a statement on a line of its own: the retail branch expands to `__asm` statements, and an `__asm`
//   statement runs to the end of its source line.
#ifndef FABLE_ENGINE_CENGINECLOTHRENDERERSSE_H
#define FABLE_ENGINE_CENGINECLOTHRENDERERSSE_H

#if defined(FABLE_PORTABLE) || defined(FABLE_DUAL_PATH_TEST)
namespace fable_portable
{
    inline void AddTriangleNormalSSE(const void* positions, void* normals, unsigned long index0, unsigned long index1, unsigned long index2)
    {
        const float* p0 = (const float*)positions + index0 * 4;
        const float* p1 = (const float*)positions + index1 * 4;
        const float* p2 = (const float*)positions + index2 * 4;
        float a[4], b[4], l[4], r[4], n[4];
        for (int k = 0; k < 4; ++k) { a[k] = p2[k] - p0[k]; b[k] = p1[k] - p0[k]; }
        l[0] = a[1] * b[2]; l[1] = a[2] * b[0]; l[2] = a[0] * b[1]; l[3] = a[0] * b[0];
        r[0] = a[2] * b[1]; r[1] = a[0] * b[2]; r[2] = a[1] * b[0]; r[3] = a[0] * b[0];
        for (int k = 0; k < 4; ++k) n[k] = l[k] - r[k];
        float* n0 = (float*)normals + index0 * 4;
        float* n1 = (float*)normals + index1 * 4;
        float* n2 = (float*)normals + index2 * 4;
        float s0[4], s1[4], s2[4];
        for (int k = 0; k < 4; ++k) { s0[k] = n0[k] + n[k]; s1[k] = n1[k] + n[k]; s2[k] = n2[k] + n[k]; }
        for (int k = 0; k < 4; ++k) n0[k] = s0[k];
        for (int k = 0; k < 4; ++k) n1[k] = s1[k];
        for (int k = 0; k < 4; ++k) n2[k] = s2[k];
    }
}
#define FABLE_PORTABLE_SSE_ADD_TRIANGLE_NORMAL(positions, normals, index0, index1, index2) \
    fable_portable::AddTriangleNormalSSE((positions), (normals), (index0), (index1), (index2))
#endif

#ifdef FABLE_PORTABLE
#define GF_SSE_ADD_TRIANGLE_NORMAL(positions, normals, index0, index1, index2) \
    FABLE_PORTABLE_SSE_ADD_TRIANGLE_NORMAL(positions, normals, index0, index1, index2)
#else
#define GF_SSE_ADD_TRIANGLE_NORMAL(positions, normals, index0, index1, index2) \
    __asm mov esi, positions \
    __asm mov edi, normals \
    __asm mov eax, index0 \
    __asm mov edx, index1 \
    __asm mov ecx, index2 \
    __asm shl eax, 4 \
    __asm shl edx, 4 \
    __asm shl ecx, 4 \
    __asm movaps xmm2, [esi + eax] \
    __asm movaps xmm0, [esi + ecx] \
    __asm movaps xmm1, [esi + edx] \
    __asm subps xmm0, xmm2 \
    __asm subps xmm1, xmm2 \
    __asm movaps xmm2, xmm0 \
    __asm movaps xmm3, xmm1 \
    __asm shufps xmm0, xmm0, 9 \
    __asm shufps xmm1, xmm1, 0x12 \
    __asm shufps xmm2, xmm2, 0x12 \
    __asm shufps xmm3, xmm3, 9 \
    __asm mulps xmm0, xmm1 \
    __asm mulps xmm2, xmm3 \
    __asm subps xmm0, xmm2 \
    __asm movaps xmm1, [edi + eax] \
    __asm movaps xmm2, [edi + edx] \
    __asm movaps xmm3, [edi + ecx] \
    __asm addps xmm1, xmm0 \
    __asm addps xmm2, xmm0 \
    __asm addps xmm3, xmm0 \
    __asm movaps [edi + eax], xmm1 \
    __asm movaps [edi + edx], xmm2 \
    __asm movaps [edi + ecx], xmm3
#endif

#endif // FABLE_ENGINE_CENGINECLOTHRENDERERSSE_H
