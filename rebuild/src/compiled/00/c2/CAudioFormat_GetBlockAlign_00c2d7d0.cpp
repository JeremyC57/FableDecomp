// CAudioFormat::GetBlockAlign() -- bytes in one block of the format: 2 per channel for 16-bit PCM, 36 per channel for the two ADPCM codecs, 0 for an unknown codec
// Leaf: GetBlockAlign
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2D7D0, length 48 bytes (36 + int3 padding), __fastcall, plain `ret`.
// Callers: BytesToSamples @ 0x00C2D800, SamplesToBytes @ 0x00C2D820 (landed, with this body as a same-TU stand-in) and
//   InventoryItem_CalcBlockDamageReduction @ 0x00C2D970 / 0x00C2D890 (this chunk).
// Name: libpk, no PDB; the descriptive names follow the landed callers (the catalog's `GetCombatDamageModifier` is propagated).
// Shape: a `switch` on the codec word (`test ; je ; jle ; cmp eax,2 ; jg`); `* 36` is `lea eax,[eax+eax*8] ; shl eax,2`,
//   `* 2` is `shl eax,1`.
struct CAudioFormat {                        // the landed BytesToSamples 0x00C2D800 / SamplesToBytes 0x00C2D820 model
    unsigned long  m_length;                // +0x00
    unsigned long  m_loopStart;             // +0x04
    unsigned long  m_loopLength;            // +0x08
    unsigned short m_channels;              // +0x0C
    unsigned short m_codec;                 // +0x0E 0 = PCM, 1/2 = Xbox ADPCM (64 samples in 36 bytes per channel)
    unsigned long GetBlockAlign();
};

unsigned long CAudioFormat::GetBlockAlign()
{
    switch (m_codec) {
    case 0: return m_channels * 2;
    case 1: case 2: return m_channels * 36;
    }
    return 0;
}