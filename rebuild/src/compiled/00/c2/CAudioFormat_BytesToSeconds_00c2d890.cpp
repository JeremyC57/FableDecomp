// CAudioFormat::BytesToSeconds(unsigned long bytes) -- playing time of a byte count: (float)BytesToSamples(bytes) / (float)sample rate (+0x10), both unsigned conversions
// Leaf: BytesToSeconds
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2D890, length 80 bytes (66 + int3 padding), __fastcall, `ret 4`, float.
// Caller: 0x00C2D9A3. Callees: GetBlockAlign @ 0x00C2D7D0, GetSamplesPerBlock @ 0x00C2D790 (BytesToSamples @ 0x00C2D800 inlined).
// Name: libpk, no PDB; descriptive (the catalog's `Traits_CalcBlockDamageReduction` is propagated).
// Shape: `this` stays in ecx and the block count in edx across the calls (same-TU callee bodies); the sample count is
//   homed in the dead argument slot for `fild`, each unsigned conversion with its `jge ; fadd [2^32]` fixup, the rate
//   read straight from the member; `fdivp st(1)`.
struct CAudioFormat {                        // the landed BytesToSamples 0x00C2D800 / SamplesToBytes 0x00C2D820 model
    unsigned long  m_length;                // +0x00
    unsigned long  m_loopStart;             // +0x04
    unsigned long  m_loopLength;            // +0x08
    unsigned short m_channels;              // +0x0C
    unsigned short m_codec;                 // +0x0E 0 = PCM, 1/2 = Xbox ADPCM (64 samples in 36 bytes per channel)
    unsigned long  m_sampleRate;            // +0x10

    unsigned long GetSamplesPerBlock();     // 0x00C2D790
    unsigned long GetBlockAlign();          // 0x00C2D7D0
    unsigned long BytesToSamples(unsigned long bytes)   // out of line at 0x00C2D800, inlined here
    {
        unsigned long blocks = bytes / GetBlockAlign();
        return GetSamplesPerBlock() * blocks;
    }
    float BytesToSeconds(unsigned long bytes);
};

// Same-TU bodies of the two callees (retail's TU, above the caller; byte-identical to 0x00C2D790 / 0x00C2D7D0, as in the
// landed 0x00C2D800 / 0x00C2D820): VC7.1 sees that they touch neither ecx, edx nor the x87 stack.
__declspec(noinline) unsigned long CAudioFormat::GetSamplesPerBlock()
{
    switch (m_codec) {
    case 0: return m_channels;
    case 1: case 2: return m_channels * 64;
    }
    return 0;
}
__declspec(noinline) unsigned long CAudioFormat::GetBlockAlign()
{
    switch (m_codec) {
    case 0: return m_channels * 2;
    case 1: case 2: return m_channels * 36;
    }
    return 0;
}

float CAudioFormat::BytesToSeconds(unsigned long bytes)
{
    return (float)BytesToSamples(bytes) / (float)m_sampleRate;
}