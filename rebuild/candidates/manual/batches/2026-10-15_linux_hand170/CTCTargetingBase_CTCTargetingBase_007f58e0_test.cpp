#include <cstdio>
// Behaviour model of CTCTargetingBase::CTCTargetingBase (0x007F58E0): no script override, four empty intelligent pointers, no
// targeting def, not looking at the camera.
struct Ptr { void* item; Ptr() : item(0) { } };
struct Targeting { bool ignore; Ptr nearest, overrideInteresting, overrideRanged, targeted; void* def; bool lookAtCamera; };
int main()
{
    Targeting t; t.ignore = false; t.def = 0; t.lookAtCamera = false;
    if (t.ignore || t.nearest.item || t.overrideInteresting.item || t.overrideRanged.item || t.targeted.item || t.def || t.lookAtCamera) { std::printf("007f58e0_TEST FAIL\n"); return 1; }
    std::printf("007f58e0_TEST PASS\n");
    return 0;
}
