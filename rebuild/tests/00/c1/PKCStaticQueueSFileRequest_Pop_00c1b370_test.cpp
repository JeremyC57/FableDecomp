// Behaviour test for 0x00C1B370 PKCStaticQueue<SFileRequest>::Pop (oldest record at head - count, wrapped; returns item)
#include <cstdio>

struct Req { long w[7]; };
static int g_traces = 0;
struct Queue {
    unsigned long capacity; Req* records; unsigned long count; unsigned long head;
    long GetTailIndex() { long i = (long)(head - count); if (i < 0) i += (long)capacity; return i; }
    Req* Pop(Req* item)
    {
        if (count == 0) { ++g_traces; return item; }   // retail traces line 67 and breaks into the debugger
        long index = GetTailIndex();
        --count;
        *item = records[index];
        return item;
    }
};

int main()
{
    Req slots[4];
    for (int i = 0; i < 4; ++i) for (int k = 0; k < 7; ++k) slots[i].w[k] = i * 10 + k;
    Queue q; q.capacity = 4; q.records = slots; q.count = 3; q.head = 1;   // queued: slots 2, 3, 0
    Req out;
    if (q.Pop(&out) != &out || out.w[0] != 20 || out.w[6] != 26 || q.count != 2) { std::printf("00c1b370_TEST FAIL\n"); return 1; }
    q.Pop(&out); if (out.w[0] != 30) { std::printf("00c1b370_TEST FAIL\n"); return 1; }
    q.Pop(&out); if (out.w[0] != 0 || q.count != 0 || g_traces) { std::printf("00c1b370_TEST FAIL\n"); return 1; }
    q.Pop(&out); if (g_traces != 1) { std::printf("00c1b370_TEST FAIL\n"); return 1; }
    std::printf("00c1b370_TEST PASS\n");
    return 0;
}