// Behaviour test for CQ_NewOakValeIntroScript::CNOVI_AffairMan::Main (0x00DB09E0): asked to keep quiet, "yes" adds bad deed 4,
// gives one gold and remembers the deal, "no" remembers the threat; either way the encounter ends. The dirty-words index steps
// by 10 and wraps from 60 back to 10; at index 10 the woman speaks first.
#include <cstdio>

struct AffairMan {
    bool EncounterOver, HeroAgreedToKeepQuiet, HeroSaidHeWouldReportMan; long BadgerIndex, Gold, BadDeed;
    void Ask(bool yes)
    {
        if (yes) { BadDeed = 4; Gold += 1; HeroAgreedToKeepQuiet = true; }
        else HeroSaidHeWouldReportMan = true;
        EncounterOver = true;
    }
    bool WomanFirst()
    {
        BadgerIndex = BadgerIndex + 10;
        if (BadgerIndex > 50) BadgerIndex = 10;
        return BadgerIndex == 10;
    }
};

int main()
{
    AffairMan m = { false, false, false, 0, 0, 0 };
    m.Ask(true);
    bool ok = m.EncounterOver && m.HeroAgreedToKeepQuiet && m.Gold == 1 && m.BadDeed == 4;
    AffairMan n = { false, false, false, 0, 0, 0 };
    n.Ask(false);
    ok = ok && n.EncounterOver && n.HeroSaidHeWouldReportMan && n.Gold == 0;
    ok = ok && m.WomanFirst();
    for (int i = 0; i < 4; ++i) ok = ok && !m.WomanFirst();
    ok = ok && m.BadgerIndex == 50 && m.WomanFirst();
    if (!ok) { std::printf("00db09e0_TEST FAIL\n"); return 1; }
    std::printf("00db09e0_TEST PASS\n");
    return 0;
}