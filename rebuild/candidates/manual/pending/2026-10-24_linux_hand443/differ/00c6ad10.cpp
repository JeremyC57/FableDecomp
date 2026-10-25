// CSoundMixer::MixBlocksRamped(const unsigned char* blocks, unsigned long blockCount, unsigned short totalStart, short totalDelta) -- mark the mixer as mixed and add `blockCount` 256-byte sample blocks into the accumulator (+0x21c) through the SSE2 ramp mixer 0x00C6AAE0, each with the start gain totalStart / blockCount and the step totalDelta / blockCount (unsigned divides), 0x10 groups per block
// Grade: DIFFER(92v89) -- LTCG register argument: retail passes the start gain to 0x00C6AAE0 in ecx (`mov ecx,ebp`) and pops the four stack arguments itself; ours pushes it as a fifth cdecl argument; every other instruction lines up | 1 grade | next: lane gap: LTCG custom calling convention
// Leaf: ?MixBlocksRamped@CSoundMixer
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6AD10, 89 bytes (+ int3 padding), `ret 10h`.
// Module: libpk's software mixer (no PDB/RTTI; descriptive names; the manifest's "_Fill_n<unsigned_long*,...>" is
//   propagated). Caller: 0x00C5C8C8.
// Callee: 0x00C6AAE0 (manifest "Audio_ProcessSamples_IIRFilter", propagated): the SSE2 ramp mixer, the same shape as
//   0x00C6D410 (aligned /GS frame, gains built from the start gain in ecx and the step argument).
// LTCG: retail passes the start gain in ecx (`mov ecx,ebp ; call 0C6AAE0h`) with the other four arguments on the stack
//   and pops them itself (`add esp,10h`): a link-time custom calling convention VC7.1 C++ cannot declare. Ours pushes the
//   start gain as a fifth cdecl argument (3 bytes longer); every other instruction lines up.
void __cdecl MixSamplesRampSSE2(unsigned long startGain, const unsigned char* source, long groupCount, long* accumulator, long step);   // 0x00C6AAE0
struct CSoundMixer {
    unsigned char _pad_0x00[0x08];
    unsigned char m_bMixed;                    // +0x08
    unsigned char _pad_0x09[0x213];
    long*         m_pAccumulator;              // +0x21c
    void MixBlocksRamped(const unsigned char* blocks, unsigned long blockCount, unsigned short totalStart, short totalDelta);
};
void CSoundMixer::MixBlocksRamped(const unsigned char* blocks, unsigned long blockCount, unsigned short totalStart, short totalDelta)
{
    unsigned long start = totalStart / blockCount;
    m_bMixed = 1;
    unsigned long delta = totalDelta / blockCount;
    for (unsigned long i = 0; i < blockCount; ++i) {
        MixSamplesRampSSE2(start, blocks, 0x10, m_pAccumulator, delta);
        blocks += 0x100;
    }
}
