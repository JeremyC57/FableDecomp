// _global::DecodeAdpcmToPcm(CAudioFormat* format, const void* source, void* dest, unsigned long destSize) -- decodes a codec-1 (Xbox ADPCM) sample into a PCM buffer and rewrites the format record as PCM
// Leaf: DecodeAdpcmToPcm
// Volatile: the record's channel count and sample rate are `volatile` -- retail keeps two self-copies (`mov cx,[esi+0Ch] ; mov [esi+0Ch],cx` at
//   0x00C3466F/0x00C34676 and the +0x10 dword at 0x00C346A8/0x00C346AF) and never hoists a field load above an earlier store to
//   the record; plain C++ members let VC7.1 delete `x = x` and reorder the loads (the parked reference, 122v136, tried that).
//   Labelled volatile per the brief: the lander refuses `volatile`, so this stays in overcap/.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C34630, 136 bytes, __fastcall (format in ecx, source in edx; dest and destSize on
//   the stack, `ret 8`). Callees: BytesToSamples 0x00C2D800, SamplesToBytes 0x00C2D820, DecodeXboxAdpcm 0x00C425E0.
struct CAudioFormat {
    unsigned long           m_length;       // +0x00 bytes
    unsigned long           m_loopStart;    // +0x04 bytes (its load is hoisted above the earlier stores: not volatile)
    unsigned long           m_loopLength;   // +0x08 bytes
    volatile unsigned short m_channels;     // +0x0C (self-copy kept)
    unsigned short          m_codec;        // +0x0E 0 = PCM, 1/2 = ADPCM
    volatile unsigned long  m_sampleRate;   // +0x10 (self-copy kept, loaded before the +8 store)
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
    format->m_channels = format->m_channels;
    format->m_codec = 0;
    format->m_length = written;
    format->m_loopStart = format->SamplesToBytes(format->BytesToSamples(format->m_loopStart));
    format->m_loopLength = format->SamplesToBytes(format->BytesToSamples(format->m_loopLength));
    format->m_sampleRate = format->m_sampleRate;
    return 0;
}
