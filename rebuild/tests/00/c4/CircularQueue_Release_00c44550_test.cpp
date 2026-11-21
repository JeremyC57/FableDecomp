#include <cstdio>
#include <cstdlib>
#include <cstring>
// Behaviour model of CircularQueue::Release (0x00C44550): with a capacity, Clear the queue, free the item / time block, zero items,
// times and capacity; with no capacity, nothing happens. (Standalone; the row's model is the landed sibling 0x00C437C0 shape.)
static int g_cleared = 0; static void* g_freed = 0;
static void FreeBlock_00c3c370(void* b) { g_freed = b; }
struct CircularQueue { void** m_items; unsigned long long* m_times; unsigned long m_capacity; unsigned long m_head; unsigned long m_tail;
    void Clear() { ++g_cleared; }
    void Release() { if (m_capacity) { Clear(); FreeBlock_00c3c370(m_items); m_items = 0; m_times = 0; m_capacity = 0; } } };
int main() {
    void* blk[4] = {0, 0, 0, 0};
    CircularQueue q; q.m_items = blk; q.m_times = (unsigned long long*)(blk + 2); q.m_capacity = 0; q.m_head = 0; q.m_tail = 0;
    q.Release(); if (g_cleared || g_freed || q.m_items != blk) { std::printf("00c44550_TEST FAIL\n"); return 1; }
    q.m_capacity = 2; q.Release(); if (g_cleared != 1 || g_freed != blk || q.m_items || q.m_times || q.m_capacity) { std::printf("00c44550_TEST FAIL\n"); return 1; }
    std::printf("00c44550_TEST PASS\n"); return 0;
}
