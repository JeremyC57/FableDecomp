// _global::DecodeAdpcmToPcm(CAudioFormat* format, const void* source, void* dest, unsigned long destSize) -- decodes a codec-1 (Xbox ADPCM) sample into a PCM buffer and rewrites the format record as PCM
// Leaf: DecodeAdpcmToPcm
// Grade: RELOCATION_MATCH -- the two retail self-copies (channels word +0x0C, sample rate +0x10) are reads through a
//   `const CAudioFormat& original = *format` view written back through `format`: VC7.1 keeps load+store (and does not
//   hoist across the record stores) where a plain `format->m = format->m` is dropped
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C34630, 136 bytes, ecx = format, edx = source, two stack args (`ret 8`);
//   eax = 0 on success, -1 when the format is not codec 1, the buffer is too small or the decoder wrote nothing.
//   Descriptive name (no PDB covers this library): the catalog's `Thiscall_HeroExperienceBlockParry` is propagated.
//   Callers 0x00C1E44C and 0x00C261A4 both load ecx with the address of a format record and edx with the source data.
// Callees: CAudioFormat::BytesToSamples 0x00C2D800 / SamplesToBytes 0x00C2D820 (landed), the __fastcall decoder 0x00C425E0
//   (ecx = source, edx = source bytes, then channels, dest, dest bytes).
struct CAudioFormat {
    unsigned long  m_length;                // +0x00 bytes
    unsigned long  m_loopStart;             // +0x04 bytes
    unsigned long  m_loopLength;            // +0x08 bytes
    unsigned short m_channels;              // +0x0C
    unsigned short m_codec;                 // +0x0E 0 = PCM, 1/2 = ADPCM
    unsigned long  m_sampleRate;            // +0x10
    unsigned long BytesToSamples(unsigned long bytes);     // 0x00C2D800
    unsigned long SamplesToBytes(unsigned long samples);   // 0x00C2D820
};

long __fastcall DecodeXboxAdpcm(const void* source, unsigned long sourceBytes, unsigned long channels,
                                void* dest, unsigned long destBytes);               // 0x00C425E0

long __fastcall DecodeAdpcmToPcm(CAudioFormat* format, const void* source, void* dest, unsigned long destSize)
{
    if (format->m_codec != 1)
        return -1;
    unsigned long needed = format->BytesToSamples(format->m_length) * 2;
    if (destSize < needed)
        return -1;
    long written = DecodeXboxAdpcm(source, format->m_length, format->m_channels, dest, needed);
    if (written <= 0)
        return -1;
    const CAudioFormat& original = *format;
    format->m_channels = original.m_channels;
    format->m_codec = 0;
    format->m_length = written;
    format->m_loopStart = format->SamplesToBytes(format->BytesToSamples(format->m_loopStart));
    format->m_loopLength = format->SamplesToBytes(format->BytesToSamples(format->m_loopLength));
    format->m_sampleRate = original.m_sampleRate;
    return 0;
}