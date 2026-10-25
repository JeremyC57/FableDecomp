// behaviour test for 0x00C262E0 CStreamBuffer::Attach: sample ref, format minus header, handle/users, loading queue, pool lock
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
    void Attach(CPanel* panel, CSoundSample* sample, long handle, long users);
};

void CStreamBuffer::Attach(CPanel* panel, CSoundSample* sample, long handle, long users)
{
    m_state = 1;
    m_pSample = sample;
    sample->AddRef();
    m_format = m_pSample->m_format;
    m_dataSize = m_pSample->GetDataSize();
    m_dataSize += m_pSample->GetHeaderSize();
    m_format.m_dataSize -= m_pSample->GetHeaderSize();
    m_handle = handle;
    m_lastUse = 0;
    m_users = users;
    m_bLooping = m_pSample->IsLooping();
    panel->m_loadingBuffers.Add(this);
    panel->m_pool.Lock(m_handle);
}

int main()
{
    CPanel panel; panel.m_useStamp = 0;
    CSoundSample s; s.m_header = 44; s.m_size = 1000; s.m_refCount = 1; s.m_looping = true;
    s.m_format.m_dataSize = 5000; s.m_format.m_04 = 4; s.m_format.m_08 = 8; s.m_format.m_channels = 2; s.m_format.m_0e = 16; s.m_format.m_sampleRate = 22050;
    CStreamBuffer b; b.m_lastUse = 99;
    b.Attach(&panel, &s, 7, 3);
    bool ok = b.m_state == 1 && b.m_pSample == &s && s.m_refCount == 2 && b.m_dataSize == 1044
           && b.m_format.m_dataSize == 4956 && b.m_format.m_channels == 2 && b.m_format.m_sampleRate == 22050 && s.m_format.m_dataSize == 5000
           && b.m_handle == 7 && b.m_lastUse == 0 && b.m_users == 3 && b.m_bLooping
           && panel.m_loadingBuffers.Has(&b) && panel.m_loadingBuffers.m_count == 1 && g_lockCalls == 1 && g_lastHandle == 7;
    if (!ok) { std::printf("00c262e0_TEST FAIL\n"); return 1; }
    std::printf("00c262e0_TEST PASS\n");
    return 0;
}