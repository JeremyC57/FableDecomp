// Behaviour test for NCreatureMode::IsAbleToBeEngagedInCombat 0x0085FBC0: false for a physical thing (+0x6C bit 1) without the
// +0xBC 0x20 flag, false underground, false while the mode manager (interface 0x31) is in mode 0x1D or 0x51, false for the
// CREATURE_DRAGON_01 def; otherwise true. The thing, mode manager and def are test models of the row's accessors.
#include <cstdio>
#include <cstring>

struct TestThing {
    unsigned long ThingTypeFlags;          // +0x6C
    unsigned char PhysicalFlags;           // +0xBC
    bool Underground;                      // IsUnderground() result
    bool HasModeManager;                   // HasTC(0x31)
    long Mode;                             // GetCurrentMode().GetType()
    const char* DefName;                   // PBaseDef->GetDefName()
    int ModeQueries;
};
static bool IsUnderground(const TestThing& thing) { return thing.Underground; }
static bool DefNameIs(const TestThing& thing, const char* text) { return std::strcmp(thing.DefName, text) == 0; }

static bool IsAbleToBeEngagedInCombat(TestThing& thing)
{
    if ((thing.ThingTypeFlags & 2) && !(thing.PhysicalFlags & 0x20))
        return false;
    else if (IsUnderground(thing))
        return false;
    else {
        if (thing.HasModeManager) {
            ++thing.ModeQueries;
            long mode = thing.Mode;
            if (mode == 0x1d || mode == 0x51)
                return false;
        }
        return DefNameIs(thing, "CREATURE_DRAGON_01") ? false : true;
    }
}
static int fail(int n) { std::printf("0085fbc0_TEST FAIL %d\n", n); return 1; }
int main()
{
    TestThing t = { 3, 0x20, false, true, 4, "CREATURE_HOBBE", 0 };
    if (!IsAbleToBeEngagedInCombat(t) || t.ModeQueries != 1) return fail(1);
    t.PhysicalFlags = 0;                               // physical without the flag
    if (IsAbleToBeEngagedInCombat(t) || t.ModeQueries != 1) return fail(2);
    t.ThingTypeFlags = 1;                              // bit 1 clear: the flag no longer matters
    if (!IsAbleToBeEngagedInCombat(t)) return fail(3);
    t.Underground = true;
    if (IsAbleToBeEngagedInCombat(t)) return fail(4);
    t.Underground = false; t.Mode = 0x1d;
    if (IsAbleToBeEngagedInCombat(t)) return fail(5);
    t.Mode = 0x51;
    if (IsAbleToBeEngagedInCombat(t)) return fail(6);
    t.HasModeManager = false;                          // no manager: the mode is not consulted
    int q = t.ModeQueries;
    if (!IsAbleToBeEngagedInCombat(t) || t.ModeQueries != q) return fail(7);
    t.DefName = "CREATURE_DRAGON_01";
    if (IsAbleToBeEngagedInCombat(t)) return fail(8);
    std::printf("0085fbc0_TEST PASS\n");
    return 0;
}