#include <cstdio>
// behaviour test: SetupFromData -- stores the def type and buildings; respawnable: due tomorrow, or with a creature def
// min + (LCG roll % (max - min), 0 for an empty range) days from today; not respawnable: day 0x7fffffff
static unsigned long Ror13(unsigned long v) { return (v >> 13) | (v << 19); }
struct Dummy {
    unsigned long DefType; void* Home; void* Work; long Day;
    void SetupFromData(unsigned long defType, void* home, void* work, bool respawnable, long today, bool hasCreatureDef,
                       long minDays, long maxDays, unsigned long& seed)
    {
        DefType = defType; Home = home; Work = work;
        if (respawnable) {
            Day = today + 1;
            if (hasCreatureDef) {
                seed = seed * 0x24a1 + 0x24df; seed = Ror13(seed);
                unsigned long range = (unsigned long)(maxDays - minDays);
                Day = minDays + (long)(range ? seed % range : 0) + today;
            }
        } else
            Day = 0x7fffffff;
    }
};
int main()
{
    int h = 0, w = 0; unsigned long seed = 1;
    Dummy d;
    d.SetupFromData(5, &h, &w, false, 10, true, 2, 4, seed);
    bool ok = d.DefType == 5 && d.Home == &h && d.Work == &w && d.Day == 0x7fffffff && seed == 1;
    d.SetupFromData(6, 0, 0, true, 10, false, 2, 4, seed);
    ok = ok && d.Day == 11 && seed == 1;
    d.SetupFromData(6, 0, 0, true, 10, true, 3, 3, seed);       // empty range: exactly min days
    ok = ok && d.Day == 13 && seed != 1;
    d.SetupFromData(6, 0, 0, true, 10, true, 2, 5, seed);
    ok = ok && d.Day >= 12 && d.Day < 15;
    if (!ok) { std::printf("0072f170_TEST FAIL\n"); return 1; }
    std::printf("0072f170_TEST PASS\n");
    return 0;
}