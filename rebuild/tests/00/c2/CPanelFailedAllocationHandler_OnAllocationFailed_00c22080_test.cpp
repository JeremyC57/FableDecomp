// Behaviour test for 0x00C22080 CPanelFailedAllocationHandler::OnAllocationFailed: evict the first idle buffer, else the
// first busy one unless it covers the key; detach, release and delete it and return 1, else return 0.
#include <cstdio>

static int g_log[16];
static int g_n = 0;
struct CPanel;
struct CStreamBuffer {
    long m_size;
    int  m_id;
    bool Covers(long key) { return m_size >= key; }
    void Release(CPanel*) { g_log[g_n++] = 100 + m_id; }
    ~CStreamBuffer() { g_log[g_n++] = 200 + m_id; }
};
struct CStreamBufferQueue { CStreamBuffer* first; CStreamBuffer* GetFirst() { return first; } };
struct CPanel {
    CStreamBufferQueue m_idleBuffers, m_busyBuffers;
    void DetachBuffer(CStreamBuffer* b) { g_log[g_n++] = b->m_id; if (m_idleBuffers.first == b) m_idleBuffers.first = 0; else m_busyBuffers.first = 0; }
};
struct CHandler {
    CPanel* m_pPanel; long m_key;
    long OnAllocationFailed(long)
    {
        CStreamBuffer* buffer = m_pPanel->m_idleBuffers.GetFirst();
        if (!buffer) {
            buffer = m_pPanel->m_busyBuffers.GetFirst();
            if (buffer && buffer->Covers(m_key))
                buffer = 0;
        }
        if (buffer) {
            m_pPanel->DetachBuffer(buffer);
            buffer->Release(m_pPanel);
            delete buffer;
            return 1;
        }
        return 0;
    }
};

static CStreamBuffer* Make(long size, int id) { CStreamBuffer* b = new CStreamBuffer; b->m_size = size; b->m_id = id; return b; }

int main()
{
    CPanel panel; panel.m_idleBuffers.first = Make(10, 1); panel.m_busyBuffers.first = Make(50, 2);
    CHandler h; h.m_pPanel = &panel; h.m_key = 40;
    int ok = 1;
    if (h.OnAllocationFailed(64) != 1 || g_n != 3 || g_log[0] != 1 || g_log[1] != 101 || g_log[2] != 201) ok = 0;   // idle first
    if (h.OnAllocationFailed(64) != 0 || g_n != 3) ok = 0;                // busy buffer covers the key: kept
    h.m_key = 60;
    if (h.OnAllocationFailed(64) != 1 || g_n != 6 || g_log[3] != 2 || g_log[5] != 202) ok = 0;   // busy too small: evicted
    if (h.OnAllocationFailed(64) != 0 || g_n != 6) ok = 0;                // both queues empty
    std::printf(ok ? "00c22080_TEST PASS\n" : "00c22080_TEST FAIL\n");
    return ok ? 0 : 1;
}