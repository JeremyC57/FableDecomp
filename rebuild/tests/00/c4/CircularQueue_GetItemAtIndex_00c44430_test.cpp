#include <cstdio>
// Behaviour test: GetItemAtIndex is false out of range / on an empty queue / on an empty slot, true on a filled one.
class CItem { public: int m_x; };
class CircularQueue {
public:
    CItem** m_items; void* m_times; unsigned int m_capacity;
    bool IsValidIndex(unsigned int index) const { return m_capacity != 0 && index < m_capacity; }
    bool GetItemAtIndex(unsigned int index)
    {
        if (!IsValidIndex(index))
            return false;
        return m_items[index] != 0;
    }
};
int main()
{
    CItem a;
    CItem* slots[3] = { &a, 0, &a };
    CircularQueue q; q.m_items = slots; q.m_times = 0; q.m_capacity = 0;
    if (q.GetItemAtIndex(0)) { std::printf("00c44430_TEST FAIL\n"); return 1; }
    q.m_capacity = 3;
    if (!q.GetItemAtIndex(0) || q.GetItemAtIndex(1) || !q.GetItemAtIndex(2) || q.GetItemAtIndex(3))
    { std::printf("00c44430_TEST FAIL\n"); return 1; }
    std::printf("00c44430_TEST PASS\n");
    return 0;
}