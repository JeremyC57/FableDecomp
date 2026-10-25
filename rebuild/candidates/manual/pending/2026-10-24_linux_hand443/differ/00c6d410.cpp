// MixSamplesRampSSE2(unsigned short startGain [ecx], const short* source, unsigned long groupCount, long* accumulator, short step) -- SSE2 ramped mix: the gain vector starts at startGain + k*step (k = 0..7) and rises by 8*step per group (paddsw, saturating); each group of 8 samples adds ((source[k] * gain[k]) >> 16) << 1
// Grade: DIFFER(520v240) -- retail_asm candidate: SSE2 inline-asm ramp loop (pmulhw / paddsw) over an aligned /GS frame, and the start gain arrives in ecx (LTCG register argument); ours is the scalar model with a stack argument | 1 grade | next: land as retail_asm; the ecx argument stays an LTCG lane gap
// Leaf: ?MixSamplesRampSSE2@@
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6D410, 240 bytes, `ret` (custom LTCG convention: startGain in ecx, the rest on the
//   stack, caller pops).
// Module: libpk's software mixer (descriptive name). The 0x10-byte aligned frame (`push ebx ; mov ebx,esp ; and esp,-16`),
//   the /GS cookie (0x0139C8A8, checked by 0x00BFE9F9) and the 16-byte gain vector built word by word on the frame come
//   from a `__declspec(align(16)) short gains[8]` local that the inline-assembly loop loads.
//   Callers: 0x00C6D590 (ramp up: `xor ecx,ecx`, step = tap +6), 0x00C6D5C0 (ramp down: cx = tap gain +4, step = -(tap +6)),
//   0x00C6D68F..0x00C6D836 (the other ramp mixers).
// LTCG: the start gain arrives in ecx (`lea edx,[ecx+eax]` before any load of ecx): a link-time register argument that a
//   VC7.1 C++ declaration cannot express (lane gap on top of the asm).
// Retail-asm: 0x00C6D48E..0x00C6D4EF `movdqa xmm7,[gains] ; movdqa xmm6,[steps] ; movdqu xmm0,[eax] ; pmulhw xmm0,xmm7 ;
//   paddsw xmm7,xmm6 ; ... psrad 0Fh ; paddd [edx] ; movdqa [edx],..` -- an SSE2 inline-assembly loop; scalar model below.
void __cdecl MixSamplesRampSSE2(unsigned short startGain, const short* source, unsigned long groupCount, long* accumulator, short step)
{
    __declspec(align(16)) short gains[8];
    __declspec(align(16)) short steps[8];
    for (int k = 0; k < 8; ++k) {
        gains[k] = (short)(startGain + k * step);
        steps[k] = (short)(step * 8);
    }
    // Retail-asm: SSE2 loop over groupCount groups of 8 samples
    for (unsigned long group = 0; group < groupCount; ++group) {
        for (int k = 0; k < 8; ++k) {
            accumulator[k] += ((source[k] * gains[k]) >> 16) << 1;
            long next = gains[k] + steps[k];
            gains[k] = (short)(next > 0x7fff ? 0x7fff : next < -0x8000 ? -0x8000 : next);
        }
        source += 8;
        accumulator += 8;
    }
}
