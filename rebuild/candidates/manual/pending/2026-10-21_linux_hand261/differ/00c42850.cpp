// CCompressorZlib::ParseWaveHeader(const void*, unsigned long) -- parse an in-memory RIFF/WAVE image: format chunk, data chunk (pointer, size, duration in ms) and an optional single-loop smpl chunk
// Grade: DIFFER(458v454) -- zero register: retail pushes ebx late (separate early-fail epilogue) and keeps 0 in ebx for the length test, cbSize store, __alldiv high word, loop-count test and the loop clears; ours uses immediates
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C42850, 455 bytes (464 with padding), `ret 8`; virtual (vtable 0x012B4FB0, no RTTI).
// Callees: SetStreamInput(void*, unsigned long) 0x00C426C0, the stream's virtual Read(void*, unsigned long) (slot 3,
//   +0x0C = 0x00C42720), FindChunk(SChunk*, bool rewind) 0x00C427D0, __alldiv 0x00F3FC50 (64-bit division helper).
// Label: the catalog names this CCompressorZlib (propagated); the object is the audio layer's in-memory WAV reader
//   (its +4/+8/+0xC words are the SetStreamInput buffer, position and length; the landed 0x00C426C0 uses the same name).
// Layout (retail, local model): +0x04 data, +0x08 read position, +0x0C stream length, +0x10 WAVEFORMATEX (cbSize +0x20),
//   +0x28 channels, +0x2C sample rate, +0x30 data size, +0x34/+0x38 loop start/end (bytes), +0x3C data pointer,
//   +0x40 data offset, +0x44 duration (ms).
// Shape: the chunk header and the sample-loop record share one frame block (sibling scopes); the length clamp is
//   `if (len == 0) ... else if (size > len) fail; else if (size < len) ...`.
struct SChunk {
    unsigned long Id;
    unsigned long Size;
    unsigned long Offset;
};
struct SRiffHeader {
    unsigned long Id;
    unsigned long Size;
    unsigned long Format;
};
struct SSampleChunk {
    unsigned long Manufacturer;
    unsigned long Product;
    unsigned long SamplePeriod;
    unsigned long MIDIUnityNote;
    unsigned long MIDIPitchFraction;
    unsigned long SMPTEFormat;
    unsigned long SMPTEOffset;
    unsigned long SampleLoops;                      // +0x1c
    unsigned long SamplerData;
};
struct SSampleLoop {
    unsigned long Identifier;
    unsigned long Type;
    unsigned long Start;                            // +0x08
    unsigned long End;                              // +0x0c
    unsigned long Fraction;
    unsigned long PlayCount;
};
struct SWaveFormat {
    unsigned short FormatTag;                       // +0x10
    unsigned short Channels;                        // +0x12
    unsigned long  SamplesPerSec;                   // +0x14
    unsigned long  AvgBytesPerSec;                  // +0x18
    unsigned short BlockAlign;                      // +0x1c
    unsigned short BitsPerSample;                   // +0x1e
    unsigned short Size;                            // +0x20
};

struct CCompressorZlib {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual bool Read(void* dest, unsigned long count);         // slot 3 (+0x0C)
    const unsigned char* m_pData;                   // +0x04
    unsigned long        m_position;                // +0x08
    unsigned long        m_length;                  // +0x0C
    SWaveFormat          m_format;                  // +0x10
    unsigned long        m_unknown24;               // +0x24
    unsigned long        m_channels;                // +0x28
    unsigned long        m_sampleRate;              // +0x2c
    unsigned long        m_dataSize;                // +0x30
    unsigned long        m_loopStart;               // +0x34
    unsigned long        m_loopEnd;                 // +0x38
    const unsigned char* m_pWaveData;               // +0x3c
    unsigned long        m_dataOffset;              // +0x40
    unsigned long        m_durationMs;              // +0x44
    bool SetStreamInput(const void* data, unsigned long length);   // 0x00C426C0
    bool FindChunk(SChunk* chunk, bool rewind);     // 0x00C427D0
    virtual bool ParseWaveHeader(const void* data, unsigned long length);
};

bool CCompressorZlib::ParseWaveHeader(const void* data, unsigned long length)
{
    SetStreamInput(data, length);
    m_length = 12;
    SRiffHeader riff;
    if (!Read(&riff, 12) || riff.Id != 0x46464952)
        return false;
    unsigned long size = riff.Size + 8;
    if (length == 0)
        m_length = size;
    else if (size > length)
        return false;
    else if (size < length)
        m_length = size;
    else
        m_length = length;
    if (riff.Format != 0x45564157)
        return false;
    unsigned long smplSize;
    {
        SChunk chunk;
        chunk.Id = 0x20746d66;
        if (!FindChunk(&chunk, true))
            return false;
        if (chunk.Size < 0x10)
            return false;
        if (!Read(&m_format, 0x10))
            return false;
        unsigned short bits = m_format.BitsPerSample;
        unsigned short channels = m_format.Channels;
        unsigned long rate = m_format.SamplesPerSec;
        m_format.Size = 0;
        if (bits != 8 && bits != 4 && bits != 16)
            return false;
        m_channels = channels;
        m_sampleRate = rate;
        chunk.Id = 0x61746164;
        if (!FindChunk(&chunk, true))
            return false;
        m_dataOffset = chunk.Offset;
        m_dataSize = chunk.Size;
        m_pWaveData = m_pData + chunk.Offset;
        m_durationMs = (unsigned long)((__int64)chunk.Size * 1000 / (__int64)m_format.AvgBytesPerSec);
        chunk.Id = 0x6c706d73;
        if (!FindChunk(&chunk, true))
            goto noLoop;
        smplSize = chunk.Size;
    }
    if (smplSize < 0x24)
        return false;
    SSampleChunk sample;
    if (!Read(&sample, 0x24))
        return false;
    if (sample.SampleLoops == 0)
        goto noLoop;
    if (sample.SampleLoops > 1)
        return false;
    if (smplSize < 0x3c)
        return false;
    {
        SSampleLoop loop;
        if (!Read(&loop, 0x18))
            return false;
        m_loopStart = m_format.BlockAlign * loop.Start;
        m_loopEnd = m_format.BlockAlign * loop.End;
        if (m_loopStart > m_loopEnd || m_loopEnd >= m_dataSize)
            return false;
    }
    m_position = m_dataOffset;
    return true;
noLoop:
    m_loopStart = 0;
    m_loopEnd = 0;
    m_position = m_dataOffset;
    return true;
}
