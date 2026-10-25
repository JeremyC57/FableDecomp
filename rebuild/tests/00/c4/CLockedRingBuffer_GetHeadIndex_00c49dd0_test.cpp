// Behaviour test for 00c49dd0: CLockedRingBuffer::GetHeadIndex -- (tail - count) wrapped by capacity, computed under the lock
#include <cstdio>

static int g_depth = 0, g_locks = 0;
struct CScopedLock {
    CScopedLock(void*) { ++g_locks; ++g_depth; }
    ~CScopedLock() { --g_depth; }
};
struct CLockedRingBuffer {
    unsigned char m_section[0x18];
    int   m_capacity;
    void* m_pSlots;
    int   m_count;
    int   m_tail;
    int GetHeadIndex()
    {
        CScopedLock lock(this);
        int head = m_tail - m_count;
        if (head < 0)
            head += m_capacity;
        return head;
    }
};

int main()
{
    CLockedRingBuffer r; r.m_capacity = 8; r.m_pSlots = 0;
    r.m_tail = 5; r.m_count = 3;
    if (r.GetHeadIndex() != 2) { std::printf("00c49dd0_TEST FAIL\n"); return 1; }
    r.m_tail = 1; r.m_count = 4;
    if (r.GetHeadIndex() != 5) { std::printf("00c49dd0_TEST FAIL\n"); return 1; }
    r.m_tail = 0; r.m_count = 0;
    if (r.GetHeadIndex() != 0 || g_locks != 3 || g_depth != 0) { std::printf("00c49dd0_TEST FAIL\n"); return 1; }
    std::printf("00c49dd0_TEST PASS\n");
    return 0;
}