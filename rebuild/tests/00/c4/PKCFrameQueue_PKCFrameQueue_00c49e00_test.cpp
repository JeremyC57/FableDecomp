// Behaviour test for 00c49e00: PKCFrameQueue::PKCFrameQueue -- capacity stored, capacity pointer slots from new[], count and head zeroed
#include <cstdio>

struct CCodaFrame { int m_id; };
class PKCFrameQueue {
public:
    PKCFrameQueue(long capacity) : m_capacity(capacity), m_pSlots(new CCodaFrame*[capacity]), m_count(0), m_head(0) { }
    virtual ~PKCFrameQueue() { delete[] m_pSlots; }
    long Push(CCodaFrame* f) { if (m_count >= m_capacity) return 0; m_pSlots[(m_head + m_count) % m_capacity] = f; ++m_count; return 1; }
    long         m_capacity;
    CCodaFrame** m_pSlots;
    long         m_count;
    long         m_head;
};

int main()
{
    PKCFrameQueue q(3);
    if (q.m_capacity != 3 || q.m_pSlots == 0 || q.m_count != 0 || q.m_head != 0) { std::printf("00c49e00_TEST FAIL\n"); return 1; }
    CCodaFrame a, b, c, d;
    if (!q.Push(&a) || !q.Push(&b) || !q.Push(&c) || q.Push(&d) || q.m_pSlots[2] != &c) { std::printf("00c49e00_TEST FAIL\n"); return 1; }
    std::printf("00c49e00_TEST PASS\n");
    return 0;
}