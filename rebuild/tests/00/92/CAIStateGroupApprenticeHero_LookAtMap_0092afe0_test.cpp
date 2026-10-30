// behaviour test for 0x0092AFE0 CAIStateGroup_ApprenticeHero::LookAtMap: no map -> true; remember the map radius; a failed
// move or look-at -> false; otherwise true exactly when the look-at-map animation action completes (== 2)
#include <cstdio>
static float g_radius;
static bool LookAtMap(bool hasMap, float radius, long move, long look, long anim, int& animCalls)
{
    if (!hasMap) return true;
    static float lastMapRadius;
    lastMapRadius = radius;
    g_radius = lastMapRadius;
    if (move != 2) return false;
    if (look != 2) return false;
    ++animCalls;
    if (anim != 2) return false;
    return true;
}
int main()
{
    int calls = 0;
    bool ok = LookAtMap(false, 1, 2, 2, 2, calls) && calls == 0;
    ok = ok && !LookAtMap(true, 3.5f, 4, 2, 2, calls) && g_radius == 3.5f && calls == 0;
    ok = ok && !LookAtMap(true, 2.0f, 2, 1, 2, calls) && calls == 0;
    ok = ok && LookAtMap(true, 2.0f, 2, 2, 2, calls) && calls == 1;
    ok = ok && !LookAtMap(true, 2.0f, 2, 2, 3, calls) && calls == 2;
    std::printf(ok ? "0092afe0_TEST PASS\n" : "0092afe0_TEST FAIL\n");
    return ok ? 0 : 1;
}