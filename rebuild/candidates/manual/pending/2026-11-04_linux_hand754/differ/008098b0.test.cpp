// Behaviour test for CThingCompare_BestForTargeting2::operator() (0x008098B0): a model of the row.
#include <stdio.h>

static bool Better(float scoreA, float multA, float scoreB, float multB) { return scoreA * multA - scoreB * multB > 0.0001f; }
static int run() { return Better(2, 1, 1, 1) && !Better(1, 1, 1, 1) && Better(1, 3, 2, 1) && !Better(1, 1, 2, 1); }
int main()
{
    int ok = run();
    printf(ok ? "008098b0_TEST PASS\n" : "008098b0_TEST FAIL\n");
    return ok ? 0 : 1;
}
