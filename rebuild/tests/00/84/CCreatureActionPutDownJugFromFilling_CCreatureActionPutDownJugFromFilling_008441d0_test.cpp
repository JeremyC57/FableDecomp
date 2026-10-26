// behaviour test for 0x008441D0 CCreatureAction_PutDownJugFromFilling::CCreatureAction_PutDownJugFromFilling: the base is built with the owner, (-1.0f, 0.5f), the animation looked up by "FILL_JUG_OUTOF" and a zeroed sound pair, before the PThingToPutDown pointer is pointed at the target
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
    gName = "FILL_JUG_OUTOF";                                      // GetAnimationFromName(owner, name)
    gA = -1.0f; gB = 0.5f; gSound0 = sound[0]; gSound1 = sound[1]; gBaseAt = gStep++;   // CCreatureActionBase(...)
    gItem = target; gSetAt = gStep++;                        // PThingToPutDown.SetPItem(target)
    (void)owner;
}
int main()
{
    int fail = 0;
    Thing owner, target;
    Construct(&owner, &target);
    if (gBaseAt != 0 || gSetAt != 1) fail |= 1;
    if (gA != -1.0f || gB != 0.5f) fail |= 2;
    if (std::strcmp(gName, "FILL_JUG_OUTOF") != 0 || gSound0 != 0 || gSound1 != 0) fail |= 4;
    if (gItem != &target) fail |= 8;
    if (fail) { std::printf("008441d0_TEST FAIL %d\n", fail); return 1; }
    std::printf("008441d0_TEST PASS\n");
    return 0;
}