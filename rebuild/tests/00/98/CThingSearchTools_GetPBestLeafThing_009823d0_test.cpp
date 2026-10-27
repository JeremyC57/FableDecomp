#include <cstdio>
// Behaviour model: the best (nearest) visible thing that passes the filter.
struct T { int dist; bool visible, pass; };
int main()
{
    T things[4] = { {5, true, true}, {2, false, true}, {3, true, false}, {4, true, true} };
    T* best = 0;
    for (int i = 0; i < 4; ++i) if (things[i].visible && things[i].pass && (!best || things[i].dist < best->dist)) best = &things[i];
    if (best != &things[3]) { std::printf("009823d0_TEST FAIL\n"); return 1; }
    std::printf("009823d0_TEST PASS\n");
    return 0;
}