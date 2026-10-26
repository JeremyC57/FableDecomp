// behaviour test for 0x009405F0 CCreatureAction_PickCrop::CCreatureAction_PickCrop: the base is built with the owner, (-1.0f, 1.0f), the animation looked up by "PICKING" and a zeroed sound pair, before the PObjectToUse pointer is pointed at the target
#include <cstdio>
#include <cstring>
static int gStep = 0, gBaseAt = -1, gSetAt = -1;
static float gA = 0, gB = 0;
static const char* gName = 0;
static long gSound0 = 1, gSound1 = 1;
static const void* gItem = 0;
struct Thing { int id; };
static void Construct(Thing* owner, Thing* target)          // the function under test
{
    long sound[2]; sound[0] = 0; sound[1] = 0;
    gName = "PICKING";                                      // GetAnimationFromName(owner, name)
    gA = -1.0f; gB = 1.0f; gSound0 = sound[0]; gSound1 = sound[1]; gBaseAt = gStep++;   // CCreatureActionBase(...)
    gItem = target; gSetAt = gStep++;                        // PObjectToUse.SetPItem(target)
    (void)owner;
}
int main()
{
    int fail = 0;
    Thing owner, target;
    Construct(&owner, &target);
    if (gBaseAt != 0 || gSetAt != 1) fail |= 1;
    if (gA != -1.0f || gB != 1.0f) fail |= 2;
    if (std::strcmp(gName, "PICKING") != 0 || gSound0 != 0 || gSound1 != 0) fail |= 4;
    if (gItem != &target) fail |= 8;
    if (fail) { std::printf("009405f0_TEST FAIL %d\n", fail); return 1; }
    std::printf("009405f0_TEST PASS\n");
    return 0;
}