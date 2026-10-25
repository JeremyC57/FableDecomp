// CSampleData::GetPlaybackData(void** data) -- hand out the sample's playable PCM: the source buffer for stereo samples, else the converted copy (built on first use by MakeStereo, whose error is returned)
// Leaf: ?GetPlaybackData@CSampleData
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C732F0, length 57 bytes, ret 4. Callee: CSampleData::MakeStereo
//   0x00C4DE00 (allocates 2x the source length through the pool 0x00C3C360 and duplicates each sample; -5 when the
//   allocation fails, -14 for an unknown format). No direct callers: the body is also inlined into the OpenAL
//   device's slot 19 (0x00C73340).
// Layout (descriptive; libpk audio, no PDB/RTTI): CSampleData -- source PCM +0, converted PCM +4, flag byte +8
//   (bit 1 = converted copy built), source descriptor +0xC (format kind +0, channel count +4, length +0xC,
//   data +0x18).
// Shape: one `long result = 0` returned once, with the stereo case as the if-arm and the conversion as the
//   else-arm, gives retail's hoisted `xor eax,eax` (the constant 2 is CSE'd into ecx for both the channel compare and the flag test).
struct CSampleSource {
    unsigned long m_format;                // +0x00
    unsigned long m_channels;              // +0x04
    unsigned long m_08;
    unsigned long m_length;                // +0x0C
    unsigned long m_10;
    unsigned long m_14;
    void* m_pData;                         // +0x18
};

struct CSampleData {
    void* m_pSource;                       // +0x00
    void* m_pConverted;                    // +0x04
    unsigned char m_flags;                 // +0x08
    unsigned char _pad09[3];
    CSampleSource* m_pDescriptor;          // +0x0C
    long MakeStereo();                     // 0x00C4DE00
    long GetPlaybackData(void** data);
};

long CSampleData::GetPlaybackData(void** data)
{
    if (m_pDescriptor->m_channels == 2) {
        *data = m_pSource;
        return 0;
    }
    long result = 0;
    if (!(m_flags & 2))
        result = MakeStereo();
    *data = m_pConverted;
    return result;
}