// AccumulateFloatBlockSSE2(const float* src, long* dst) -- add a 128-sample float block into a 32-bit accumulator block with SSE2, converting each sample to an integer under round-to-nearest (MXCSR rounding forced to nearest for the loop and restored after)
// Grade: DIFFER(42v141) -- retail_asm candidate: the whole body is inline SSE2 assembly (`stmxcsr`/`ldmxcsr` around eight `cvtps2dq` / `paddd` / `movdqa` passes over 64 bytes, ebp frame, `push esi` saving the old rounding bits), the shape of an `__asm` function; emmintrin intrinsics not tried
// Leaf: ?AccumulateFloatBlockSSE2@@YAXPBMPAJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5D2E0, length 141 bytes (+ int3 padding); `ret` (__cdecl, args
// at [ebp+8] / [ebp+0Ch]). Callers: 0x00C5D3DA and the SSE2 mixer's MixAddFloat 0x00C5D490 (0x00C5D49A).
// Label: the manifest's "Math_AccumulateFloatArray" is close (propagated); descriptive name (GameCODA, no PDB).
// Shape (retail): `and eax,0FFFF9FFFh` clears MXCSR.RC (round to nearest), `and esi,6000h` keeps the caller's RC and
// `or ecx,esi` restores it; the loop count is 8 (`mov ecx,8`), 16 samples per pass. C++ reference only.
// C++ stand-in for the retail inline-asm rounding helper (`fld value ; fistp result`, round to nearest even): the
// retail row cannot come from C++ (VC7.1 `(long)x` calls __ftol2, /QIfist emits `fistp qword`).
static long RoundToNearest(float value)
{
    float down = (float)(long)value;
    if (value < 0.0f && down != value)
        down -= 1.0f;
    float frac = value - down;
    long result = (long)down;
    if (frac > 0.5f || (frac == 0.5f && (result & 1)))
        ++result;
    return result;
}

void __cdecl AccumulateFloatBlockSSE2(const float* src, long* dst)
{
    for (int i = 0; i < 128; ++i)
        dst[i] += RoundToNearest(src[i]);
}
