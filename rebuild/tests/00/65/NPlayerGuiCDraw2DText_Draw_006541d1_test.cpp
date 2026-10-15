#include <cstdio>
// Behaviour test for 006541d1.

// CDraw2DText::Draw: every queued text gets its own primitive handle (handles resized to the queue length) and is handed to the
// renderer with flags 0x40; the queue is emptied afterwards; an empty queue does nothing.
struct CText { int id; };
struct CRenderer { int added; int lastId; int handleIndex; long flags; };
struct CDraw2DText {
    bool Drawing; int handleCount; int handles[8]; CText queue[8]; int queued;
    void Draw(CRenderer* r)
    {
        if (queued != 0) {
            Drawing = true;
            int count = queued;
            handleCount = count;
            for (int i = 0; i < count; ++i) {
                CText primitive = queue[i];
                ++r->added; r->lastId = primitive.id; r->handleIndex = i; r->flags = 0x40; handles[i] = primitive.id;
            }
            queued = 0;
        }
    }
};
int main()
{
    CRenderer r; r.added = 0; r.lastId = 0; r.handleIndex = -1; r.flags = 0;
    CDraw2DText d; d.Drawing = false; d.handleCount = 0; d.queued = 0;
    d.Draw(&r);
    int ok = !d.Drawing && r.added == 0;
    d.queue[0].id = 5; d.queue[1].id = 6; d.queue[2].id = 7; d.queued = 3;
    d.Draw(&r);
    ok = ok && d.Drawing && d.handleCount == 3 && r.added == 3 && r.lastId == 7 && r.handleIndex == 2 && r.flags == 0x40 && d.queued == 0 && d.handles[1] == 6;
    if (!ok) { std::printf("006541d1_TEST FAIL\n"); return 1; }
    std::printf("006541d1_TEST PASS\n");
    return 0;
}