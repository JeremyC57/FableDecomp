// Behaviour test for 0x007B4B00 deque<CCrimeWaitingForBribe>::clear: interior + finish buffers freed, finish = start.
#include <cstdio>
#include <cstdlib>

static int g_frees = 0;
static void Free(void* p) { if (p) { ++g_frees; std::free(p); } }
struct Rec { int a, b; };
struct It { Rec* cur; Rec* first; Rec* last; Rec** node; };
struct Deque {
    It start, finish;
    void clear()
    {
        for (Rec** node = start.node + 1; node < finish.node; ++node) Free(*node);
        if (start.node != finish.node) Free(finish.first);
        finish = start;
    }
};
static Rec* Buf() { return (Rec*)std::malloc(16 * sizeof(Rec)); }

int main()
{
    Rec* map[4] = { Buf(), Buf(), Buf(), Buf() };
    Deque d;
    d.start.node = &map[0]; d.start.first = map[0]; d.start.cur = map[0] + 5; d.start.last = map[0] + 16;
    d.finish.node = &map[3]; d.finish.first = map[3]; d.finish.cur = map[3] + 2; d.finish.last = map[3] + 16;
    d.clear();
    // buffers 1, 2 (interior) and 3 (finish) freed; buffer 0 kept
    if (g_frees != 3 || d.finish.node != &map[0] || d.finish.cur != map[0] + 5 || d.finish.first != map[0] || d.finish.last != map[0] + 16) { std::printf("007b4b00_TEST FAIL\n"); return 1; }
    // single-node deque: nothing freed
    d.finish.cur = map[0] + 9;
    d.clear();
    if (g_frees != 3 || d.finish.cur != map[0] + 5) { std::printf("007b4b00_TEST FAIL\n"); return 1; }
    std::free(map[0]);
    std::printf("007b4b00_TEST PASS\n");
    return 0;
}