// Behaviour test for COpenALVoice::UnqueueBuffer (0x00C57C90): the lazy resolvers fetch alGetError /
// alSourceUnqueueBuffers once (DLL export path or alGetProcAddress path), the oldest buffer of the ring is
// unqueued, the read index wraps at the buffer count and the queued count drops.
#include <cstdio>
#include <cstring>

typedef long (*PFN_alGetError)();
typedef void (*PFN_alSourceUnqueueBuffers)(unsigned long source, long count, unsigned long* buffers);

static int g_errorCalls = 0, g_unqueueCalls = 0, g_dllResolves = 0, g_alResolves = 0;
static unsigned long g_lastSource = 0, g_lastBuffer = 0; static long g_lastCount = 0;

static long StubGetError() { ++g_errorCalls; return 0; }
static void StubUnqueue(unsigned long source, long count, unsigned long* buffers)
{ ++g_unqueueCalls; g_lastSource = source; g_lastCount = count; g_lastBuffer = *buffers; }

static void* ResolveExport(void* module, const char* name)     // stands in for kernel32 GetProcAddress
{
    (void)module; ++g_dllResolves;
    if (!std::strcmp(name, "alGetError")) return (void*)StubGetError;
    if (!std::strcmp(name, "alSourceUnqueueBuffers")) return (void*)StubUnqueue;
    return 0;
}

struct CAudioContext {
    void* Module;
    PFN_alSourceUnqueueBuffers pfnSourceUnqueueBuffers;
    PFN_alGetError pfnGetError;
    bool UseALGetProcAddress;
    void* GetProcAddress(const char* name) { ++g_alResolves; --g_dllResolves; return ResolveExport(Module, name); }
    long GetError()
    {
        if (!pfnGetError)
            pfnGetError = (PFN_alGetError)(UseALGetProcAddress ? GetProcAddress("alGetError") : ResolveExport(Module, "alGetError"));
        return pfnGetError();
    }
    void SourceUnqueueBuffers(unsigned long source, long count, unsigned long* buffers)
    {
        if (!pfnSourceUnqueueBuffers)
            pfnSourceUnqueueBuffers = (PFN_alSourceUnqueueBuffers)(UseALGetProcAddress ? GetProcAddress("alSourceUnqueueBuffers")
                                                                                       : ResolveExport(Module, "alSourceUnqueueBuffers"));
        pfnSourceUnqueueBuffers(source, count, buffers);
    }
};
static CAudioContext g_AudioContext;

struct COpenALVoice {
    unsigned long* m_pBuffers;
    unsigned long  m_source;
    unsigned long  m_numBuffers;
    long           m_queued;
    unsigned long  m_readIndex;
    void UnqueueBuffer()
    {
        g_AudioContext.GetError();
        g_AudioContext.SourceUnqueueBuffers(m_source, 1, &m_pBuffers[m_readIndex]);
        if (++m_readIndex == m_numBuffers)
            m_readIndex = 0;
        --m_queued;
    }
};

int main()
{
    unsigned long names[3] = { 11, 22, 33 };
    COpenALVoice v; v.m_pBuffers = names; v.m_source = 7; v.m_numBuffers = 3; v.m_queued = 3; v.m_readIndex = 1;
    std::memset(&g_AudioContext, 0, sizeof(g_AudioContext));
    v.UnqueueBuffer();
    int ok = g_errorCalls == 1 && g_unqueueCalls == 1 && g_lastSource == 7 && g_lastCount == 1 && g_lastBuffer == 22
          && v.m_readIndex == 2 && v.m_queued == 2 && g_dllResolves == 2 && g_alResolves == 0;
    v.UnqueueBuffer();                                   // wraps 2 -> 0, resolvers cached
    ok = ok && g_lastBuffer == 33 && v.m_readIndex == 0 && v.m_queued == 1 && g_dllResolves == 2;
    g_AudioContext.pfnGetError = 0; g_AudioContext.pfnSourceUnqueueBuffers = 0; g_AudioContext.UseALGetProcAddress = true;
    v.UnqueueBuffer();                                   // alGetProcAddress path
    ok = ok && g_lastBuffer == 11 && v.m_readIndex == 1 && v.m_queued == 0 && g_alResolves == 2 && g_errorCalls == 3;
    if (!ok) { std::printf("00c57c90_TEST FAIL\n"); return 1; }
    std::printf("00c57c90_TEST PASS\n");
    return 0;
}