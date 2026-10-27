// CPreTransposedBoneMatrix::Set(CPreTransposedBoneMatrix* dst, const CMatrix3x4* src) -- copy a 128-byte pre-transposed bone matrix with sixteen MMX quadword load/store pairs (the pre-SSE2 path)
// Leaf: ?Set@CPreTransposedBoneMatrix@@
// Retail-asm: 0x00C70042..0x00C700C8 `mov esi,[esp+10h] ; mov edi,[esp+0Ch] ; movq mm0,[esi] ; movq [edi],mm0 .. movq mm7,[esi+78h] ; movq [edi+78h],mm7 ; emms` -- a hand MMX block copy (35 instructions, no branch: rasm1 family (f)): fixed esi/edi pointers, mm0..mm7 cycled twice, `emms` last, with `push esi ; push edi` saves for exactly the registers the block names; mmintrin intrinsics spell this as `_m_from`/`_mm_stream` pairs that VC7.1 allocates differently (rasm1 probe). The frame is compiler-emitted.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C70040, 141 bytes (+3 int3); __cdecl (two stack arguments, plain `ret`): the
//   manifest's "__fastcall" is wrong, ecx is never read. Twin of the SSE2 copy 0x00C70A60.
class CMatrix3x4;
struct CPreTransposedBoneMatrix {
public:
    static void Set(CPreTransposedBoneMatrix* dst, const CMatrix3x4* src);
};
void CPreTransposedBoneMatrix::Set(CPreTransposedBoneMatrix* dst, const CMatrix3x4* src)
{
    __asm {
        mov     esi, src
        mov     edi, dst
        movq    mm0, [esi]
        movq    [edi], mm0
        movq    mm1, [esi + 8h]
        movq    [edi + 8h], mm1
        movq    mm2, [esi + 10h]
        movq    [edi + 10h], mm2
        movq    mm3, [esi + 18h]
        movq    [edi + 18h], mm3
        movq    mm4, [esi + 20h]
        movq    [edi + 20h], mm4
        movq    mm5, [esi + 28h]
        movq    [edi + 28h], mm5
        movq    mm6, [esi + 30h]
        movq    [edi + 30h], mm6
        movq    mm7, [esi + 38h]
        movq    [edi + 38h], mm7
        movq    mm0, [esi + 40h]
        movq    [edi + 40h], mm0
        movq    mm1, [esi + 48h]
        movq    [edi + 48h], mm1
        movq    mm2, [esi + 50h]
        movq    [edi + 50h], mm2
        movq    mm3, [esi + 58h]
        movq    [edi + 58h], mm3
        movq    mm4, [esi + 60h]
        movq    [edi + 60h], mm4
        movq    mm5, [esi + 68h]
        movq    [edi + 68h], mm5
        movq    mm6, [esi + 70h]
        movq    [edi + 70h], mm6
        movq    mm7, [esi + 78h]
        movq    [edi + 78h], mm7
        emms
    }
}