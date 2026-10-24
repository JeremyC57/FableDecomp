#include <cstdio>
// Behaviour test: DequeueItem on a copy of the model; Release decrements a non-zero refcount.
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
class CItem {
public:
    unsigned int m_words[3];
    unsigned int m_refCount;
    void Release() { if (m_refCount) --m_refCount; }
};
class CircularQueue {
public:
    CItem** m_items; u64* m_times; unsigned int m_capacity, m_head, m_tail; CItem* m_lastItem; unsigned int m_stats[3];
    bool DequeueItem(unsigned int index)
    {
        if (m_items[index] == 0)
            return false;
        m_lastItem = m_items[index];
        m_items[index]->Release();
        m_items[index] = 0;
        m_times[index] = 0;
        return true;
    }
};
int main()
{
    CItem a; a.m_refCount = 2;
    CItem* slots[3] = { 0, &a, 0 };
    u64 times[3] = { 7, 8, 9 };
    CircularQueue q; q.m_items = slots; q.m_times = times; q.m_capacity = 3; q.m_lastItem = 0;
    if (q.DequeueItem(0) || q.m_lastItem != 0 || times[0] != 7) { std::printf("00c44270_TEST FAIL\n"); return 1; }
    if (!q.DequeueItem(1) || q.m_lastItem != &a || a.m_refCount != 1 || slots[1] != 0 || times[1] != 0 || times[2] != 9)
    { std::printf("00c44270_TEST FAIL\n"); return 1; }
    if (q.DequeueItem(1)) { std::printf("00c44270_TEST FAIL\n"); return 1; }
    std::printf("00c44270_TEST PASS\n");
    return 0;
}