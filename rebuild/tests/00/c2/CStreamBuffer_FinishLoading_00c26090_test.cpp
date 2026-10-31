// Behaviour test for 0x00C26090 CStreamBuffer::FinishLoading: the body over a fake handle pool (lock counts per
// handle), event set, chunk reader, decoder and queues. Checks the null-sample / failed-load exits (users dropped,
// block unlocked), a plain PCM finish (queue moves, LRU stamp, state 3), the compressed unwrap, the ADPCM decode into
// a new block (old block freed, handle swapped, offset 0, the old user count as the allocation key) and the failures.
#include <cstdio>
#include <cstring>
static int g_lock[8]; static int g_freed[8]; static char g_mem[8][32];
struct CHandleBasedMemoryPool {
    void* GetPointerFromHandle(long h) { return g_mem[h]; }
    void Lock(long h) { g_lock[h]++; }
    void Unlock(long h) { g_lock[h]--; }
    void FreeAllocation(long h) { g_freed[h]++; }
};
struct CSoundEventManager;
static long g_allocResult, g_allocSize, g_allocKey;
struct CSoundEventSet { long AllocateHandle(CSoundEventManager*, long size, long key) { g_allocSize = size; g_allocKey = key; return g_allocResult; } };
struct CStreamBuffer;
static char g_log[64]; static int g_logLen;
struct CStreamBufferQueue { char tag; void Remove(CStreamBuffer*) { g_log[g_logLen++] = '-'; g_log[g_logLen++] = tag; }
                            void Add(CStreamBuffer*) { g_log[g_logLen++] = '+'; g_log[g_logLen++] = tag; } };
struct CSoundEventManager { CSoundEventSet m_eventSet; CStreamBufferQueue m_cachedBuffers, m_idleBuffers, m_loadingBuffers; long m_useStamp; CHandleBasedMemoryPool m_pool; };
struct CSoundFormat { unsigned long Size, r1, r2; unsigned short Channels, Encoding; unsigned long SampleRate;
    long CheckPlayable() { switch (Encoding) { case 1: case 2: return -1; default: return 0; } }
    unsigned long GetPCMBufferSize() { return Size * 4; } };
static long g_chunkResult, g_chunkStart, g_chunkEnd; static void* g_chunkSrc; static unsigned long g_chunkSize;
static long ReadChunkExtent_00c347b0(void* first, void* second, long* outStart, long* outEnd)
{ g_chunkSrc = first; g_chunkSize = (unsigned long)second; *outStart = g_chunkStart; *outEnd = g_chunkEnd; return g_chunkResult; }
static long g_decodeResult; static void* g_decodeSrc; static void* g_decodeDst; static unsigned long g_decodeDstSize;
static long DecodeSoundData_00c34630(CSoundFormat*, void* src, void* dst, unsigned long dstSize)
{ g_decodeSrc = src; g_decodeDst = dst; g_decodeDstSize = dstSize; return g_decodeResult; }
struct CStreamBuffer {
    void* m_pSample; long m_handle, m_dataSize, m_lastUse, m_users; unsigned char m_state; bool m_bCompressed; CSoundFormat m_format;
    long FinishLoading(CSoundEventManager* manager, long success)
    {
        CHandleBasedMemoryPool* pool = &manager->m_pool;
        pool->Unlock(m_handle);
        long users = m_users;
        m_users = 0;
        if (m_pSample == 0 || success == 0)
            return -1;
        if (m_bCompressed) {
            pool->Lock(m_handle);
            void* src = (char*)pool->GetPointerFromHandle(m_handle) + m_dataSize;
            long start;
            long end;
            long result = ReadChunkExtent_00c347b0(src, (void*)m_format.Size, &start, &end);
            pool->Unlock(m_handle);
            if (result < 0)
                return -1;
            m_dataSize += start;
            m_format.Size = end;
            m_bCompressed = false;
        }
        if (m_format.CheckPlayable() < 0) {
            unsigned long pcmSize = m_format.GetPCMBufferSize();
            long handle = manager->m_eventSet.AllocateHandle(manager, pcmSize, users);
            if (handle < 0)
                return -1;
            pool->Lock(m_handle);
            pool->Lock(handle);
            char* src = (char*)pool->GetPointerFromHandle(m_handle) + m_dataSize;
            void* dst = pool->GetPointerFromHandle(handle);
            long result = DecodeSoundData_00c34630(&m_format, src, dst, pcmSize);
            pool->Unlock(handle);
            pool->Unlock(m_handle);
            pool->FreeAllocation(m_handle);
            m_handle = handle;
            m_dataSize = 0;
            if (result < 0)
                return -1;
        }
        manager->m_loadingBuffers.Remove(this);
        m_lastUse = manager->m_useStamp;
        manager->m_useStamp++;
        manager->m_idleBuffers.Add(this);
        manager->m_cachedBuffers.Add(this);
        m_state = 3;
        return 0;
    }
};
static void reset(CStreamBuffer& b) { std::memset(&b, 0, sizeof b); std::memset(g_lock, 0, sizeof g_lock); std::memset(g_freed, 0, sizeof g_freed); g_logLen = 0;
    b.m_pSample = &b; b.m_handle = 1; b.m_users = 3; b.m_state = 1; g_lock[1] = 1; b.m_format.Size = 10; b.m_dataSize = 6; }
int main()
{
    int fails = 0;
    CSoundEventManager m; m.m_cachedBuffers.tag = 'c'; m.m_idleBuffers.tag = 'i'; m.m_loadingBuffers.tag = 'l'; m.m_useStamp = 40;
    CStreamBuffer b;
    reset(b); b.m_pSample = 0;
    if (b.FinishLoading(&m, 1) != -1 || b.m_users != 0 || g_lock[1] != 0 || g_logLen != 0) fails |= 1;
    reset(b);
    if (b.FinishLoading(&m, 0) != -1 || b.m_users != 0 || g_lock[1] != 0) fails |= 2;
    reset(b);
    if (b.FinishLoading(&m, 1) != 0 || b.m_state != 3 || b.m_lastUse != 40 || m.m_useStamp != 41 || g_lock[1] != 0
        || g_logLen != 6 || std::memcmp(g_log, "-l+i+c", 6) != 0 || b.m_handle != 1 || b.m_dataSize != 6) fails |= 4;
    reset(b); b.m_bCompressed = true; g_chunkResult = 0; g_chunkStart = 2; g_chunkEnd = 7;
    if (b.FinishLoading(&m, 1) != 0 || g_chunkSrc != g_mem[1] + 6 || g_chunkSize != 10 || b.m_dataSize != 8
        || b.m_format.Size != 7 || b.m_bCompressed || g_lock[1] != 0) fails |= 8;
    reset(b); b.m_bCompressed = true; g_chunkResult = -1;
    if (b.FinishLoading(&m, 1) != -1 || !b.m_bCompressed || g_lock[1] != 0 || b.m_dataSize != 6) fails |= 16;
    reset(b); b.m_format.Encoding = 1; g_allocResult = 5; g_decodeResult = 0;
    if (b.FinishLoading(&m, 1) != 0 || g_allocSize != 40 || g_allocKey != 3 || g_decodeSrc != g_mem[1] + 6 || g_decodeDst != g_mem[5]
        || g_decodeDstSize != 40 || g_freed[1] != 1 || b.m_handle != 5 || b.m_dataSize != 0 || g_lock[1] != 0 || g_lock[5] != 0 || b.m_state != 3) fails |= 32;
    reset(b); b.m_format.Encoding = 2; g_allocResult = -1;
    if (b.FinishLoading(&m, 1) != -1 || b.m_handle != 1 || g_freed[1] != 0) fails |= 64;
    reset(b); b.m_format.Encoding = 2; g_allocResult = 4; g_decodeResult = -2;
    if (b.FinishLoading(&m, 1) != -1 || b.m_handle != 4 || g_freed[1] != 1 || b.m_dataSize != 0 || b.m_state != 1 || g_logLen != 0) fails |= 128;
    if (fails) { std::printf("00c26090_TEST FAIL %d\n", fails); return 1; }
    std::printf("00c26090_TEST PASS\n");
    return 0;
}