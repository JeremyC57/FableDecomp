// CBankProcessor::AddSample(const SSampleHeader* header) -- build the audio format of one bank sample record (wave format tag 1 PCM -> codec 0, 2 ADPCM -> 2, 0x69 Xbox ADPCM -> 1; any other tag logs "Unknown sample format for index <n>" and skips it), convert its loop points from samples to bytes, create the sample through the manager and register it under its index
// Leaf: AddSample
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2E190, length 224 bytes (219 + int3 padding), __fastcall, `ret 4`.
// Caller: the landed 0x00C2E270 (CChildListLoader::AddChildren: one call per item of the bank's sample list, stopping when
//   the log at +8 reports a failure); its owner is the bank processor built at 0x00C2E0C0 and run by 0x00C2EB00.
// Callees: the log's AddMessage @ 0x00C307E0 (a new 12-byte message node registered through the log's slot 9; returns its
//   stream), the libpk stream's `__fastcall operator<<` (text @ 0x00C0DEC0, unsigned @ 0x00C0DF10), CAudioFormat::SamplesToBytes
//   @ 0x00C2D820 (twice), the manager's CreateSample(format, offset) @ 0x00C0E700 (`new` of a 0x24-byte sample, ctor 0x00C1D880),
//   the sample map's Add(log, sample, index) @ 0x00C3B0E0 (logs a duplicate index, else stores an 8-byte node).
// Name: libpk, no PDB; descriptive (the catalog's `C3DAnimationSequence_ProcessSampleFormatChunk` is propagated). The
//   tags are the WAVEFORMATEX ones (WAVE_FORMAT_PCM 1, WAVE_FORMAT_ADPCM 2, WAVE_FORMAT_XBOX_ADPCM 0x69).
// Shape: the format's inline constructor stores the codec word BEFORE the channel word (`mov [esp+1Ah],bx ; mov
//   [esp+18h],cx`), so it assigns in that order in its body (an init list follows declaration order and swaps them); the
//   switch lowers to `dec ; je ; dec ; je ; sub 67h ; je` with the logged default inline and its own epilogue; the loop
//   start clamps branch-free (`setl ; dec ; and`), the end (+1) through ebx; the created sample is a named local, so the
//   index is read after CreateSample returns (an inline call argument loads it first).
struct CDebugStream {                        // libpk message text
    char* m_pBuffer;
    CDebugStream& __fastcall operator<<(const char* text);       // 0x00C0DEC0
    CDebugStream& __fastcall operator<<(unsigned long value);    // 0x00C0DF10
};

struct CLoadLog {                            // the processor's message log / status (+0x30 ok flag)
    CDebugStream& AddMessage();              // 0x00C307E0
};

struct CAudioFormat {                        // 20 bytes (the CAudioFormat of 0x00C2D790..0x00C2D9A0)
    unsigned long  m_length;                // +0x00 data length in bytes
    unsigned long  m_loopStart;             // +0x04 loop start in bytes
    unsigned long  m_loopEnd;               // +0x08 loop end in bytes
    unsigned short m_channels;              // +0x0C
    unsigned short m_codec;                 // +0x0E 0 = PCM, 1 = Xbox ADPCM, 2 = ADPCM
    unsigned long  m_sampleRate;            // +0x10
    CAudioFormat(unsigned long length, unsigned short channels, unsigned long sampleRate)
    {
        m_length = length;
        m_loopStart = 0;
        m_loopEnd = 0;
        m_codec = 0;
        m_channels = channels;
        m_sampleRate = sampleRate;
    }
    unsigned long SamplesToBytes(unsigned long samples);   // 0x00C2D820
};

struct SSampleHeader {                       // one sample record of the bank data
    unsigned long  m_index;                 // +0x00
    unsigned long  m_04;                    // +0x04
    unsigned long  m_length;                // +0x08
    unsigned long  m_dataOffset;            // +0x0C
    unsigned short m_channels;              // +0x10
    unsigned short m_formatTag;             // +0x12
    unsigned long  m_sampleRate;            // +0x14
    long           m_loopStart;             // +0x18 in samples, negative = none
    long           m_loopEnd;               // +0x1C in samples (inclusive), negative = none
};

struct CSampleManager {
    void* CreateSample(const CAudioFormat* format, unsigned long dataOffset);   // 0x00C0E700
};

struct CSampleMap {                          // index -> sample (0x30 bytes)
    void Add(CLoadLog* log, void* sample, unsigned long index);   // 0x00C3B0E0
    unsigned char _body[0x30];
};

struct CBankProcessor {
    void*           m_pSource;              // +0x00
    CSampleManager* m_pManager;             // +0x04
    CLoadLog*       m_pLog;                 // +0x08
    unsigned char   _pad_0x0c[0x30];        // +0x0C the sound map
    CSampleMap      m_samples;              // +0x3C
    void AddSample(const SSampleHeader* header);
};

void CBankProcessor::AddSample(const SSampleHeader* header)
{
    CAudioFormat format(header->m_length, header->m_channels, header->m_sampleRate);
    switch (header->m_formatTag) {
    case 1:    format.m_codec = 0; break;
    case 2:    format.m_codec = 2; break;
    case 0x69: format.m_codec = 1; break;
    default:
        m_pLog->AddMessage() << "Unknown sample format for index " << header->m_index;
        return;
    }
    unsigned long loopStart = header->m_loopStart < 0 ? 0 : header->m_loopStart;
    unsigned long loopEnd = header->m_loopEnd < 0 ? 0 : header->m_loopEnd + 1;
    format.m_loopStart = format.SamplesToBytes(loopStart);
    format.m_loopEnd = format.SamplesToBytes(loopEnd);
    void* sample = m_pManager->CreateSample(&format, header->m_dataOffset);
    m_samples.Add(m_pLog, sample, header->m_index);
}