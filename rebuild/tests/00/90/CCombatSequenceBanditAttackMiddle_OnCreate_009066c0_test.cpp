#include <cstdio>
// Behaviour model: two actions are queued, then the base creation runs.
static int queued, created;
int main()
{
    for (int i = 0; i < 2; ++i) ++queued;
    ++created;
    if (queued != 2 || created != 1) { std::printf("009066c0_TEST FAIL\n"); return 1; }
    std::printf("009066c0_TEST PASS\n");
    return 0;
}