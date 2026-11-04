// Behaviour test for 0x00969E70 CAIStateGroup_SleepOnFloor::IsRunnable(): a model; checks false with no sleeping spot in the 20-unit box, else a spot picked by the advanced world seed (seed*0x24A1+0x24DF rotated right by 13) modulo the count.
#include <cstdio>
#include <cmath>
#include <vector>
#include <string>
#include <set>
#include <map>

static unsigned long Ror13(unsigned long v) { v &= 0xffffffffu; return ((v >> 13) | (v << 19)) & 0xffffffffu; }
static bool IsRunnable(const std::vector<int>& spots, unsigned long& seed, int& picked)
{
    if (spots.empty()) return false;
    unsigned long count = (unsigned long)spots.size();
    seed = (seed * 0x24A1u + 0x24DFu) & 0xffffffffu; seed = Ror13(seed);
    picked = spots[count ? seed % count : 0]; return true;
}
int main()
{
    int ok = 1, p = -1; unsigned long seed = 1; std::vector<int> s;
    ok &= !IsRunnable(s, seed, p) && seed == 1;
    s.push_back(10); s.push_back(11); s.push_back(12);
    unsigned long expect = Ror13(1u * 0x24A1u + 0x24DFu);
    ok &= IsRunnable(s, seed, p) && seed == expect && p == s[expect % 3];
    std::printf(ok ? "00969E70_TEST PASS\n" : "00969E70_TEST FAIL\n"); return ok ? 0 : 1;
}
