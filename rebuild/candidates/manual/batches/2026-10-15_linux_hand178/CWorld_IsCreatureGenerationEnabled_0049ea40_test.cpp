#include <cstdio>
#include <cstring>
struct Q { bool gen, activeInRegion, quest; };
static bool Enabled(bool worldGen, const Q& q, bool regionFlag, bool defBit, bool hasDef)
{
    if (worldGen && q.gen) { if (regionFlag && !q.activeInRegion) return true; }
    if (q.quest) return true;
    if (hasDef && defBit) return true;
    return false;
}
static bool Run()
{
    Q q = { true, false, false };
    if (!Enabled(true, q, true, false, false)) return false;
    if (Enabled(false, q, true, false, false)) return false;
    q.activeInRegion = true; if (Enabled(true, q, true, false, true)) return false;
    if (!Enabled(true, q, true, true, true)) return false;
    q.quest = true; return Enabled(false, q, false, false, false);
}
int main()
{
    if (!Run()) { std::printf("0049ea40_TEST FAIL\n"); return 1; }
    std::printf("0049ea40_TEST PASS\n");
    return 0;
}
