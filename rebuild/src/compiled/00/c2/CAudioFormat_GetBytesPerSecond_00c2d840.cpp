// CAudioFormat::GetBytesPerSecond() -- the format's data rate: (float)block align * (float)sample rate / (float)samples per block, all unsigned conversions
// Leaf: ?GetBytesPerSecond@CAudioFormat@@QAEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C2D840, 65 bytes (+15 int3 padding), thiscall, `ret`, float result.
// Caller: 0x00C351CA (stores the rate at +4 of its own object, `fstp [esi+4]`, on the format at +8 of its argument).
// Callees: GetBlockAlign @ 0x00C2D7D0, GetSamplesPerBlock @ 0x00C2D790 (both landed).
// Name: libpk (0x00C08AE0..), no PDB; descriptive. The catalog's `CWeaponDrawState::CalcBlockDamageReduction` is not
//   this class: the body is the landed CAudioFormat family (BytesToSeconds 0x00C2D890, BytesToSamples 0x00C2D800).
// Layout: the landed CAudioFormat model (channels +0x0C, codec +0x0E, sample rate +0x10).
// Shape: `this` stays in ecx across both calls (same-TU callee bodies above, noinline: VC7.1 knows they leave ecx and the
//   x87 stack alone); each call result is homed in the `push ecx` frame word for `fild`, with its `jge ; fadd [2^32]`
//   unsigned fixup, the rate is read straight from the member; `fmulp`, then `fdivp st(1)`.
struct CAudioFormat {                        // the landed BytesToSamples 0x00C2D800 / BytesToSeconds 0x00C2D890 model
    unsigned long  m_length;                // +0x00
    unsigned long  m_loopStart;             // +0x04
    unsigned long  m_loopLength;            // +0x08
    unsigned short m_channels;              // +0x0C
    unsigned short m_codec;                 // +0x0E 0 = PCM, 1/2 = Xbox ADPCM (64 samples in 36 bytes per channel)
    unsigned long  m_sampleRate;            // +0x10

    unsigned long GetSamplesPerBlock();     // 0x00C2D790
    unsigned long GetBlockAlign();          // 0x00C2D7D0
    float GetBytesPerSecond();
};

// Same-TU bodies of the two callees (retail's TU, above the caller; byte-identical to 0x00C2D790 / 0x00C2D7D0, as in the
// landed 0x00C2D890): VC7.1 sees that they touch neither ecx, edx nor the x87 stack.
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

float CAudioFormat::GetBytesPerSecond()
{
    return (float)GetBlockAlign() * (float)m_sampleRate / (float)GetSamplesPerBlock();
}