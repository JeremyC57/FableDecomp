// COpenALVoice::UnqueueBuffer() -- clear the OpenAL error state, unqueue the oldest processed buffer of the voice's source through the global OpenAL wrapper, then advance the buffer ring's read index (wrap at the buffer count) and drop the queued count
// Leaf: ?UnqueueBuffer@COpenALVoice@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C57C90, length 193 bytes (+ int3 padding); `ret`, returns nothing.
// Callees (all through the global OpenAL wrapper g_AudioContext @ 0x0143BDB0, inlined accessors): alGetError (entry
// point +0xE0 = 0x0143BE90, "alGetError" @ 0x012B87E8) and alSourceUnqueueBuffers (+0x90 = 0x0143BE40,
// "alSourceUnqueueBuffers" @ 0x012B8EAC), each resolved on first use through CAudioContext::GetProcAddress 0x00C4D740
// when +0x104 (0x0143BEB4) is set, else through GetProcAddress (IAT 0x0143FE44) on the module at +4 (0x0143BDB4).
// Caller: 0x00C57D80 (alGetSourcei(source, AL_BUFFERS_PROCESSED) then this once per processed buffer, then a replay
// when the source stopped).
// Library: the GameCODA (Sensaura "CODA") OpenAL back end statically linked next to libpk (strings "C_ERR_CODACREATED",
// "/OpenAL32.dll", "/wrap_oal.dll"; no PDB/RTTI): class and member names are descriptive. The manifest label
// "CAudioSource_UnqueueProcessedBuffer" is close; the class follows its own +0x604.. buffer-ring fields.
// Layout of the voice (only what this row reads): the buffer-name array +0x604, the AL source name +0x608, the ring's
// buffer count +0x618, the queued count +0x61C and the read index +0x624.
// Shape: both accessors are the wrapper's inline lazy resolvers applied to the global instance (absolute member
// addresses); the call arguments (source, 1, &buffers[readIndex]) are evaluated before the inline body, so they sit in
// ebx/edi across the resolve, and GetProcAddress is hoisted into ebp once for both resolvers.
extern "C" __declspec(dllimport) void* __stdcall GetProcAddress(void* module, const char* name);   // IAT 0x0143FE44

typedef long (__cdecl* PFN_alGetError)();
typedef void (__cdecl* PFN_alSourceUnqueueBuffers)(unsigned long source, long count, unsigned long* buffers);

// Retail OpenAL wrapper (no RTTI; the global instance at 0x0143BDB0, the landed CAudioContext rows' layout): the
// OpenAL32.dll / wrap_oal.dll module at +4, the lazily resolved entry points, and the flag at +0x104 that routes
// resolution through alGetProcAddress instead of the DLL export table.
struct CAudioContext {
    void* __vftable;                                     // +0x00
    void* Module;                                        // +0x04
    unsigned char _pad0[0x88];
    PFN_alSourceUnqueueBuffers pfnSourceUnqueueBuffers;  // +0x90
    unsigned char _pad1[0x4c];
    PFN_alGetError pfnGetError;                          // +0xe0
    unsigned char _pad2[0x20];
    bool UseALGetProcAddress;                            // +0x104
    void* GetProcAddress(const char* name);              // 0x00C4D740 (resolve through alGetProcAddress)
    long GetError()
    {
        if (!pfnGetError)
            pfnGetError = (PFN_alGetError)(UseALGetProcAddress ? GetProcAddress("alGetError")
                                                               : ::GetProcAddress(Module, "alGetError"));
        return pfnGetError();
    }
    void SourceUnqueueBuffers(unsigned long source, long count, unsigned long* buffers)
    {
        if (!pfnSourceUnqueueBuffers)
            pfnSourceUnqueueBuffers = (PFN_alSourceUnqueueBuffers)(UseALGetProcAddress ? GetProcAddress("alSourceUnqueueBuffers")
                                                                                       : ::GetProcAddress(Module, "alSourceUnqueueBuffers"));
        pfnSourceUnqueueBuffers(source, count, buffers);
    }
};
extern CAudioContext g_AudioContext;                     // 0x0143BDB0

struct COpenALVoice {
    unsigned char  _pad0[0x604];
    unsigned long* m_pBuffers;                           // +0x604 AL buffer names of the streaming ring
    unsigned long  m_source;                             // +0x608 AL source name
    unsigned char  _pad1[0xc];
    unsigned long  m_numBuffers;                         // +0x618
    long           m_queued;                             // +0x61c
    unsigned char  _pad2[4];
    unsigned long  m_readIndex;                          // +0x624 oldest queued buffer
    void UnqueueBuffer();
};

void COpenALVoice::UnqueueBuffer()
{
    g_AudioContext.GetError();
    g_AudioContext.SourceUnqueueBuffers(m_source, 1, &m_pBuffers[m_readIndex]);
    if (++m_readIndex == m_numBuffers)
        m_readIndex = 0;
    --m_queued;
}