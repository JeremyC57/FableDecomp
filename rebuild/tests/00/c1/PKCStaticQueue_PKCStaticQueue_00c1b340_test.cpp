// Behaviour test for 00c1b340: PKCStaticQueue constructor -- capacity stored, capacity 0x1c-byte records allocated in one scalar block, count and head zeroed (the queue's Push/Pop/GetCount/IsFull then work off those fields)
#include <cstdio>
#include <cstdlib>

static unsigned int g_lastRequest = 0;
static void* Allocate(unsigned int size) { g_lastRequest = size; return std::malloc(size); }

struct SRequest {                          // seven words
    unsigned int m_hFile, m_key, m_a, m_b, m_c, m_d, m_e;
};

class PKCStaticQueue {
public:
    PKCStaticQueue(long capacity)
        : m_capacity(capacity),
          m_pRecords((SRequest*)Allocate(capacity * sizeof(SRequest))),
          m_count(0),
          m_head(0)
    {
    }
    virtual ~PKCStaticQueue() { std::free(m_pRecords); }
    long GetCount() { return m_count; }
    long IsFull() { return m_count >= m_capacity; }
    long Push(const SRequest* r)
    {
        if (IsFull()) return 0;
        m_pRecords[(m_head + m_count) % m_capacity] = *r;
        ++m_count;
        return 1;
    }
    void Pop(SRequest* r)
    {
        *r = m_pRecords[m_head];
        m_head = (m_head + 1) % m_capacity;
        --m_count;
    }

    long      m_capacity;
    SRequest* m_pRecords;
    long      m_count;
    long      m_head;
};

int main()
{
    if (sizeof(SRequest) != 0x1c) { std::printf("00c1b340_TEST FAIL\n"); return 1; }
    PKCStaticQueue q(0x28);                // the file manager's capacity
    if (q.m_capacity != 0x28 || q.m_count != 0 || q.m_head != 0 || q.m_pRecords == 0) { std::printf("00c1b340_TEST FAIL\n"); return 1; }
    if (g_lastRequest != 0x28 * 0x1c) { std::printf("00c1b340_TEST FAIL\n"); return 1; }
    PKCStaticQueue small(2);
    SRequest a = { 1, 2, 3, 4, 5, 6, 7 }, b = a, c = a, out;
    b.m_hFile = 9; c.m_hFile = 11;
    if (!small.Push(&a) || !small.Push(&b) || small.Push(&c) || !small.IsFull() || small.GetCount() != 2) { std::printf("00c1b340_TEST FAIL\n"); return 1; }
    small.Pop(&out);
    if (out.m_hFile != 1 || out.m_e != 7 || small.GetCount() != 1 || small.IsFull()) { std::printf("00c1b340_TEST FAIL\n"); return 1; }
    std::printf("00c1b340_TEST PASS\n");
    return 0;
}