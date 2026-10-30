// Behaviour test for 0x00C5B300 CSoftwareVoice::AdvanceBuffer(sound): only acts on a drained ring; a single-buffer sound
// restarts when looping or stops the voice; a queued looping sound with a streamed (prefetch) tail buffer re-feeds it;
// else the tail is dequeued and the next slot handed over at its due time, or the voice stops when nothing is left.
#include <cstdio>

struct Buffer { unsigned long prefetch; };
struct Queue {
    Buffer** items; unsigned long* times; unsigned long capacity, head, tail, stats0;
    bool IsEmpty() const { return capacity != 0 && stats0 == 0; }
    void DequeueNext() { if (items[tail]) { items[tail] = 0; --stats0; if (++tail == capacity) tail = 0; } }
};
struct Sound { bool loop; Queue queue; Buffer* single; int reports; };
struct Voice {
    void* ringData; unsigned long queued; Buffer* current; unsigned long clock; Buffer* fed; int feedLoop; int clears;
    void SetBuffer(Buffer* b, int loop) { fed = b; feedLoop = loop; }
    void Clear() { ++clears; ringData = 0; }
    void AdvanceBuffer(Sound* s)
    {
        if (!ringData || queued != 0) return;
        if (s->single) { if (s->loop) { SetBuffer(s->single, 1); return; } Clear(); current = 0; return; }
        Queue& q = s->queue;
        Buffer* entry = q.capacity ? q.items[q.tail] : 0;
        if (s->loop && entry->prefetch != 0) { SetBuffer(entry, s->loop); return; }
        q.DequeueNext(); ++s->reports;
        if (!q.IsEmpty() && q.items[q.tail] != 0 && (q.tail != 0 || s->loop)) {
            clock = q.times[q.tail]; SetBuffer(q.capacity ? q.items[q.tail] : 0, s->loop); return;
        }
        Clear(); current = 0;
    }
};

static int fail() { std::printf("00c5b300_TEST FAIL\n"); return 1; }
static int dummy;

int main()
{
    Buffer b0 = { 0 }, b1 = { 0 }, single = { 0 };
    Buffer* items[3] = { &b0, &b1, 0 }; unsigned long times[3] = { 100, 200, 300 };
    Sound s; s.loop = false; s.single = 0; s.reports = 0;
    s.queue.items = items; s.queue.times = times; s.queue.capacity = 3; s.queue.head = 2; s.queue.tail = 0; s.queue.stats0 = 2;
    Voice v; v.ringData = &dummy; v.queued = 5; v.current = &b0; v.clock = 0; v.fed = 0; v.feedLoop = -1; v.clears = 0;
    v.AdvanceBuffer(&s); if (v.fed || s.reports) return fail();        // ring still has frames
    v.queued = 0;
    v.AdvanceBuffer(&s);                                                // dequeue b0, hand over b1 at 200
    if (v.fed != &b1 || v.clock != 200 || s.queue.tail != 1 || s.reports != 1 || v.feedLoop != 0) return fail();
    v.AdvanceBuffer(&s);                                                // dequeue b1, nothing left: stop
    if (v.clears != 1 || v.current != 0 || s.queue.tail != 2) return fail();
    v.ringData = &dummy; s.single = &single; s.loop = true; v.fed = 0;
    v.AdvanceBuffer(&s); if (v.fed != &single || v.feedLoop != 1) return fail();
    s.loop = false; v.current = &single;
    v.AdvanceBuffer(&s); if (v.clears != 2 || v.current != 0) return fail();
    Buffer streamed = { 4096 }; Buffer* items2[2] = { &streamed, 0 };
    s.single = 0; s.loop = true; s.queue.items = items2; s.queue.capacity = 2; s.queue.tail = 0; s.queue.stats0 = 1;
    v.ringData = &dummy; v.fed = 0; s.reports = 0;
    v.AdvanceBuffer(&s); if (v.fed != &streamed || s.reports != 0 || s.queue.tail != 0) return fail();
    std::printf("00c5b300_TEST PASS\n");
    return 0;
}