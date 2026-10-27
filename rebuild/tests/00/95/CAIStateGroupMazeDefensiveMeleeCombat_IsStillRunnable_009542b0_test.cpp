#include <cstdio>
// Behaviour model: runnable while the target lives and the group is not stopping.
static bool still(bool target, bool dead, bool stopping) { return target && !dead && !stopping; }
int main()
{
    if (!still(true, false, false) || still(false, false, false) || still(true, true, false) || still(true, false, true)) { std::printf("009542b0_TEST FAIL\n"); return 1; }
    std::printf("009542b0_TEST PASS\n");
    return 0;
}