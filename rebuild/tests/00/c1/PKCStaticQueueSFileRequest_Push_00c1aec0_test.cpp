// Behaviour test for 0x00C1AEC0 PKCStaticQueue<SFileRequest>::Push (ring append, head wraps by capacity, full-queue trace)
#include <cstdio>
#include <cstring>

struct Req { long w[7]; };
static int g_traces = 0;
static char g_last[128];
struct Queue {
    unsigned long capacity; Req* records; unsigned long count; unsigned long head;
    long Push(const Req* item)
    {
        if (count >= capacity) {
            std::sprintf(g_last, "%s(%d) : %s\n", "PKCStaticQueue.h", 57, "This queue is full!");
            ++g_traces;
        }
        records[head] = *item;
        head = (head + 1) % capacity;
        return (long)++count;
    }
};

int main()
{
    Req slots[3]; std::memset(slots, 0, sizeof slots);
    Queue q; q.capacity = 3; q.records = slots; q.count = 0; q.head = 0;
    Req r; for (int i = 0; i < 7; ++i) r.w[i] = i;
    if (q.Push(&r) != 1 || q.head != 1 || slots[0].w[6] != 6 || g_traces) { std::printf("00c1aec0_TEST FAIL\n"); return 1; }
    r.w[0] = 10; q.Push(&r); r.w[0] = 20; q.Push(&r);
    if (q.count != 3 || q.head != 0 || slots[2].w[0] != 20 || g_traces) { std::printf("00c1aec0_TEST FAIL\n"); return 1; }
    q.count = 2; r.w[0] = 30;                       // one slot freed: the head wrapped to 0
    if (q.Push(&r) != 3 || slots[0].w[0] != 30 || q.head != 1) { std::printf("00c1aec0_TEST FAIL\n"); return 1; }
    q.Push(&r);                                      // full: traced (and retail breaks)
    if (g_traces != 1 || std::strcmp(g_last, "PKCStaticQueue.h(57) : This queue is full!\n") != 0) { std::printf("00c1aec0_TEST FAIL\n"); return 1; }
    std::printf("00c1aec0_TEST PASS\n");
    return 0;
}