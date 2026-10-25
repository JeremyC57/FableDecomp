// Behaviour test for 00c49d50: CLockedRingBuffer::Push -- under the lock append at the tail (dropped when full), tail wraps modulo capacity, count++
#include <cstdio>

static int g_depth = 0, g_maxDepth = 0, g_locks = 0;
struct CScopedLock {
    CScopedLock(void*) { ++g_locks; if (++g_depth > g_maxDepth) g_maxDepth = g_depth; }
    ~CScopedLock() { --g_depth; }
};
struct CCodaFrame { int m_id; };
struct CLockedRingBuffer {
    unsigned char m_section[0x18];
    int           m_capacity;
    CCodaFrame**  m_pSlots;
    int           m_count;
    int           m_tail;
    void Push(CCodaFrame* frame)
    {
        CScopedLock lock(this);
        if (m_count == m_capacity)
            return;
        m_pSlots[m_tail] = frame;
        m_tail = (m_tail + 1) % m_capacity;
        ++m_count;
    }
};

int main()
{
    CCodaFrame* slots[3] = { 0, 0, 0 };
    CCodaFrame f[5];
    CLockedRingBuffer r; r.m_capacity = 3; r.m_pSlots = slots; r.m_count = 1; r.m_tail = 2;   // head at slot 1
    r.Push(&f[0]);                             // slot 2, tail wraps to 0
    r.Push(&f[1]);                             // slot 0
    r.Push(&f[2]);                             // full: dropped
    if (slots[2] != &f[0] || slots[0] != &f[1] || r.m_count != 3 || r.m_tail != 1 || g_locks != 3 || g_depth != 0 || g_maxDepth != 1) {
        std::printf("00c49d50_TEST FAIL\n"); return 1;
    }
    std::printf("00c49d50_TEST PASS\n");
    return 0;
}