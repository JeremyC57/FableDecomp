// Behaviour test for 00c4a0e0: CLockedRingBuffer::Pop -- null when empty, else the head frame (FIFO order) and one fewer
#include <cstdio>
static int g_locks = 0;
struct CCodaFrame { int id; };
struct Lock { Lock() { ++g_locks; } ~Lock() { --g_locks; } };
struct CLockedRingBuffer {
    long m_capacity; CCodaFrame** m_pSlots; long m_count; long m_tail;
    void Push(CCodaFrame* f) { Lock l; if (m_count == m_capacity) return; m_pSlots[m_tail] = f; m_tail = (m_tail + 1) % m_capacity; ++m_count; }
    long GetHeadIndex() { return (m_tail - m_count + m_capacity) % m_capacity; }
    CCodaFrame* Pop() { Lock l; if (m_count == 0) return 0; CCodaFrame* f = m_pSlots[GetHeadIndex()]; --m_count; return f; }
};
int main()
{
    CCodaFrame* slots[3]; CLockedRingBuffer r = { 3, slots, 0, 0 }; CCodaFrame a = { 1 }, b = { 2 }, c = { 3 };
    bool ok = r.Pop() == 0;
    r.Push(&a); r.Push(&b); ok = ok && r.Pop() == &a; r.Push(&c); ok = ok && r.Pop() == &b && r.Pop() == &c && r.Pop() == 0 && g_locks == 0;
    if (!ok) { std::printf("00c4a0e0_TEST FAIL\n"); return 1; }
    std::printf("00c4a0e0_TEST PASS\n");
    return 0;
}
