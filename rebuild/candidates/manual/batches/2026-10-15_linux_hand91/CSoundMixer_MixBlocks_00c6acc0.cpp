// CSoundMixer::MixBlocks(const unsigned char* blocks, unsigned long blockCount, unsigned short totalVolume) -- splits the volume evenly over the blocks and accumulates each 0x100-byte block of samples into the mixer's accumulator
// Leaf: MixBlocks
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6ACC0, length 73 bytes (+ int3 padding).
// Callee: MixSamplesMMX 0x00C6AA10 (__cdecl; MMX `pmulhw` scale of 16 sample quads by the volume
//         word, added into the 32-bit accumulator: (source, count, accumulator, volume)).
// Catalog label "CTCHeroStats::ApplyVectorScale" is a Ghidra guess in the no-RTTI 0x00C6xxxx module.
// The object is the software mixer reached through the global sound system (0x0143BC2C -> +0x17c,
// caller 0x00C5C8FF passes the channel's 16-bit volume from +0x24a); no class name survives, so the
// class is named after its job. Layout: +0x08 a "something was mixed" byte, +0x21c the accumulator.
// Shape: `div` of the zero-extended 16-bit total by the unsigned block count, the quotient kept in
// ebx and tested as a word (`test bx,bx`); the early return precedes the flag store; the loop counts
// the block count down in ebp (`dec ebp ; jne`) with the source pointer stepping 0x100 bytes.
void __cdecl MixSamplesMMX(const unsigned char* source, long quadCount, long* accumulator, unsigned short volume);   // 0x00C6AA10

struct CSoundMixer {
    unsigned char _pad_0x00[0x08];
    unsigned char m_bMixed;                    // +0x08
    unsigned char _pad_0x09[0x213];
    long*         m_pAccumulator;              // +0x21c
    void MixBlocks(const unsigned char* blocks, unsigned long blockCount, unsigned short totalVolume);
};

void CSoundMixer::MixBlocks(const unsigned char* blocks, unsigned long blockCount, unsigned short totalVolume)
{
    unsigned short volume = (unsigned short)(totalVolume / blockCount);
    if (volume == 0)
        return;
    m_bMixed = 1;
    for (unsigned long i = 0; i < blockCount; ++i) {
        MixSamplesMMX(blocks, 0x10, m_pAccumulator, volume);
        blocks += 0x100;
    }
}
