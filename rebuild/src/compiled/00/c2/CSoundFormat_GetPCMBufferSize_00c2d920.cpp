// CSoundFormat::GetPCMBufferSize() -- bytes needed to hold this sound's data decoded to 16-bit PCM at 44.1 kHz (never less than the stored size)
// Leaf: GetPCMBufferSize
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2D920, length 69 bytes (the 80-byte catalog row ends in int3 padding).
// Callees: CSoundFormat::BytesToSamples(unsigned long) @ 0x00C2D800 (bytes / BlockBytes * BlockSamples),
//          CSoundFormat::SamplesToBytes(unsigned long) @ 0x00C2D820 (samples / BlockSamples * BlockBytes);
//          both read Channels (+0x0C) and Encoding (+0x0E: 0 = PCM, 1..2 = 36-byte/64-sample ADPCM blocks).
//          Manifest labels "CTCHeroExperience::AddSuccessfulBlockParry" on both are propagated.
// Label: the manifest's "CTCInventory::Copy" is propagated; the callers (0x00C1E2B8, 0x00C1E40F, 0x00C2614C)
//        sit in the no-RTTI sound module, so the class name is descriptive.
// Layout: a 20-byte format record {Size, two words, Channels, Encoding, SampleRate}; the PCM twin is
//         built in a 0x14-byte frame slot by an inline constructor (zeroes, the channel count copied,
//         Encoding 0, 44100 Hz). The three zero words are initialisers; SampleRate, Channels and Encoding
//         are body assignments in that order (retail stores the rate before the two words; init-list
//         order would store it last).
// Shape: Size is read into a local before the first call so it stays in esi; the result is the
//        `larger ? larger : size` select (`cmp eax,esi ; ja`).

struct CSoundFormat {
    unsigned long  Size;                          // +0x00 data size in bytes
    unsigned long  m_reserved1;                   // +0x04
    unsigned long  m_reserved2;                   // +0x08
    unsigned short Channels;                      // +0x0C
    unsigned short Encoding;                      // +0x0E
    unsigned long  SampleRate;                    // +0x10

    CSoundFormat(unsigned short channels)
        : Size(0), m_reserved1(0), m_reserved2(0) { SampleRate = 44100; Channels = channels; Encoding = 0; }
    unsigned long BytesToSamples(unsigned long bytes);      // 0x00C2D800
    unsigned long SamplesToBytes(unsigned long samples);    // 0x00C2D820
    unsigned long GetPCMBufferSize();
};

unsigned long CSoundFormat::GetPCMBufferSize()
{
    unsigned long size = Size;
    CSoundFormat pcm(Channels);
    unsigned long pcmBytes = pcm.SamplesToBytes(BytesToSamples(size));
    return pcmBytes > size ? pcmBytes : size;
}