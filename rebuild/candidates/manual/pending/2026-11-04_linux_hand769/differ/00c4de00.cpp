// CAudioSoundBuffer::KeepShadowCopy() -- make the OpenAL shadow copy of a mono wave as a stereo buffer: duplicate every 16-bit (format 1) or 8-bit (format 2) sample into a CodaMalloc'd block twice the size (-5 when the allocation fails, -14 for another format), clear the AL error, generate the shadow buffer once under a suspended context (flag 2), upload the stereo data, free the block; on an AL error delete the shadow buffer, clear flag 2 and poison it (0xC0DAF00D); returns 0 or the converted AL error
// Grade: DIFFER(934v929) -- register assignment at the upload error check: retail keeps the converted result in edi and the alGetError import in ebp, ours swaps them, so the flag clear becomes a split load/and/store (`and [eax+8]` read-modify-write in retail, +5 bytes in ours); the rest of the masked diff is relocation noise of the lazy GetProcAddress blocks; 11 graded attempts
// Leaf: ?KeepShadowCopy@CAudioSoundBuffer@@QAEJXZ
// Flags: none
extern "C" __declspec(dllimport) void* __stdcall GetProcAddress(void* module, const char* name);   // IAT 0x0143FE44
extern "C" void* __cdecl CodaMalloc(unsigned long size);   // 0x00C3C360
extern "C" void __cdecl CodaFree(void* block);             // 0x00C3C370

typedef void (__cdecl* PFN_alcSuspendContext)(void* context);
typedef void (__cdecl* PFN_alcProcessContext)(void* context);
typedef void (__cdecl* PFN_alGenBuffers)(long count, unsigned long* buffers);
typedef void (__cdecl* PFN_alDeleteBuffers)(long count, const unsigned long* buffers);
typedef void (__cdecl* PFN_alBufferData)(unsigned long buffer, long format, const void* data, long size, long frequency);
typedef long (__cdecl* PFN_alGetError)();

enum {
    AL_NO_ERROR = 0,
    AL_OUT_OF_MEMORY = 0xA005,
    AL_FORMAT_STEREO8 = 0x1102,
    AL_FORMAT_STEREO16 = 0x1103
};

struct CAudioContext {
    void*                 __vftable;                     // +0x00
    void*                 Module;                        // +0x04
    unsigned char         _pad08[0x1c - 0x08];
    PFN_alcSuspendContext pfnSuspendContext;             // +0x1c
    PFN_alcProcessContext pfnProcessContext;             // +0x20
    unsigned char         _pad24[0x2c - 0x24];
    PFN_alGenBuffers      pfnGenBuffers;                 // +0x2c
    PFN_alDeleteBuffers   pfnDeleteBuffers;              // +0x30
    void*                 pfnOther_34;                   // +0x34
    PFN_alBufferData      pfnBufferData;                 // +0x38
    unsigned char         _pad3c[0xe0 - 0x3c];
    PFN_alGetError        pfnGetError;                   // +0xe0
    unsigned char         _pade4[0x100 - 0xe4];
    void*                 Context;                       // +0x100
    bool                  UseALGetProcAddress;           // +0x104
    unsigned char         _pad105[0x1b0 - 0x105];
    bool                  Suspended;                     // +0x1b0
    void* GetProcAddress(const char* name);              // 0x00C4D740
    long GetError()                                      // 0x00C4D960
    {
        PFN_alGetError pfn = pfnGetError;
        if (!pfn) {
            if (UseALGetProcAddress)
                pfn = (PFN_alGetError)GetProcAddress("alGetError");
            else
                pfn = (PFN_alGetError)::GetProcAddress(Module, "alGetError");
            pfnGetError = pfn;
        }
        return pfn();
    }
    void Suspend()                                       // 0x00C4D790
    {
        if (!pfnSuspendContext)
            pfnSuspendContext = (PFN_alcSuspendContext)::GetProcAddress(Module, "alcSuspendContext");
        pfnSuspendContext(Context);
        Suspended = true;
    }
    void Process()                                       // 0x00C4D7D0
    {
        if (!pfnProcessContext)
            pfnProcessContext = (PFN_alcProcessContext)::GetProcAddress(Module, "alcProcessContext");
        pfnProcessContext(Context);
        Suspended = false;
    }
    void GenBuffers(long count, unsigned long* buffers)  // 0x00C4D870
    {
        if (!pfnGenBuffers)
            pfnGenBuffers = (PFN_alGenBuffers)(UseALGetProcAddress ? GetProcAddress("alGenBuffers")
                                                                   : ::GetProcAddress(Module, "alGenBuffers"));
        pfnGenBuffers(count, buffers);
    }
    void DeleteBuffers(long count, const unsigned long* buffers)   // 0x00C4D8C0
    {
        if (!pfnDeleteBuffers)
            pfnDeleteBuffers = (PFN_alDeleteBuffers)(UseALGetProcAddress ? GetProcAddress("alDeleteBuffers")
                                                                         : ::GetProcAddress(Module, "alDeleteBuffers"));
        pfnDeleteBuffers(count, buffers);
    }
    void BufferData(unsigned long buffer, long format, const void* data, long size, long frequency)   // 0x00C4D910
    {
        if (!pfnBufferData)
            pfnBufferData = (PFN_alBufferData)(UseALGetProcAddress ? GetProcAddress("alBufferData")
                                                                   : ::GetProcAddress(Module, "alBufferData"));
        pfnBufferData(buffer, format, data, size, frequency);
    }
    long CheckError();                                   // 0x00C4DBE0
};
extern CAudioContext g_AudioContext;                     // 0x0143BDB0

static inline long ConvertALError(long error)
{
    if (error != AL_NO_ERROR)
        return error == AL_OUT_OF_MEMORY ? -5 : -9;
    return 0;
}

struct SWaveData {
    long          m_sampleFormat;                   // +0x00 (1: 16-bit, 2: 8-bit)
    unsigned long m_channels;                       // +0x04
    long          m_frequency;                      // +0x08
    unsigned long m_size;                           // +0x0c
    unsigned char _pad10[8];
    const void*   m_pData;                          // +0x18
};

struct CAudioSoundBuffer {
    unsigned long    m_buffer;                      // +0x00: the AL buffer name
    unsigned long    m_shadowBuffer;                // +0x04
    unsigned long    m_flags;                       // +0x08 (1: buffer generated, 2: keep a shadow copy)
    const SWaveData* m_pWave;                       // +0x0c
    long KeepShadowCopy();
};

long CAudioSoundBuffer::KeepShadowCopy()
{
    unsigned char* data = (unsigned char*)CodaMalloc(m_pWave->m_size * 2);
    if (!data)
        return -5;
    long format;
    switch (m_pWave->m_sampleFormat) {
    case 1: {
        const unsigned short* src = (const unsigned short*)m_pWave->m_pData;
        for (unsigned long i = 0; i < m_pWave->m_size / 2; ++i) {
            unsigned short sample = src[i];
            ((unsigned short*)data)[i * 2] = sample;
            ((unsigned short*)data)[i * 2 + 1] = sample;
        }
        format = AL_FORMAT_STEREO16;
        break;
    }
    case 2: {
        const unsigned char* src = (const unsigned char*)m_pWave->m_pData;
        for (unsigned long i = 0; i < m_pWave->m_size; ++i) {
            unsigned char sample = src[i];
            data[i * 2] = sample;
            data[i * 2 + 1] = sample;
        }
        format = AL_FORMAT_STEREO8;
        break;
    }
    default:
        CodaFree(data);
        return -14;
    }
    g_AudioContext.GetError();
    if (!(m_flags & 2)) {
        g_AudioContext.GetError();
        bool wasSuspended = g_AudioContext.Suspended;
        g_AudioContext.Suspend();
        g_AudioContext.GenBuffers(1, &m_shadowBuffer);
        if (!wasSuspended)
            g_AudioContext.Process();
        long result = g_AudioContext.CheckError();
        if (result < 0) {
            CodaFree(data);
            return result;
        }
        m_flags |= 2;
    }
    g_AudioContext.BufferData(m_shadowBuffer, format, data, m_pWave->m_size * 2, m_pWave->m_frequency);
    long result = ConvertALError(g_AudioContext.GetError());
    CodaFree(data);
    if (result < 0) {
        bool wasSuspended = g_AudioContext.Suspended;
        g_AudioContext.Suspend();
        g_AudioContext.DeleteBuffers(1, &m_shadowBuffer);
        if (!wasSuspended)
            g_AudioContext.Process();
        m_flags &= ~2;
        m_shadowBuffer = 0xC0DAF00D;
        return result;
    }
    return 0;
}
