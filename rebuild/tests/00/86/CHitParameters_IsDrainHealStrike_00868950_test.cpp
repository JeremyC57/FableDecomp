// behaviour test for 0x00868950 CHitParameters::IsDrainHealStrike: true exactly when the damage map has an entry keyed 7, whatever its value, and false for an empty map
#include <cstdio>
#include <map>
static bool Query(const std::map<int, float>& damage)        // the function under test
{
    return damage.find(7) != damage.end();
}
int main()
{
    int fail = 0;
    std::map<int, float> m;
    if (Query(m)) fail |= 1;
    m[7 + 1] = 3.0f; m[7 - 1] = 2.0f;
    if (Query(m)) fail |= 2;
    m[7] = 0.0f;
    if (!Query(m)) fail |= 4;
    if (fail) { std::printf("00868950_TEST FAIL %d\n", fail); return 1; }
    std::printf("00868950_TEST PASS\n");
    return 0;
}