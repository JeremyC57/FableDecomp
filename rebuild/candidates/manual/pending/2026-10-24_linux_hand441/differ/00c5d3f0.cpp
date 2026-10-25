// AccumulateBlockSSE2Std(const long* src, long* dst) -- the `ret 8` copy of the SSE2 32-bit block accumulator (dst[i] += src[i] over 128 samples, inline SSE2), byte-identical to the SSE2 mixer's slot-5 kernel 0x00C5D4B0
// Grade: DIFFER(59v86) -- retail_asm candidate: the whole body is inline SSE2 assembly (ebp frame, `mov edi,edi` loop alignment, `movdqa`/`paddd` x8, `ret 8`), the shape of an `__asm` function; emmintrin intrinsics not tried
// Leaf: ?AccumulateBlockSSE2Std@@YGXPBJPAJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5D3F0, length 86 bytes (+ int3 padding); `ret 8`, args at [ebp+8] /
// [ebp+0Ch] (`this`, if any, unused). No reference in the image; twin of 0x00C5D4B0 (same bytes).
// Label: the manifest's "IntBlock128_AddInPlace" is close (propagated); descriptive name, calling convention
// assumed __stdcall (a thiscall member with `this` unused gives the same bytes).
void __stdcall AccumulateBlockSSE2Std(const long* src, long* dst)
{
    for (int i = 0; i < 128; ++i)
        dst[i] += src[i];
}
