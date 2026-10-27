// CPreTransposedBoneMatrix::Set(CPreTransposedBoneMatrix* dst, const CMatrix3x4* src) -- copy a 128-byte pre-transposed bone matrix with eight aligned SSE2 loads and stores
// Leaf: ?Set@CPreTransposedBoneMatrix@@
// Retail-asm: 0x00C70A62..0x00C70AB3 `mov esi,[esp+10h] ; mov edi,[esp+0Ch] ; movdqa xmm0,[esi] .. movdqa xmm7,[esi+70h] ; movdqa [edi],xmm0 .. movdqa [edi+70h],xmm7` -- a hand SSE2 block copy (18 instructions, no branch: rasm1 family (f)): fixed esi/edi pointers and all eight xmm registers, with `push esi ; push edi` saves for exactly the registers the block names; emmintrin intrinsics spell this as `_mm_load_si128`/`_mm_store_si128` pairs that VC7.1 allocates differently (rasm1 probe). The frame is compiler-emitted.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C70A60, 91 bytes (+5 int3); __cdecl (two stack arguments, plain `ret`): the
//   manifest's "__fastcall" is wrong, ecx is never read. Twin of the MMX copy 0x00C70040 (the non-SSE2 path).
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
        movdqa  xmm0, [esi]
        movdqa  xmm1, [esi + 10h]
        movdqa  xmm2, [esi + 20h]
        movdqa  xmm3, [esi + 30h]
        movdqa  xmm4, [esi + 40h]
        movdqa  xmm5, [esi + 50h]
        movdqa  xmm6, [esi + 60h]
        movdqa  xmm7, [esi + 70h]
        movdqa  [edi], xmm0
        movdqa  [edi + 10h], xmm1
        movdqa  [edi + 20h], xmm2
        movdqa  [edi + 30h], xmm3
        movdqa  [edi + 40h], xmm4
        movdqa  [edi + 50h], xmm5
        movdqa  [edi + 60h], xmm6
        movdqa  [edi + 70h], xmm7
    }
}