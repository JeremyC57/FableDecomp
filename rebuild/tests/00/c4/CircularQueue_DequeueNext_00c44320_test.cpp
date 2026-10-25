// Behaviour test for 00c44320: CircularQueue::DequeueNext -- release the tail item when present, advance the tail with wrap, return it
#include <cstdio>

class CItem { public: unsigned int m_words[3]; unsigned int m_refCount; void Release() { if (m_refCount) --m_refCount; } };
class CircularQueue {
public:
    CItem**             m_items;
    unsigned long long* m_times;
    unsigned int        m_capacity, m_head, m_tail;
    CItem*              m_lastItem;
    bool DequeueItem(unsigned int index)
    {
        if (m_items[index] == 0) return false;
        m_lastItem = m_items[index]; m_items[index]->Release(); m_items[index] = 0; m_times[index] = 0; return true;
    }
    unsigned int DequeueNext()
    {
        if (m_items[m_tail] != 0) {
            DequeueItem(m_tail);
            if (m_capacity == ++m_tail)
                m_tail = 0;
        }
        return m_tail;
    }
};

int main()
{
    CItem a = { { 0, 0, 0 }, 2 }, b = { { 0, 0, 0 }, 1 };
    CItem* items[3] = { &a, 0, &b };
    unsigned long long times[3] = { 5, 6, 7 };
    CircularQueue q; q.m_items = items; q.m_times = times; q.m_capacity = 3; q.m_head = 0; q.m_tail = 2; q.m_lastItem = 0;
    if (q.DequeueNext() != 0 || items[2] != 0 || b.m_refCount != 0 || q.m_lastItem != &b || times[2] != 0) { std::printf("00c44320_TEST FAIL\n"); return 1; }
    if (q.DequeueNext() != 1 || items[0] != 0 || a.m_refCount != 1) { std::printf("00c44320_TEST FAIL\n"); return 1; }
    if (q.DequeueNext() != 1) { std::printf("00c44320_TEST FAIL\n"); return 1; }   // empty slot: tail stays
    std::printf("00c44320_TEST PASS\n");
    return 0;
}