// Behaviour test for 00c442d0: CircularQueue::GetTime -- returns the slot's full 64-bit time
#include <cstdio>
struct CItem;
class CircularQueue { public: CItem** m_items; __int64* m_times; unsigned long m_capacity; __int64 GetTime(unsigned long index) { return m_times[index]; } };
int main()
{
    __int64 times[3] = { 5, 0x0000000700000003LL, -2 };
    CircularQueue q; q.m_items = 0; q.m_times = times; q.m_capacity = 3;
    int ok = q.GetTime(0) == 5 && q.GetTime(1) == 0x0000000700000003LL && q.GetTime(2) == -2;
    std::printf(ok ? "00c442d0_TEST PASS\n" : "00c442d0_TEST FAIL\n");
    return ok ? 0 : 1;
}
