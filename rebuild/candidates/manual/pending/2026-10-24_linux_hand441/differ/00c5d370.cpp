// AccumulateBlockSSE2(const long* src, long* dst) -- add a 128-sample 32-bit block into a 32-bit accumulator block with SSE2 (eight passes of four `movdqa` loads, `paddd` with the destination, `movdqa` stores)
// Grade: DIFFER(57v84) -- retail_asm candidate: the whole body is inline SSE2 assembly (ebp frame, `mov ecx,8`, `mov edi,edi` loop alignment, `movdqa`/`paddd`), the shape of an `__asm` function; emmintrin intrinsics not tried
// Leaf: ?AccumulateBlockSSE2@@YAXPBJPAJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5D370, length 84 bytes; `ret` (__cdecl). No reference in the image
// (the __cdecl copy of the SSE2 mixer kernel 0x00C5D3F0 / 0x00C5D4B0).
// Label: the manifest's "IntBlock128_AddInPlace" is close (propagated); descriptive name (GameCODA, no PDB).
void __cdecl AccumulateBlockSSE2(const long* src, long* dst)
{
    for (int i = 0; i < 128; ++i)
        dst[i] += src[i];
}
