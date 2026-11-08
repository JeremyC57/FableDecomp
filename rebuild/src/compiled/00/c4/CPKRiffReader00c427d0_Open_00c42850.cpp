// CPKRiffReader_00c427d0::Open(const void* data, unsigned long size) -- open an in-memory RIFF/WAVE image: the base stream open, the RIFF header (length clamp), the 16-byte format chunk (8/4/16-bit only), the data chunk (pointer, size, duration in ms) and an optional single-loop smpl chunk (loop start/end in bytes); the stream is left at the sample data
// Leaf: ?Open@CPKRiffReader_00c427d0@@UAE_NPBXK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C42850, 454 bytes, thiscall `ret 8`, virtual.
// Identity (libpk, no PDB/RTTI; descriptive names): slot 1 of the WAV reader's vtable 0x012B4FAC {0x00C34790 deleting dtor,
//   this row, Close 0x00C426E0, Read 0x00C42720}, which overrides slot 1 of the memory stream's vtable 0x012B4F9C
//   {0x00C34710, 0x00C426C0, 0x00C426E0, 0x00C42720}; the row first calls that base slot directly (0x00C426C0: data +4,
//   position 0, size +0xC, true), so it is the derived `Open(data, size)`. The landed caller ReadChunkExtent 0x00C347B0 binds this
//   address as `Open`; the class is the one the landed chunk walker FindChunk 0x00C427D0 (`CPKRiffReader_00c427d0`) is a member
//   of (it is called on `this` here), deriving from the landed StreamReader (Read 0x00C42720). The catalog's
//   "CCompressorZlib::ParseWaveHeader" is a propagated label; 0x00C426C0's landed CCompressorZlib::SetStreamInput is the
//   base `StreamReader::Open` (NAME_FIXES.tsv).
// Callees: StreamReader::Open 0x00C426C0 (direct), the virtual Read (slot 3), FindChunk 0x00C427D0, __alldiv 0x00F3FC50.
// Layout: +0x04 data, +0x08 position, +0x0C size (StreamReader); +0x10 the WAVEFORMATEX (cbSize +0x20), +0x28 channels,
//   +0x2C sample rate, +0x30 data size, +0x34 / +0x38 loop start / end (bytes), +0x3C data pointer, +0x40 data offset,
//   +0x44 duration (ms).
// Shape: two separate early RIFF tests (their epilogue stays inline, before ebx is pushed); the chunk record lives in a block
//   that ends after the smpl search (a `found` flag and the chunk size carry out), so the sample-loop record reuses its frame
//   slot; the two "no loop" arms each clear the loop pair -- four zero stores before VC7.1 cross-jumps the arms into one
//   block, which keeps 0 in ebx (with the stores written once the zero goes back to immediates); the length clamp is
//   `if (size == 0) ... else if (size > length) fail; else if (...)`.
struct SRiffChunk_00c427d0 {                        // the chunk record FindChunk fills
    unsigned long m_id;                             // +0
    unsigned long m_size;                           // +4
    unsigned long m_offset;                         // +8
};
struct SRiffHeader {
    unsigned long Id;                               // 'RIFF'
    unsigned long Size;
    unsigned long Format;                           // 'WAVE'
};
struct SSampleChunk {                               // the smpl chunk body (0x24 bytes)
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
struct SSampleLoop {                                // one smpl loop record (0x18 bytes)
    unsigned long Identifier;
    unsigned long Type;
    unsigned long Start;                            // +0x08 (sample frames)
    unsigned long End;                              // +0x0c
    unsigned long Fraction;
    unsigned long PlayCount;
};
struct SWaveFormat {                                // WAVEFORMATEX
    unsigned short FormatTag;                       // +0x10
    unsigned short Channels;                        // +0x12
    unsigned long  SamplesPerSec;                   // +0x14
    unsigned long  AvgBytesPerSec;                  // +0x18
    unsigned short BlockAlign;                      // +0x1c
    unsigned short BitsPerSample;                   // +0x1e
    unsigned short Size;                            // +0x20
};

class StreamReader {                                // the in-memory stream (vtable 0x012B4F9C)
public:
    virtual ~StreamReader();                                   // slot 0
    virtual bool Open(const void* data, unsigned long size);   // slot 1 (0x00C426C0)
    virtual void Close();                                      // slot 2 (0x00C426E0)
    virtual bool Read(void* buffer, unsigned long size);       // slot 3 (0x00C42720)
    const unsigned char* m_pData;                   // +0x04
    unsigned long        m_position;                // +0x08
    unsigned long        m_size;                    // +0x0C
};

class CPKRiffReader_00c427d0 : public StreamReader {   // the WAV reader (vtable 0x012B4FAC)
public:
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
    virtual bool Open(const void* data, unsigned long size);
    bool FindChunk(SRiffChunk_00c427d0* pChunk, bool fromStart);   // 0x00C427D0
};

bool CPKRiffReader_00c427d0::Open(const void* data, unsigned long length)
{
    StreamReader::Open(data, length);
    m_size = 12;
    SRiffHeader riff;
    if (!Read(&riff, 12))
        return false;
    if (riff.Id != 0x46464952)                      // 'RIFF'
        return false;
    unsigned long size = riff.Size + 8;
    if (length == 0)
        m_size = size;
    else if (size > length)
        return false;
    else if (size < length)
        m_size = size;
    else
        m_size = length;
    if (riff.Format != 0x45564157)                  // 'WAVE'
        return false;
    unsigned long smplSize;
    bool found = false;
    {
        SRiffChunk_00c427d0 chunk;
        chunk.m_id = 0x20746d66;                    // 'fmt '
        if (!FindChunk(&chunk, true))
            return false;
        if (chunk.m_size < 0x10)
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
        chunk.m_id = 0x61746164;                    // 'data'
        if (!FindChunk(&chunk, true))
            return false;
        m_dataOffset = chunk.m_offset;
        m_dataSize = chunk.m_size;
        m_pWaveData = m_pData + chunk.m_offset;
        m_durationMs = (unsigned long)((__int64)chunk.m_size * 1000 / (__int64)m_format.AvgBytesPerSec);
        chunk.m_id = 0x6c706d73;                    // 'smpl'
        if (FindChunk(&chunk, true)) {
            found = true;
            smplSize = chunk.m_size;
        }
    }
    if (found) {
        if (smplSize < 0x24)
            return false;
        SSampleChunk sample;
        if (!Read(&sample, 0x24))
            return false;
        if (sample.SampleLoops != 0) {
            if (sample.SampleLoops > 1)
                return false;
            if (smplSize < 0x3c)
                return false;
            SSampleLoop loop;
            if (!Read(&loop, 0x18))
                return false;
            m_loopStart = m_format.BlockAlign * loop.Start;
            m_loopEnd = m_format.BlockAlign * loop.End;
            if (m_loopStart > m_loopEnd || m_loopEnd >= m_dataSize)
                return false;
        } else {
            m_loopStart = 0;
            m_loopEnd = 0;
        }
    } else {
        m_loopStart = 0;
        m_loopEnd = 0;
    }
    m_position = m_dataOffset;
    return true;
}