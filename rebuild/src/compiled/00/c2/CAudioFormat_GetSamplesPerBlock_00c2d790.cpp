// CAudioFormat::GetSamplesPerBlock() -- samples in one block of the format: the channel count for PCM, 64 per channel for the two ADPCM codecs, 0 for an unknown codec
// Leaf: GetSamplesPerBlock
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2D790, length 32 bytes (31 + 1 int3 padding), __fastcall, plain `ret`.
// Callers: BytesToSamples @ 0x00C2D800 and SamplesToBytes @ 0x00C2D820 (both landed; they carry this body as a same-TU stand-in).
// Name: libpk, no PDB; the descriptive names follow the landed callers (the catalog's `Shield_GetBlockBonus` is propagated).
// Shape: a `switch` on the codec word: `test eax,eax ; je case0 ; jle default ; cmp eax,2 ; jg default`, the shared
//   case 1/2 arm first, `shl eax,6`.
struct CAudioFormat {                        // the landed BytesToSamples 0x00C2D800 / SamplesToBytes 0x00C2D820 model
    unsigned long  m_length;                // +0x00
    unsigned long  m_loopStart;             // +0x04
    unsigned long  m_loopLength;            // +0x08
    unsigned short m_channels;              // +0x0C
    unsigned short m_codec;                 // +0x0E 0 = PCM, 1/2 = Xbox ADPCM (64 samples in 36 bytes per channel)
    unsigned long GetSamplesPerBlock();
};

unsigned long CAudioFormat::GetSamplesPerBlock()
{
    switch (m_codec) {
    case 0: return m_channels;
    case 1: case 2: return m_channels * 64;
    }
    return 0;
}