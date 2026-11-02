// Behaviour test for NUISystem::CSwappingStateComponent::~CSwappingStateComponent (0x005474C0): the SwappingStates storage is
// freed (only when allocated) before the CChangingStateComponent base destructor runs.
#include <cstdio>
#include <cstdlib>
static int g_log[8]; static int g_n = 0;
struct SwapState { unsigned long state; float weight; };
struct SwapVector {
    SwapState* start; SwapState* finish; SwapState* end;
    SwapVector() : start(0), finish(0), end(0) {}
    ~SwapVector() { if (start) { std::free(start); g_log[g_n++] = 10; } }
};
struct CChangingStateComponent { virtual ~CChangingStateComponent() { g_log[g_n++] = 1; } };
struct CSwappingStateComponent : CChangingStateComponent {
    SwapVector SwappingStates; float LastSwapTime; unsigned long Seed;
    virtual ~CSwappingStateComponent() { }
};
int main()
{
    {
        CSwappingStateComponent c;
        c.SwappingStates.start = (SwapState*)std::malloc(4 * sizeof(SwapState));
        c.SwappingStates.finish = c.SwappingStates.start + 1; c.SwappingStates.end = c.SwappingStates.start + 4;
    }
    { CSwappingStateComponent empty; }
    if (g_n != 3 || g_log[0] != 10 || g_log[1] != 1 || g_log[2] != 1) { std::printf("005474c0_TEST FAIL\n"); return 1; }
    std::printf("005474c0_TEST PASS\n");
    return 0;
}