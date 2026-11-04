// Behaviour test for QuadricOptimiserInternals::COptimisedPrimitive::ComputeSubContractionCost (0x00AC82A0): a model of the row.
#include <stdio.h>

static float Cost(float errorSum, float groupDiff, float weight, float c1, float c2, float normalDot, float penalty)
{
    float cost = errorSum * 0.5f;
    cost = groupDiff * weight + cost;
    cost += c1 > c2 ? c1 : c2;
    if (normalDot < 0.0f) cost += penalty;
    return cost;
}
static int run() { return Cost(4, 1, 2, 3, 5, 1, 100) == 9 && Cost(4, 0, 2, 3, 1, -0.5f, 100) == 105; }
int main()
{
    int ok = run();
    printf(ok ? "00ac82a0_TEST PASS\n" : "00ac82a0_TEST FAIL\n");
    return ok ? 0 : 1;
}
