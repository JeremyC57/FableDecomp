// Behaviour test for 0x00C57460 (standalone copy of the model and logic)
#include <cstdio>
#include <cstring>

struct CAudioContext { void* Module; int calls; int lastArg; int willLoad;
    bool Initialize(bool reload) { calls++; lastArg = reload ? 1 : 0; if (willLoad) Module = (void*)this; return Module != 0; } };
CAudioContext g_AudioContext;
bool Initialize(const void* device, void* window, bool multithreaded)
{
    if (!g_AudioContext.Module)
        g_AudioContext.Initialize(false);
    return g_AudioContext.Module != 0;
}
int main()
{
    g_AudioContext.Module = 0; g_AudioContext.calls = 0; g_AudioContext.willLoad = 0;
    if (Initialize(0, 0, true) || g_AudioContext.calls != 1 || g_AudioContext.lastArg != 0) { std::printf("00c57460_TEST FAIL\n"); return 1; }
    g_AudioContext.willLoad = 1;
    if (!Initialize(0, 0, false) || g_AudioContext.calls != 2) { std::printf("00c57460_TEST FAIL\n"); return 1; }
    if (!Initialize(0, 0, false) || g_AudioContext.calls != 2) { std::printf("00c57460_TEST FAIL\n"); return 1; }
    std::printf("00c57460_TEST PASS\n");
    return 0;
}