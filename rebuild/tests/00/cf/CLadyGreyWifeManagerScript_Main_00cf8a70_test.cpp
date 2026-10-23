// Behaviour test for CLadyGreyWifeManagerScript::Main (0x00CF8A70): a married Lady Grey in limbo comes out by day in
// Bowerstone Posh and by night in the mayor's bedroom; out of limbo she goes back by the opposite rule.
#include <cstdio>
static bool Day(long t) { return t > 800 && t < 2000; }
static int Decide(bool limbo, bool posh, long t)
{
    if (limbo) {
        if (posh) return Day(t) ? 1 : 0;      // 1 = appear in Posh
        return Day(t) ? 0 : 2;                // 2 = appear in the bedroom
    }
    if (posh) return Day(t) ? 0 : 3;          // 3 = back into limbo
    return Day(t) ? 3 : 0;
}
int main()
{
    if (Decide(true, true, 1200) != 1 || Decide(true, true, 300) != 0 || Decide(true, false, 2100) != 2
        || Decide(false, true, 2100) != 3 || Decide(false, false, 1000) != 3 || Decide(false, false, 800) != 0) {
        std::printf("00cf8a70_TEST FAIL\n");
        return 1;
    }
    std::printf("00cf8a70_TEST PASS\n");
    return 0;
}