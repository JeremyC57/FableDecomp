// behaviour test for std::deque<ETutorialCategory>::~deque 0x00412210: destroy walk over [start, finish), then the
// node buffers [start node, finish node] and the map are released (map only when non-null)
#include <cstdio>
#include <cstdlib>
static int g_walkCalls = 0, g_nodesFreed = 0, g_mapFreed = 0;
struct It { int* cur; int* first; int* last; int** node; };
struct Deque {
    It start, finish; int** map; unsigned mapSize;
    void destroyNodes(int** a, int** b) { for (int** n = a; n < b; ++n) { std::free(*n); ++g_nodesFreed; } }
    ~Deque()
    {
        ++g_walkCalls;                       // __destroy_aux: no per-element work for an enum
        if (map) {
            destroyNodes(start.node, finish.node + 1);
            if (map) { std::free(map); ++g_mapFreed; }
        }
    }
};
int main()
{
    {
        Deque d;
        d.map = (int**)std::malloc(8 * sizeof(int*)); d.mapSize = 8;
        for (int i = 0; i < 8; ++i) d.map[i] = 0;
        d.map[2] = (int*)std::malloc(0x80); d.map[3] = (int*)std::malloc(0x80);
        d.start.node = d.map + 2; d.finish.node = d.map + 3;
        d.start.cur = d.start.first = d.map[2]; d.start.last = d.map[2] + 32;
        d.finish.cur = d.finish.first = d.map[3]; d.finish.last = d.map[3] + 32;
    }
    if (g_walkCalls != 1 || g_nodesFreed != 2 || g_mapFreed != 1) { std::printf("00412210_TEST FAIL\n"); return 1; }
    {
        Deque e; e.map = 0; e.mapSize = 0;
        e.start.node = e.finish.node = 0;
    }
    if (g_walkCalls != 2 || g_nodesFreed != 2 || g_mapFreed != 1) { std::printf("00412210_TEST FAIL\n"); return 1; }
    std::printf("00412210_TEST PASS\n");
    return 0;
}