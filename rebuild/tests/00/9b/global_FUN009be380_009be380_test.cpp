#include <cstdio>
// Behaviour test for 009be380: (float)numerator / denominator, the denominator being the first argument.
static float Ratio(long denominator, long numerator) { return (float)numerator / denominator; }
int main()
{
    int ok = (Ratio(1, 60) == 60.0f && Ratio(1001, 60000) > 59.9f && Ratio(1001, 60000) < 60.0f && Ratio(4, 2) == 0.5f);
    if (!ok) { std::printf("009be380_TEST FAIL\n"); return 1; }
    std::printf("009be380_TEST PASS\n");
    return 0;
}