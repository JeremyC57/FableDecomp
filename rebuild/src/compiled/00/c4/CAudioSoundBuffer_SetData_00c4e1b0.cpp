// CAudioSoundBuffer::SetData(const SWaveData* wave) -- upload a wave into the OpenAL buffer: at most stereo (-14 otherwise); a buffer not yet generated goes through Create; else clear the AL error, pick the AL format from the sample format and channel count, upload with alBufferData, translate the AL error and, when asked to, keep a shadow copy of the samples
// Leaf: ?SetData@CAudioSoundBuffer@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4E1B0, length 158 bytes (+ int3 padding); `ret 4`.
// Callees: Create 0x00C4DC40 (alGenBuffers path; manifest "CAudioBuffer::Create"), and on the global OpenAL context
// object 0x0143BDB0 this chunk's CAudioContext::GetError 0x00C4D960, BufferData 0x00C4D910 and CheckError 0x00C4DBE0;
// then KeepShadowCopy 0x00C4DE00 (allocates twice the wave size; manifest "CAudioSoundBuffer_TransferPCMData").
// Caller: 0x00C3C7EB. The catalog label "CAudioSoundBuffer_SetupFromStream" is propagated; descriptive name (libpk).
// AL formats: AL_FORMAT_MONO8 0x1100, MONO16 0x1101, STEREO8 0x1102, STEREO16 0x1103 (sample format 1 is 16-bit, 2 is
// 8-bit; any other gives format 0).
// Shape: both wave fields are read after the GetError call and before the format switch (an inline helper taking them
// by value; locals give the same bytes), so the channel count is loaded once inside the `dec eax` chain; the ternaries
// are `setne al ; lea eax,[eax+eax+base]`.

struct CAudioContext {
    long GetError();                                                                                 // 0x00C4D960
    void BufferData(unsigned long buffer, long format, const void* data, long size, long frequency); // 0x00C4D910
    long CheckError();                                                                               // 0x00C4DBE0
};
extern CAudioContext g_audioContext;                // 0x0143BDB0

struct SWaveData {
    long          m_sampleFormat;                   // +0x00 (1: 16-bit, 2: 8-bit)
    unsigned long m_channels;                       // +0x04
    long          m_frequency;                      // +0x08
    long          m_size;                           // +0x0c
    unsigned char _pad10[8];
    const void*   m_pData;                          // +0x18
};

static inline long ToALFormat(long sampleFormat, unsigned long channels)
{
    switch (sampleFormat) {
    case 1:
        return channels == 1 ? 0x1101 : 0x1103;     // AL_FORMAT_MONO16 / AL_FORMAT_STEREO16
    case 2:
        return channels == 1 ? 0x1100 : 0x1102;     // AL_FORMAT_MONO8 / AL_FORMAT_STEREO8
    }
    return 0;
}

struct CAudioSoundBuffer {
    unsigned long    m_buffer;                      // +0x00: the AL buffer name
    unsigned long    m_04;
    unsigned long    m_flags;                       // +0x08 (1: buffer generated, 2: keep a shadow copy)
    const SWaveData* m_pWave;                       // +0x0c
    long Create(const SWaveData* wave);             // 0x00C4DC40
    long KeepShadowCopy();                          // 0x00C4DE00
    long SetData(const SWaveData* wave);
};

long CAudioSoundBuffer::SetData(const SWaveData* wave)
{
    if (wave->m_channels > 2)
        return -14;
    if (!(m_flags & 1))
        return Create(wave);
    g_audioContext.GetError();
    long format = ToALFormat(wave->m_sampleFormat, wave->m_channels);
    g_audioContext.BufferData(m_buffer, format, wave->m_pData, wave->m_size, wave->m_frequency);
    long result = g_audioContext.CheckError();
    if (result >= 0 && (m_flags & 2))
        result = KeepShadowCopy();
    return result;
}