// Behaviour test for 00c442c0: CircularQueue::GetItem -- returns the slot's item
#include <cstdio>
struct CItem { int id; };
class CircularQueue { public: CItem** m_items; __int64* m_times; unsigned long m_capacity; CItem* GetItem(unsigned long index) { return m_items[index]; } };
int main()
{
    CItem a = { 1 }, b = { 2 }; CItem* items[2] = { &a, &b }; __int64 times[2] = { 5, 6 };
    CircularQueue q; q.m_items = items; q.m_times = times; q.m_capacity = 2;
    int ok = q.GetItem(0) == &a && q.GetItem(1) == &b;
    std::printf(ok ? "00c442c0_TEST PASS\n" : "00c442c0_TEST FAIL\n");
    return ok ? 0 : 1;
}
