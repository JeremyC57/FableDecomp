// Behaviour test for CAIStateGroup_UsesSeat::AllocChairSittingMode (0x00938280), g++ model of the retail logic.
#include <cstdio>
enum Kind { NONE, CHAIR, BORED, SLEEPING };
static unsigned long ROR13(unsigned long v) { return (v >> 13) | (v << 19); }
static unsigned long Random(unsigned long range, unsigned long* seed) { *seed = *seed * 0x24A1 + 0x24DF; *seed = ROR13(*seed); return range ? *seed % range : 0; }
static Kind Alloc(bool supervised, unsigned long* seed)
{
    Kind mode = NONE;
    if (!supervised) {
        switch (Random(6, seed)) {
        case 0: case 1: case 2: mode = CHAIR; break;
        case 3: case 4: mode = BORED; break;
        case 5: mode = SLEEPING; break;
        }
    } else
        mode = CHAIR;
    return mode;
}
int main()
{
    unsigned long seed = 1;
    int ok = Alloc(true, &seed) == CHAIR && seed == 1;
    int counts[4] = { 0, 0, 0, 0 };
    for (int i = 0; i < 6000; ++i) {
        unsigned long before = seed;
        Kind k = Alloc(false, &seed);
        unsigned long s = before * 0x24A1 + 0x24DF; s = ROR13(s);
        unsigned long r = s % 6;
        ok &= seed == s && k == (r <= 2 ? CHAIR : r <= 4 ? BORED : SLEEPING);
        ++counts[k];
    }
    ok &= counts[NONE] == 0 && counts[CHAIR] > 0 && counts[BORED] > 0 && counts[SLEEPING] > 0;
    std::printf(ok ? "00938280_TEST PASS\n" : "00938280_TEST FAIL\n");
    return ok ? 0 : 1;
}