// CVtable_012b8ed4::QueueChunk() -- clear the OpenAL error state, upload the written chunk (+4, +0x610 bytes, format +0x60C, 44100 Hz) into the next ring buffer, queue that buffer on the voice's source, then advance the ring's write index (wrap at the buffer count) and count the buffer as queued
// Leaf: ?QueueChunk@CVtable_012b8ed4@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C57B60, length 292 bytes (+ int3 padding); thiscall, plain `ret`.
// Caller: CVtable_012b8ed4::UnlockChunk 0x00C57D80 (landed; it unqueues the processed buffers, calls this row, then
//   restarts the source), which binds this address as the placeholder `QueueChunk_00c57b60`.
// Callees (all inline lazy resolvers of the global OpenAL wrapper g_AudioContext @ 0x0143BDB0, the landed CAudioContext
//   model): alGetError (+0xE0 = 0x0143BE90, "alGetError" 0x012B87E8), alBufferData (+0x38 = 0x0143BDE8, "alBufferData"
//   0x012B87D8), alSourceQueueBuffers (+0x8C = 0x0143BE3C, "alSourceQueueBuffers" 0x012B8E94); each resolved on first use
//   through CAudioContext::GetProcAddress 0x00C4D740 when +0x104 is set, else GetProcAddress (IAT 0x0143FE44) on +4.
// Identity (descriptive; GameCODA / libpk have no PDB or RTTI): the OpenAL streaming buffer of vtable 0x012B8ED4 (class
//   name of the landed ctor 0x00C57780, IsChunkReady 0x00C57AF0, UnlockChunk 0x00C57D80). It is the queue half of the
//   landed UnqueueBuffer 0x00C57C90 (read index +0x624 there, write index +0x620 here). The layout: chunk bytes +4..+0x604
//   (0x600 = the largest chunk size CreateAndPlay 0x00C57800 stores), buffer names +0x604, source +0x608, AL format
//   +0x60C (0x1101 / 0x1103 from CreateAndPlay), chunk size +0x610, buffer count +0x618, queued count +0x61C.
// Shape: the twin of UnqueueBuffer -- the call arguments of each inline resolver are loaded before its lazy test.
extern "C" __declspec(dllimport) void* __stdcall GetProcAddress(void* module, const char* name);   // IAT 0x0143FE44

typedef long (__cdecl* PFN_alGetError)();
typedef void (__cdecl* PFN_alBufferData)(unsigned long buffer, long format, const void* data, long size, long frequency);
typedef void (__cdecl* PFN_alSourceQueueBuffers)(unsigned long source, long count, const unsigned long* buffers);

// Retail OpenAL wrapper (no RTTI; vtable 0x012B8C18, the global instance at 0x0143BDB0): the landed CAudioContext layout.
struct CAudioContext {
    void*                    __vftable;                  // +0x00
    void*                    Module;                     // +0x04 (0x0143BDB4)
    unsigned char            _pad08[0x30];
    PFN_alBufferData         pfnBufferData;              // +0x38 (0x0143BDE8)
    unsigned char            _pad3c[0x50];
    PFN_alSourceQueueBuffers pfnSourceQueueBuffers;      // +0x8c (0x0143BE3C)
    unsigned char            _pad90[0x50];
    PFN_alGetError           pfnGetError;                // +0xe0 (0x0143BE90)
    unsigned char            _pade4[0x20];
    bool                     UseALGetProcAddress;        // +0x104 (0x0143BEB4)
    void* GetProcAddress(const char* name);              // 0x00C4D740 (resolve through alGetProcAddress)
    long GetError()
    {
        if (!pfnGetError)
            pfnGetError = (PFN_alGetError)(UseALGetProcAddress ? GetProcAddress("alGetError")
                                                               : ::GetProcAddress(Module, "alGetError"));
        return pfnGetError();
    }
    void BufferData(unsigned long buffer, long format, const void* data, long size, long frequency)
    {
        if (!pfnBufferData)
            pfnBufferData = (PFN_alBufferData)(UseALGetProcAddress ? GetProcAddress("alBufferData")
                                                                   : ::GetProcAddress(Module, "alBufferData"));
        pfnBufferData(buffer, format, data, size, frequency);
    }
    void SourceQueueBuffers(unsigned long source, long count, const unsigned long* buffers)
    {
        if (!pfnSourceQueueBuffers)
            pfnSourceQueueBuffers = (PFN_alSourceQueueBuffers)(UseALGetProcAddress ? GetProcAddress("alSourceQueueBuffers")
                                                                                   : ::GetProcAddress(Module, "alSourceQueueBuffers"));
        pfnSourceQueueBuffers(source, count, buffers);
    }
};
extern CAudioContext g_AudioContext;                     // 0x0143BDB0

// The OpenAL streaming buffer (vtable 0x012B8ED4; slot names from the landed rows of this class).
class CVtable_012b8ed4 {
public:
    virtual void v00();
    virtual bool Initialize(const void* device, void* window, bool multithreaded);   // slot 1: 0x00C57460
    virtual void v02();
    virtual unsigned char CreateAndPlay(unsigned long a, unsigned long b);           // slot 3: 0x00C57800
    virtual bool Create(unsigned long a);                                            // slot 4: 0x00C572C0
    virtual void v05();
    virtual bool IsChunkReady();                                                     // slot 6: 0x00C57AF0
    virtual void v07();
    virtual void UnlockChunk(void* chunk);                                           // slot 8: 0x00C57D80
    unsigned char  m_chunk[0x600];                  // +0x004 the chunk being written
    unsigned long* m_pBuffers;                      // +0x604 AL buffer names of the streaming ring
    unsigned long  m_source;                        // +0x608 OpenAL source name
    long           m_format;                        // +0x60c AL format (AL_FORMAT_MONO16 / STEREO16)
    long           m_chunkSize;                     // +0x610 chunk size in bytes
    bool           m_614;                           // +0x614
    bool           m_615;                           // +0x615
    unsigned char  _pad616[2];
    unsigned long  m_bufferCount;                   // +0x618
    unsigned long  m_queuedCount;                   // +0x61c
    unsigned long  m_writeIndex;                    // +0x620 next ring buffer to fill
    unsigned long  m_readIndex;                     // +0x624 oldest queued buffer
    void QueueChunk();
};

void CVtable_012b8ed4::QueueChunk()
{
    g_AudioContext.GetError();
    g_AudioContext.BufferData(m_pBuffers[m_writeIndex], m_format, m_chunk, m_chunkSize, 44100);
    g_AudioContext.SourceQueueBuffers(m_source, 1, &m_pBuffers[m_writeIndex]);
    if (++m_writeIndex == m_bufferCount)
        m_writeIndex = 0;
    ++m_queuedCount;
}