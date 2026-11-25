// Behaviour test for 0x004DF3B4 CObstacleNavigatorExternalInterface::FindPosZAt: returns 0.0f (retail's pooled __real@00000000, 0x0122DEDC).
// NF418 (2026-11-25): linked behaviour test: the row function is declared here, not defined, so the test links the row object (and
// defines its callees under the row's spellings); it asserts the value of the float literal the row now spells (a mutated literal fails).
#include <cstdio>
class C3DVector;
class CObstacleNavigatorExternalInterface { public: virtual float FindPosZAt(const class C3DVector&, bool) const; };
static float g_vec[3] = { 1.0f, 2.0f, 3.0f };
int main()
{
    CObstacleNavigatorExternalInterface o;
    float r = o.FindPosZAt(*(const C3DVector*)g_vec, true);
    if (r != 0.0f) { std::printf("BAD %f\n", r); return 1; }
    std::printf("OK_0x004df3b4\n");
    return 0;
}
