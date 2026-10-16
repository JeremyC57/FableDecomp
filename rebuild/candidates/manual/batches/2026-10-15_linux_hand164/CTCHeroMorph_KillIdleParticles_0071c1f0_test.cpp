#include <cstdio>
// Behaviour model of CTCHeroMorph::KillIdleParticles (0x0071C1F0): kill every non-null emitter, empty the list from the back.
struct Thing { int killed; };
int main()
{
    Thing a = { 0 }, b = { 0 };
    Thing* list[3] = { &a, 0, &b };
    int n = 3, order[3], k = 0;
    while (n != 0) {
        Thing* t = list[n - 1];
        if (t) { ++t->killed; order[k++] = (t == &a) ? 1 : 2; }
        --n;
    }
    if (a.killed != 1 || b.killed != 1 || k != 2 || order[0] != 2 || order[1] != 1) { std::printf("0071c1f0_TEST FAIL\n"); return 1; }
    std::printf("0071c1f0_TEST PASS\n");
    return 0;
}
