// CMixBufferSSE2::MixAdd32(const long* src, long* dst) -- the SSE2 mixer accumulator's slot-5 kernel: add a 128-sample 32-bit block into a channel accumulator (dst[i] += src[i]) with inline SSE2
// Grade: DIFFER(59v86) -- retail_asm candidate: the whole body is inline SSE2 assembly (ebp frame, `mov edi,edi` loop alignment, `movdqa`/`paddd` x8, `ret 8`, `this` unused), the shape of an `__asm` function; emmintrin intrinsics not tried
// Leaf: ?MixAdd32@CMixBufferSSE2@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5D4B0, length 86 bytes (+ int3 padding); `ret 8`. Slot 5 of vtable
// 0x012B5DE0 (0x012B5DF4), the SSE2 variant of CMixBuffer (vtable 0x012B8F38: slot 5 0x00C58000 is the C version);
// byte-identical twin of 0x00C5D3F0 (same reference body).
// Label: the manifest's "IntBlock128_AddInPlace" is propagated; descriptive names (GameCODA, no PDB/RTTI).
class CMixBufferSSE2 {
public:
    void MixAdd32(const long* src, long* dst);
};

void CMixBufferSSE2::MixAdd32(const long* src, long* dst)
{
    for (int i = 0; i < 128; ++i)
        dst[i] += src[i];
}
