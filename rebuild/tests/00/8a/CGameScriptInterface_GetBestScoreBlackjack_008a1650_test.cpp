#include <cstdio>
// Behaviour model: the stored float score truncates toward zero; no creature gives 0.
static long best(const float* score) { return score ? (long)*score : 0; }
int main()
{
    float s = 41.9f;
    if (best(&s) != 41 || best(0) != 0) { std::printf("008a1650_TEST FAIL\n"); return 1; }
    std::printf("008a1650_TEST PASS\n");
    return 0;
}