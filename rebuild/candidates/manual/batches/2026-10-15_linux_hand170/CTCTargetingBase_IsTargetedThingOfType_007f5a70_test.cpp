#include <cstdio>
// Behaviour model of CTCTargetingBase::IsTargetedThingOfType (0x007F5A70): a targeted thing with a CTCTargeted component whose
// targeting flags intersect the mask.
struct Targeted { unsigned long flags; };
struct Thing { Targeted* tc; };
static bool IsOfType(const Thing* t, unsigned long mask)
{
    if (t) { if (t->tc) return (t->tc->flags & mask) != 0; }
    return false;
}
int main()
{
    Targeted tc = { 0x6 }; Thing a = { &tc }, b = { 0 };
    if (!IsOfType(&a, 0x2) || IsOfType(&a, 0x1) || IsOfType(&b, 0x2) || IsOfType(0, 0x2)) { std::printf("007f5a70_TEST FAIL\n"); return 1; }
    std::printf("007f5a70_TEST PASS\n");
    return 0;
}
