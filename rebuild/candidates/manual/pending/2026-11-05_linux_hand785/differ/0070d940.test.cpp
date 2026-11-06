// Behaviour test for CAnimateThing::CalcInterpolatedMeshAnim (0x0070D940): a model of the blend construction -- the main set is
// used alone when the thing is beyond the LOD distance or has no transition sets; with one transition set the main set
// transitions from it, with several the sets are chained (each link's progress = its interpolated alpha / the running total,
// 1 when the total is outside [eps, 1 + eps]); combination anims (current list, then delayed list) are added in priority order
// when their animation is loaded, each with its interpolated internal alpha.
#include <stdio.h>
#include <math.h>

struct Set { float last, current; int prio; bool loaded; float LerpAlpha(float t) const { return last + (current - last) * t; } };
static float Progress(float a, float total) { const float eps = 1e-4f; if (total >= eps && eps + 1.0f >= total) return a / total; return 1.0f; }
static float links[8]; static int nlinks; static int combos[8]; static int ncombos; static float mainProgress;

static void Calc(const Set* trans, int ntrans, const Set& main, const Set* comb, int ncomb, bool beyondLod, float t)
{
    nlinks = 0; ncombos = 0; mainProgress = -1.0f;
    if (!beyondLod && ntrans > 0) {
        if (ntrans > 1) {
            float total = trans[0].LerpAlpha(t) + trans[1].LerpAlpha(t);
            links[nlinks++] = Progress(trans[1].LerpAlpha(t), total);
            for (int i = 2; i < ntrans; i++) { total += trans[i].LerpAlpha(t); links[nlinks++] = Progress(trans[i].LerpAlpha(t), total); }
        }
        mainProgress = main.LerpAlpha(t);
    }
    int order[8]; for (int i = 0; i < ncomb; i++) order[i] = i;
    for (int i = 1; i < ncomb; i++) for (int j = i; j > 0 && comb[order[j]].prio < comb[order[j - 1]].prio; j--) { int x = order[j]; order[j] = order[j - 1]; order[j - 1] = x; }   // stable
    for (int i = 0; i < ncomb; i++) if (comb[order[i]].loaded) combos[ncombos++] = order[i];
}

int main()
{
    Set tr[3] = { { 0.0f, 0.5f, 0, true }, { 0.0f, 0.5f, 0, true }, { 0.0f, 1.0f, 0, true } };
    Set mainSet = { 0.0f, 1.0f, 0, true };
    Set comb[3] = { { 0, 0, 2, true }, { 0, 0, 1, false }, { 0, 0, 1, true } };
    Calc(tr, 3, mainSet, comb, 3, false, 1.0f);
    // link 1: 0.5 / 1.0; link 2: total 2.0 is above 1 + eps -> 1
    if (nlinks != 2 || fabs(links[0] - 0.5f) > 1e-6f || links[1] != 1.0f || mainProgress != 1.0f || ncombos != 2 || combos[0] != 2 || combos[1] != 0) {
        printf("0070d940_TEST FAIL\n"); return 1;
    }
    Calc(tr, 3, mainSet, comb, 0, true, 0.5f);
    if (nlinks != 0 || mainProgress != -1.0f) { printf("0070d940_TEST FAIL\n"); return 1; }
    printf("0070d940_TEST PASS\n");
    return 0;
}
