// behaviour test for 0x00933F10 CCreatureAction_KickCreature::CCreatureAction_KickCreature: the base is built with the owner, (-1.0f, 0.8f), the animation looked up by "KICK" and a zeroed sound pair, before the PTarget pointer is pointed at the target
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
    gName = "KICK";                                      // GetAnimationFromName(owner, name)
    gA = -1.0f; gB = 0.8f; gSound0 = sound[0]; gSound1 = sound[1]; gBaseAt = gStep++;   // CCreatureActionBase(...)
    gItem = target; gSetAt = gStep++;                        // PTarget.SetPItem(target)
    (void)owner;
}
int main()
{
    int fail = 0;
    Thing owner, target;
    Construct(&owner, &target);
    if (gBaseAt != 0 || gSetAt != 1) fail |= 1;
    if (gA != -1.0f || gB != 0.8f) fail |= 2;
    if (std::strcmp(gName, "KICK") != 0 || gSound0 != 0 || gSound1 != 0) fail |= 4;
    if (gItem != &target) fail |= 8;
    if (fail) { std::printf("00933f10_TEST FAIL %d\n", fail); return 1; }
    std::printf("00933f10_TEST PASS\n");
    return 0;
}