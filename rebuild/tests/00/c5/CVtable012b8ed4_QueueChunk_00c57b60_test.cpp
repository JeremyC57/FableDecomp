// Behaviour test for 0x00C57B60 CVtable_012b8ed4::QueueChunk: lazy entry-point resolution, alGetError, alBufferData of
// the current ring buffer, alSourceQueueBuffers, write-index wrap and queued-count increment.
#include <cstdio>
#include <cstring>

typedef long (*PFN_alGetError)();
typedef void (*PFN_alBufferData)(unsigned long buffer, long format, const void* data, long size, long frequency);
typedef void (*PFN_alSourceQueueBuffers)(unsigned long source, long count, const unsigned long* buffers);

static int g_errCalls, g_resolveCalls;
static unsigned long g_lastBuffer, g_lastSource; static long g_lastFormat, g_lastSize, g_lastFreq, g_lastCount;
static const void* g_lastData; static unsigned long g_queued[8];
static long StubGetError() { ++g_errCalls; return 0; }
static void StubBufferData(unsigned long b, long f, const void* d, long s, long q)
{ g_lastBuffer = b; g_lastFormat = f; g_lastData = d; g_lastSize = s; g_lastFreq = q; }
static void StubQueue(unsigned long src, long n, const unsigned long* bufs)
{ g_lastSource = src; g_lastCount = n; for (long i = 0; i < n; ++i) g_queued[i] = bufs[i]; }

struct CAudioContext {
    PFN_alBufferData pfnBufferData; PFN_alSourceQueueBuffers pfnSourceQueueBuffers; PFN_alGetError pfnGetError;
    bool UseALGetProcAddress;
    void* GetProcAddress(const char* name)
    {
        ++g_resolveCalls;
        if (!std::strcmp(name, "alGetError")) return (void*)&StubGetError;
        if (!std::strcmp(name, "alBufferData")) return (void*)&StubBufferData;
        if (!std::strcmp(name, "alSourceQueueBuffers")) return (void*)&StubQueue;
        return 0;
    }
    long GetError()
    { if (!pfnGetError) pfnGetError = (PFN_alGetError)GetProcAddress("alGetError"); return pfnGetError(); }
    void BufferData(unsigned long b, long f, const void* d, long s, long q)
    { if (!pfnBufferData) pfnBufferData = (PFN_alBufferData)GetProcAddress("alBufferData"); pfnBufferData(b, f, d, s, q); }
    void SourceQueueBuffers(unsigned long src, long n, const unsigned long* bufs)
    { if (!pfnSourceQueueBuffers) pfnSourceQueueBuffers = (PFN_alSourceQueueBuffers)GetProcAddress("alSourceQueueBuffers"); pfnSourceQueueBuffers(src, n, bufs); }
};
static CAudioContext g_AudioContext;

struct CVtable_012b8ed4 {
    unsigned char m_chunk[0x600]; unsigned long* m_pBuffers; unsigned long m_source; long m_format; long m_chunkSize;
    unsigned long m_bufferCount, m_queuedCount, m_writeIndex, m_readIndex;
    void QueueChunk()
    {
        g_AudioContext.GetError();
        g_AudioContext.BufferData(m_pBuffers[m_writeIndex], m_format, m_chunk, m_chunkSize, 44100);
        g_AudioContext.SourceQueueBuffers(m_source, 1, &m_pBuffers[m_writeIndex]);
        if (++m_writeIndex == m_bufferCount)
            m_writeIndex = 0;
        ++m_queuedCount;
    }
};

int main()
{
    static CVtable_012b8ed4 v;
    unsigned long bufs[3] = { 11, 22, 33 };
    v.m_pBuffers = bufs; v.m_source = 7; v.m_format = 0x1103; v.m_chunkSize = 0x200;
    v.m_bufferCount = 3; v.m_queuedCount = 0; v.m_writeIndex = 1; v.m_readIndex = 0;
    v.QueueChunk();
    int bad = 0;
    if (g_errCalls != 1 || g_lastBuffer != 22 || g_lastFormat != 0x1103 || g_lastData != v.m_chunk || g_lastSize != 0x200
        || g_lastFreq != 44100 || g_lastSource != 7 || g_lastCount != 1 || g_queued[0] != 22) bad = 1;
    if (v.m_writeIndex != 2 || v.m_queuedCount != 1 || g_resolveCalls != 3) bad = 1;
    v.QueueChunk();                                     // wraps to 0, entry points already resolved
    if (v.m_writeIndex != 0 || v.m_queuedCount != 2 || g_lastBuffer != 33 || g_queued[0] != 33 || g_resolveCalls != 3) bad = 1;
    v.QueueChunk();
    if (v.m_writeIndex != 1 || g_lastBuffer != 11 || g_errCalls != 3) bad = 1;
    if (bad) { std::printf("00c57b60_TEST FAIL\n"); return 1; }
    std::printf("00c57b60_TEST PASS\n");
    return 0;
}