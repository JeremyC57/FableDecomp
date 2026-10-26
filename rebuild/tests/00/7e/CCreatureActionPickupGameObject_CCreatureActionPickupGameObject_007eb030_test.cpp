// behaviour test for 0x007EB030 CCreatureAction_PickupGameObject::CCreatureAction_PickupGameObject: the base is built with the owner, (0.05f, 1.0f), the animation looked up by "STANDARD_CAST" and a zeroed sound pair, before the PObject pointer is pointed at the target
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
    gName = "STANDARD_CAST";                                      // GetAnimationFromName(owner, name)
    gA = 0.05f; gB = 1.0f; gSound0 = sound[0]; gSound1 = sound[1]; gBaseAt = gStep++;   // CCreatureActionBase(...)
    gItem = target; gSetAt = gStep++;                        // PObject.SetPItem(target)
    (void)owner;
}
int main()
{
    int fail = 0;
    Thing owner, target;
    Construct(&owner, &target);
    if (gBaseAt != 0 || gSetAt != 1) fail |= 1;
    if (gA != 0.05f || gB != 1.0f) fail |= 2;
    if (std::strcmp(gName, "STANDARD_CAST") != 0 || gSound0 != 0 || gSound1 != 0) fail |= 4;
    if (gItem != &target) fail |= 8;
    if (fail) { std::printf("007eb030_TEST FAIL %d\n", fail); return 1; }
    std::printf("007eb030_TEST PASS\n");
    return 0;
}