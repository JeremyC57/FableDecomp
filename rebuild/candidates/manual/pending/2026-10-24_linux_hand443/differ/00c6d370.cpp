// MixSamplesSSE2(const short* source, unsigned long groupCount, long* accumulator, short gain) -- SSE2 constant-gain mix: for each group of 8 samples accumulator[k] += ((source[k] * gain) >> 16) << 1 (pmulhw, then the word placed in a dword's high half and shifted right 15)
// Grade: DIFFER(318v156) -- retail_asm candidate: SSE2 inline-asm loop (movdqu / pmulhw / punpck / psrad / paddd) over an aligned /GS frame; ours is the scalar model | 1 grade | next: land as retail_asm
// Leaf: ?MixSamplesSSE2@@
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6D370, 156 bytes (+ int3 padding), `ret` (__cdecl).
// Module: libpk's software mixer (descriptive name). The 0x10-byte aligned frame (`push ebx ; mov ebx,esp ; and esp,-16`),
//   the /GS cookie (0x0139C8A8, checked by 0x00BFE9F9) and the 16-byte gain vector built word by word on the frame come
//   from a `__declspec(align(16)) short gains[8]` local that the inline-assembly loop loads.
//   Callers: the SSE2 delay-tap mixers (0x00C6D600 passes source - delay, the count, the accumulator and the tap gain), 0x00C6D85E..
// Retail-asm: 0x00C6D3AC..0x00C6D3FB `movdqa xmm7,[gains] ; movdqu xmm0,[eax] ; pmulhw xmm0,xmm7 ; pxor ; punpcklwd ;
//   punpckhwd ; psrad 0Fh ; paddd [edx] ; movdqa [edx],..` -- an SSE2 inline-assembly loop; the C++ below is its scalar model.
void __cdecl MixSamplesSSE2(const short* source, unsigned long groupCount, long* accumulator, short gain)
{
    __declspec(align(16)) short gains[8];
    for (int k = 0; k < 8; ++k)
        gains[k] = gain;
    // Retail-asm: SSE2 loop over groupCount groups of 8 samples
    for (unsigned long group = 0; group < groupCount; ++group) {
        for (int k = 0; k < 8; ++k)
            accumulator[k] += ((source[k] * gains[k]) >> 16) << 1;
        source += 8;
        accumulator += 8;
    }
}
