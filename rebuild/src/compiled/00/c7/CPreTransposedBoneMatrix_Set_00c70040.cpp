// CPreTransposedBoneMatrix::Set(CPreTransposedBoneMatrix* dst, const CMatrix3x4* src) -- copy a 128-byte pre-transposed bone matrix with sixteen MMX quadword load/store pairs (the pre-SSE2 path)
// Leaf: ?Set@CPreTransposedBoneMatrix@@
// Retail-asm: 0x00C70042..0x00C700C8 `mov esi,[esp+10h] ; mov edi,[esp+0Ch] ; movq mm0,[esi] ; movq [edi],mm0 .. movq mm7,[esi+78h] ; movq [edi+78h],mm7 ; emms` -- a hand MMX block copy (35 instructions, no branch: rasm1 family (f)): fixed esi/edi pointers, mm0..mm7 cycled twice, `emms` last, with `push esi ; push edi` saves for exactly the registers the block names; mmintrin intrinsics spell this as `_m_from`/`_mm_stream` pairs that VC7.1 allocates differently (rasm1 probe). The frame is compiler-emitted.
// Helper (2026-11-05 asm migration): the retail inline-asm block is the shared libpk/MixKernelsSIMD.h (byte-matching path + FABLE_PORTABLE twin); the row has
//   no asm of its own. C++/intrinsic attempts: the batch ATTEMPTS.tsv, family SIMD_LIBPK.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C70040, 141 bytes (+3 int3); __cdecl (two stack arguments, plain `ret`): the
//   manifest's "__fastcall" is wrong, ecx is never read. Twin of the SSE2 copy 0x00C70A60.
#include "libpk/MixKernelsSIMD.h"
class CMatrix3x4;
struct CPreTransposedBoneMatrix {
public:
    static void Set(CPreTransposedBoneMatrix* dst, const CMatrix3x4* src);
};
void CPreTransposedBoneMatrix::Set(CPreTransposedBoneMatrix* dst, const CMatrix3x4* src)
{
    LIBPK_MMX_COPY128(src, dst);
}