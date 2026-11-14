// CMatrix4x4::AlignedTransformTransposedAssumeW(const C4DVector*, C4DVector*, unsigned long, float) const -- transform count aligned vectors by the transposed matrix with W taken as w (SSE four at a time when available, x87 for the rest)
// Leaf: ?AlignedTransformTransposedAssumeW@CMatrix4x4@@QBEXPBVC4DVector@@PAV2@KM@Z
// Flags: /Zi
// Retail-asm: 0x00A5CC1B `mov eax,[ebp-24h]` + 0x00A5CC24 `movaps xmm0,[eax]` .. 0x00A5CD03 `movlps xmm0,[eax]` .. 0x00A5CE05 `jne` (the GF_SSE_MATRIX_ALIGNED_TRANSFORM_TRANSPOSED_ASSUME_W
//   block of engine/CMatrix4x4TransformSSE.h: every pointer re-read from a frame word, fixed xmm schedule, esi / edi / ecx clobbered behind the compiler); the row has no asm of its own.
// Name: Ego_r.pdb + FableWin.pdb ?AlignedTransformTransposedAssumeW@CMatrix4x4@@QBEXPBVC4DVector@@PAV2@KM@Z; reloc-bound from CParticleSimulation::ApplyDragging 0x00ADF300.
// Flags evidence: /Zi -- the realigned ebx frame copies the return address (0x00A5CB9D `mov ebp,[ebx+4]` / 0x00A5CBA0 `mov [esp+4],ebp`, `sub esp,8`); VC7.1 emits that
//   copy only when the TU carries debug information (measured: without /Zi `push ecx` and no copy, 832v847).
// Shape: x87 remainder = the inline TransformTransposedAssumeW(const C4DVector&, C4DVector&, float) of engine/CMatrix4x4.h; the two empty constructors run through `vector constructor iterator`.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A5CB90, 847 bytes, `ret 10h`, `and esp,-16` frame.
#define FABLE_RETAIL_CMATRIX4X4
#define FABLE_RETAIL_CMATRIX4X4_FROMMATRIX3X4
#define FABLE_RETAIL_CMATRIX4X4_ALIGNEDTRANSFORMTRANSPOSEDASSUMEW
#include "engine/CMatrix4x4.h"
#include "engine/CMatrix4x4TransformSSE.h"
#include "engine/NVectorMath.h"

void CMatrix4x4::AlignedTransformTransposedAssumeW(const C4DVector* src, C4DVector* dest, unsigned long count, float w) const
{
    if (g_SSEAvailable) {
        unsigned long blocks = count >> 2;
        if (blocks != 0) {
            const CMatrix4x4* self = this;
            __declspec(align(16)) CMatrix4x4 expandedRows[4];
            __declspec(align(16)) C4DVector tempRows[4];
            __declspec(align(16)) C4DVector wwww(w, w, w, w);
            CMatrix4x4* expanded = expandedRows;
            C4DVector* temp = tempRows;
            GF_SSE_MATRIX_ALIGNED_TRANSFORM_TRANSPOSED_ASSUME_W(self, expanded, blocks, src, dest, temp, wwww);
            dest += blocks * 4;
            src += blocks * 4;
            count &= 3;
        }
    }
    for (; count != 0; --count) {
        TransformTransposedAssumeW(*src, *dest, w);
        ++dest;
        ++src;
    }
}
