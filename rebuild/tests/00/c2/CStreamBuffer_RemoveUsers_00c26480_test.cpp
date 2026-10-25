// behaviour test for 0x00C26480 CStreamBuffer::RemoveUsers: drop users; idle (unlocked, stamped) at zero, else stay busy
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
    void RemoveUsers(CPanel* panel, long users);
};

void CStreamBuffer::RemoveUsers(CPanel* panel, long users)
{
    panel->m_busyBuffers.Remove(this);
    m_users -= users;
    if (m_users != 0) {
        panel->m_busyBuffers.Add(this);
        m_state = 2;
    } else {
        panel->m_pool.Unlock(m_handle);
        m_lastUse = panel->m_useStamp;
        ++panel->m_useStamp;
        panel->m_idleBuffers.Add(this);
        m_state = 3;
    }
}

int main()
{
    CPanel panel; panel.m_useStamp = 10;
    CStreamBuffer a; a.m_handle = 4; a.m_users = 3; a.m_state = 2;
    panel.m_busyBuffers.Add(&a);
    a.RemoveUsers(&panel, 1);
    bool ok = a.m_users == 2 && a.m_state == 2 && panel.m_busyBuffers.Has(&a) && panel.m_busyBuffers.m_count == 1 && g_unlockCalls == 0 && panel.m_useStamp == 10;
    a.RemoveUsers(&panel, 2);
    ok = ok && a.m_users == 0 && a.m_state == 3 && !panel.m_busyBuffers.Has(&a) && panel.m_idleBuffers.Has(&a)
         && g_unlockCalls == 1 && g_lastHandle == 4 && a.m_lastUse == 10 && panel.m_useStamp == 11;
    if (!ok) { std::printf("00c26480_TEST FAIL\n"); return 1; }
    std::printf("00c26480_TEST PASS\n");
    return 0;
}