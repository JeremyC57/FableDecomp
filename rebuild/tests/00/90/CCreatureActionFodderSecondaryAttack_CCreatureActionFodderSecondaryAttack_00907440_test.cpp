// behaviour test for 0x00907440 CCreatureAction_FodderSecondaryAttack::CCreatureAction_FodderSecondaryAttack: the base is built with the owner, the target, action code 150 and the sound pair looked up for the owner under a null name, and the null-name string dies only after the base is built
#include <cstdio>
static int gStep = 0, gNameAt = -1, gSoundAt = -1, gBaseAt = -1, gDieAt = -1;
static const void* gOwner = 0; static const void* gTarget = 0; static const void* gSoundOwner = 0;
static long gCode = 0;
static const char* gName = "x";
struct Thing { int id; };
static void Construct(Thing& owner, Thing& target)          // the function under test
{
    const char* name = 0; gName = name; gNameAt = gStep++;                 // CCharString(0, -1)
    gSoundOwner = &owner; gSoundAt = gStep++;                               // GetSoundPair(&owner, name)
    gOwner = &owner; gTarget = &target; gCode = 150; gBaseAt = gStep++; // CCombatActionBase(owner, target, code, sound)
    gDieAt = gStep++;                                                        // ~CCharString
}
int main()
{
    int fail = 0;
    Thing owner, target;
    Construct(owner, target);
    if (gNameAt != 0 || gSoundAt != 1 || gBaseAt != 2 || gDieAt != 3) fail |= 1;
    if (gOwner != &owner || gTarget != &target || gSoundOwner != &owner) fail |= 2;
    if (gCode != 150 || gName != 0) fail |= 4;
    if (fail) { std::printf("00907440_TEST FAIL %d\n", fail); return 1; }
    std::printf("00907440_TEST PASS\n");
    return 0;
}