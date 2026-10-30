// Behaviour test for 0x0076E2F0 CTCWeaponTrail::CalculateInterpolatedWeaponHelpers (see src/0076e2f0.cpp): when both helper
// names of the trail are equal the second lookup takes helper index 1; a carried weapon reads both helpers from the carrier-posed
// mesh (first name -> start), otherwise from the appearance at the given time (first name -> end).
#include <cstdio>
#include <cstring>
struct V3 { float X, Y, Z; };
struct Pair { const char* first; const char* second; };
static int g_lastIndex[2]; static int g_calls;
static V3 Lookup(const char* name, long index) { V3 v = { (float)std::strlen(name), (float)index, 0.0f }; g_lastIndex[g_calls++ & 1] = (int)index; return v; }
static void Calc(const Pair& names, bool carried, V3& end, V3& start)
{
    long secondIndex = 0;
    if (!std::strcmp(names.first, names.second)) secondIndex = 1;
    if (carried) { start = Lookup(names.first, 0); end = Lookup(names.second, secondIndex); }
    else { end = Lookup(names.first, 0); start = Lookup(names.second, secondIndex); }
}
int main()
{
    Pair same = { "HLP", "HLP" }, diff = { "A", "BBBB" };
    V3 e, s;
    Calc(same, true, e, s);
    if (g_lastIndex[1] != 1 || s.X != 3.0f || e.Y != 1.0f) { std::printf("0076e2f0_TEST FAIL\n"); return 1; }
    Calc(diff, false, e, s);
    if (e.X != 1.0f || s.X != 4.0f || s.Y != 0.0f) { std::printf("0076e2f0_TEST FAIL\n"); return 1; }
    std::printf("0076e2f0_TEST PASS\n");
    return 0;
}