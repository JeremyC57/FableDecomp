// Behaviour test for CAT_BanditAlly::Main (0x00F063B0): the frame step of the hireable bandit -- teleport once in Bowerstone Jail
// when not hired, leave Bowerstone Posh once when hired and following, switch allegiance (once, with one speech) after the hero's
// third attack, re-arm the invalid-region nag timer, take the FOLLOW / WAIT expressions, and accept the hire question.
#include <cstdio>
#include <cstring>

struct Bandit {
    bool Hired, Following, Teleported, Switched, RemovedFromPosh, ResetTimer;
    long Timer, Teleports, Removals, SwitchSpeeches, Nags;
    bool SwitchComplete;
    // returns false when the main loop ends (allegiance switched)
    bool Step(bool phase3, const char* region, long attackCount, bool regionInvalid, const char* expression, bool acceptHire)
    {
        if (phase3 && std::strcmp(region, "BowerstoneJail") == 0 && !Hired && !Teleported) { Teleported = true; ++Teleports; }
        else if (std::strcmp(region, "BowerstonePosh") == 0 && Hired && Following && !RemovedFromPosh) { RemovedFromPosh = true; ++Removals; }
        if (attackCount > 2 && !Switched) {
            if (!SwitchComplete) { ++SwitchSpeeches; SwitchComplete = true; }
            Switched = true;
            return false;
        }
        if (regionInvalid && Following) {
            if (ResetTimer) { Timer = 3; ResetTimer = false; }
            if (Timer == 0) { ++Nags; ResetTimer = true; }
        }
        if (expression) {
            if (std::strcmp(expression, "EXPRESSION_FOLLOW") == 0) {
                if (!Hired) { if (acceptHire) { Hired = true; Following = true; } }
                else Following = true;
            }
            else if (std::strcmp(expression, "EXPRESSION_WAIT") == 0) Following = false;
        }
        if (Timer > 0) --Timer;
        return true;
    }
};

int main()
{
    Bandit b = { false, false, false, false, false, true, 0, 0, 0, 0, 0, false };
    bool ok = true;
    ok = ok && b.Step(true, "BowerstoneJail", 0, false, 0, false) && b.Teleports == 1;
    ok = ok && b.Step(true, "BowerstoneJail", 0, false, 0, false) && b.Teleports == 1;
    ok = ok && b.Step(false, "Other", 0, false, "EXPRESSION_FOLLOW", false) && !b.Hired;
    ok = ok && b.Step(false, "Other", 0, false, "EXPRESSION_FOLLOW", true) && b.Hired && b.Following;
    ok = ok && b.Step(false, "Other", 0, true, 0, false) && b.Nags == 0 && b.Timer == 2;
    b.Timer = 0;
    ok = ok && b.Step(false, "Other", 0, true, 0, false) && b.Nags == 1 && b.ResetTimer;
    ok = ok && b.Step(false, "BowerstonePosh", 0, false, 0, false) && b.Removals == 1;
    ok = ok && b.Step(false, "BowerstonePosh", 0, false, "EXPRESSION_WAIT", false) && b.Removals == 1 && !b.Following;
    ok = ok && b.Step(false, "Other", 2, false, 0, false);
    ok = ok && !b.Step(false, "Other", 3, false, 0, false) && b.Switched && b.SwitchSpeeches == 1;
    if (!ok) { std::printf("00f063b0_TEST FAIL\n"); return 1; }
    std::printf("00f063b0_TEST PASS\n");
    return 0;
}