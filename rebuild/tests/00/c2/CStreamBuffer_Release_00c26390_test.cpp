// behaviour test for 0x00C26390 CStreamBuffer::Release: dequeue by state, free the pool block, drop the sample
#include <cstdio>

struct SSampleFormat {
    long m_dataSize, m_04, m_08;
    unsigned short m_channels, m_0e;
    long m_sampleRate;
};

static int g_lockCalls = 0, g_unlockCalls = 0, g_freeCalls = 0;
static long g_lastHandle = 0;

struct CSoundSample {
    long           m_header;
    SSampleFormat  m_format;
    unsigned short m_refCount;
    bool           m_looping;
    long           m_size;
    virtual bool IsLooping() { return m_looping; }
    virtual long GetHeaderSize() { return m_header; }
    void AddRef() { ++m_refCount; }
    void Release() { --m_refCount; }
    long GetDataSize() { return m_size; }
};

struct CStreamBuffer;

struct CStreamBufferQueue {
    CStreamBuffer* m_items[8];
    int m_count, m_adds, m_removes;
    CStreamBufferQueue() : m_count(0), m_adds(0), m_removes(0) {}
    bool Has(CStreamBuffer* b) const { for (int i = 0; i < m_count; ++i) if (m_items[i] == b) return true; return false; }
    virtual void Remove(CStreamBuffer* b) { ++m_removes; for (int i = 0; i < m_count; ++i) if (m_items[i] == b) { m_items[i] = m_items[--m_count]; return; } }
    virtual void Add(CStreamBuffer* b) { ++m_adds; m_items[m_count++] = b; }
};

struct CHandleBasedMemoryPool {
    void Lock(long handle) { ++g_lockCalls; g_lastHandle = handle; }
    void Unlock(long handle) { ++g_unlockCalls; g_lastHandle = handle; }
    void Free(long handle) { ++g_freeCalls; g_lastHandle = handle; }
};

struct CPanel {
    CStreamBufferQueue     m_cachedBuffers, m_idleBuffers, m_busyBuffers, m_loadingBuffers;
    long                   m_useStamp;
    CHandleBasedMemoryPool m_pool;
};

struct CStreamBuffer {
    CSoundSample*  m_pSample;
    long           m_handle, m_dataSize, m_lastUse, m_users;
    unsigned char  m_state;
    bool           m_bLooping;
    unsigned char  m_16, m_17;
    SSampleFormat  m_format;
    CStreamBuffer() : m_pSample(0), m_handle(-1), m_dataSize(0), m_lastUse(0), m_users(0), m_state(0), m_bLooping(false) {}
    void Release(CPanel* panel);
};

void CStreamBuffer::Release(CPanel* panel)
{
    if (m_state == 1) {
        panel->m_loadingBuffers.Remove(this);
        m_state = 0;
    } else if (m_state == 3) {
        panel->m_idleBuffers.Remove(this);
        panel->m_cachedBuffers.Remove(this);
        m_state = 0;
    }
    panel->m_pool.Free(m_handle);
    m_handle = -1;
    m_dataSize = 0;
    if (m_pSample)
        m_pSample->Release();
    m_pSample = 0;
}

int main()
{
    CPanel panel; panel.m_useStamp = 0;
    CSoundSample s; s.m_refCount = 2;
    CStreamBuffer a; a.m_pSample = &s; a.m_handle = 5; a.m_dataSize = 100; a.m_state = 1;
    panel.m_loadingBuffers.Add(&a);
    a.Release(&panel);
    bool ok = !panel.m_loadingBuffers.Has(&a) && a.m_state == 0 && g_freeCalls == 1 && g_lastHandle == 5
           && a.m_handle == -1 && a.m_dataSize == 0 && a.m_pSample == 0 && s.m_refCount == 1;
    CStreamBuffer b; b.m_pSample = &s; b.m_handle = 6; b.m_state = 3;
    panel.m_idleBuffers.Add(&b); panel.m_cachedBuffers.Add(&b);
    b.Release(&panel);
    ok = ok && !panel.m_idleBuffers.Has(&b) && !panel.m_cachedBuffers.Has(&b) && b.m_state == 0 && g_freeCalls == 2 && g_lastHandle == 6 && s.m_refCount == 0;
    CStreamBuffer c; c.m_handle = 9; c.m_state = 2;           // busy: no queue touched, no sample
    panel.m_busyBuffers.Add(&c);
    c.Release(&panel);
    ok = ok && panel.m_busyBuffers.Has(&c) && c.m_state == 2 && panel.m_busyBuffers.m_removes == 0 && g_freeCalls == 3 && c.m_handle == -1 && c.m_pSample == 0;
    if (!ok) { std::printf("00c26390_TEST FAIL\n"); return 1; }
    std::printf("00c26390_TEST PASS\n");
    return 0;
}