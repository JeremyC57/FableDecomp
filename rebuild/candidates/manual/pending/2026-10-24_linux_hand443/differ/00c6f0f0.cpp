// MixSamplesMMX(const short* source, unsigned long groupCount, long* accumulator, short gain) -- MMX constant-gain mix: for each group of 8 samples (two 4-word halves) accumulator[k] += ((source[k] * gain) >> 16) << 1, then emms
// Grade: DIFFER(296v173) -- retail_asm candidate: MMX inline-asm loop (movq / pmulhw / punpck / psrad / paddd, emms) over an aligned /GS frame; ours is the scalar model | 1 grade | next: land as retail_asm
// Leaf: ?MixSamplesMMX@@
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6F0F0, 173 bytes (+ int3 padding), `ret` (__cdecl).
// Module: libpk's software mixer (descriptive name). The 0x10-byte aligned frame (`push ebx ; mov ebx,esp ; and esp,-16`),
//   the /GS cookie (0x0139C8A8, checked by 0x00BFE9F9) and the 8-byte gain vector built word by word on the frame come
//   from a `__declspec(align(16)) short gains[4]` local that the inline-assembly loop loads.
//   Callers: the MMX delay-tap mixers (0x00C6F3B0 passes source - delay, the count, the accumulator and the tap gain), 0x00C6F60E..
// Retail-asm: 0x00C6F11C..0x00C6F18C `movq mm7,[gains] ; movq mm0,[eax] ; pmulhw mm0,mm7 ; pxor ; punpcklwd ; punpckhwd ;
//   psrad 0Fh ; paddd [edx] ; movq [edx],..` (twice per pass) then `emms` -- an MMX inline-assembly loop; scalar model below.
void __cdecl MixSamplesMMX(const short* source, unsigned long groupCount, long* accumulator, short gain)
{
    __declspec(align(16)) short gains[4];
    for (int k = 0; k < 4; ++k)
        gains[k] = gain;
    // Retail-asm: MMX loop over groupCount groups of 8 samples, then emms
    for (unsigned long group = 0; group < groupCount; ++group) {
        for (int k = 0; k < 8; ++k)
            accumulator[k] += ((source[k] * gains[k & 3]) >> 16) << 1;
        source += 8;
        accumulator += 8;
    }
}
