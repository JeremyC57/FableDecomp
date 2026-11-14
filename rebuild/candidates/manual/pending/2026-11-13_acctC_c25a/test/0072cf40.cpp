// Behaviour test for 0x0072CF40 CTCSpecialAbilities::UseSpecialAbility(EHeroAbility): an absent (0) or unavailable ability returns false
// with no event; otherwise one performed-ability event (carrying the ability) is added, the ability's own Setup member runs (each of the
// 18 abilities maps to its own Setup; raise dead and unknown values run none and fail), and on success a deed is posted at the
// creature's position when it has a deed log: 0x11 for most abilities, 0x3D for heal life, none for berserk. The Setup result is returned.
#include <cstdio>
#include <vector>
struct Log { std::vector<int> events, setups, deeds; };
static bool setupResult = true;
static bool Use(int ability, bool available, bool hasDeedLog, Log& log)
{
    if (ability == 0 || !available) return false;
    log.events.push_back(ability);
    int deed = 0x11; bool success = false;
    switch (ability) {
    case 1: case 2: case 3: case 4: case 5: case 6: case 9: case 10: case 11: case 12: case 13: case 15: case 16: case 17: case 18: case 19:
        log.setups.push_back(ability); success = setupResult; break;
    case 7: success = false; break;
    case 8: deed = 0; log.setups.push_back(ability); success = setupResult; break;
    case 14: deed = 0x3D; log.setups.push_back(ability); success = setupResult; break;
    }
    if (success && deed != 0 && hasDeedLog) log.deeds.push_back(deed);
    return success;
}
static int fails = 0;
static void check(bool c, const char* w) { if (!c) { ++fails; std::printf("FAIL %s\n", w); } }
int main() {
    Log l; check(!Use(0, true, true, l) && l.events.empty(), "null ability");
    l = Log(); check(!Use(3, false, true, l) && l.events.empty(), "unavailable");
    l = Log(); check(Use(3, true, true, l) && l.events.size() == 1 && l.events[0] == 3 && l.setups[0] == 3 && l.deeds.size() == 1 && l.deeds[0] == 0x11, "enflame");
    l = Log(); check(Use(14, true, true, l) && l.deeds.size() == 1 && l.deeds[0] == 0x3D, "heal life deed");
    l = Log(); check(Use(8, true, true, l) && l.setups[0] == 8 && l.deeds.empty(), "berserk no deed");
    l = Log(); check(!Use(7, true, true, l) && l.events.size() == 1 && l.setups.empty() && l.deeds.empty(), "raise dead fails after the event");
    l = Log(); check(!Use(25, true, true, l) && l.events.size() == 1 && l.setups.empty(), "unknown ability");
    l = Log(); check(Use(19, true, false, l) && l.deeds.empty(), "no deed log");
    setupResult = false; l = Log(); check(!Use(2, true, true, l) && l.setups[0] == 2 && l.deeds.empty(), "setup failure");
    if (fails == 0) std::printf("0072cf40_TEST PASS\n");
    return fails;
}
