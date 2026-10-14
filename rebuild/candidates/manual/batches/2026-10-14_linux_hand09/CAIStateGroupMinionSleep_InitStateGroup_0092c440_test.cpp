#include <cstdio>
static unsigned long ROR13(unsigned long v) { return (v >> 13) | (v << 19); }
static unsigned long Step(unsigned long& seed) { seed = seed * 0x24A1 + 0x24DF; seed = ROR13(seed); return seed; }
struct CGroup { unsigned long seed; unsigned long bedTimeDelay, wakeTimeDelay; long state; bool flag34, WasCarryingWeapon;
    void InitStateGroup() { state = -1; flag34 = false; bedTimeDelay = 0; wakeTimeDelay = Step(seed) % 10; WasCarryingWeapon = false; } };
int main()
{
    CGroup g; g.seed = 12345; unsigned long s = g.seed; unsigned long expect = Step(s) % 10;
    g.bedTimeDelay = 5; g.wakeTimeDelay = 77; g.state = 3; g.flag34 = true; g.WasCarryingWeapon = true;
    g.InitStateGroup();
    bool ok = g.state == -1 && !g.flag34 && g.bedTimeDelay == 0 && g.wakeTimeDelay == expect && g.wakeTimeDelay < 10 && !g.WasCarryingWeapon && g.seed == s;
    if (!ok) { std::printf("0092c440_TEST FAIL\n"); return 1; }
    std::printf("0092c440_TEST PASS\n"); return 0;
}
