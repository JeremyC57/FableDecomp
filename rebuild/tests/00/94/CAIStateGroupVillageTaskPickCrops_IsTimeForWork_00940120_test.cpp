#include <cstdio>
// Behaviour model: true only with the component, a village and the matching time.
static bool test(bool hasMember, bool hasVillage, bool time) { return hasMember && hasVillage && time; }
int main()
{
    if (!test(true, true, true) || test(false, true, true) || test(true, false, true) || test(true, true, false)) { std::printf("00940120_TEST FAIL\n"); return 1; }
    std::printf("00940120_TEST PASS\n");
    return 0;
}