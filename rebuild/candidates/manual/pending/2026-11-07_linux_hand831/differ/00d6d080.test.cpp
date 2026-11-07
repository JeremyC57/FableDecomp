// Behaviour test for 0x00D6D080 NScript::CQ_HangingTreeEvilScript::Introduction: models the cast map (HERO plus BAN1..BAN4 from the
// first four HTE_FreedomFighter handles), the quest start screen shown once (QuestStartScreenShown latch), IntroCutscenePlayed set
// after the three script frames, and the wait for BanditsStarted (one script frame per poll, abandoned when the thread terminates).
#include <stdio.h>
#include <string.h>

struct SState { bool QuestStartScreenShown, IntroCutscenePlayed, BanditsStarted; int StartScreens, Frames; };
static const char* CastName(int i) { static const char* n[] = { "HERO", "BAN1", "BAN2", "BAN3", "BAN4" }; return n[i]; }
static int CastHandle(int i, int hero, const int* bandits) { return i == 0 ? hero : bandits[i - 1]; }
// terminateAt: the script frame count at which IsActiveThreadTerminating starts returning true (-1 = never)
static bool Introduction(SState& s, int startAfter, int terminateAt)
{
    for (int f = 0; f < 3; ++f) { ++s.Frames; if (s.Frames == terminateAt) return false; }
    if (!s.QuestStartScreenShown) { ++s.StartScreens; s.QuestStartScreenShown = true; }
    s.IntroCutscenePlayed = true;
    while (!s.BanditsStarted) {
        ++s.Frames;
        if (s.Frames == startAfter) s.BanditsStarted = true;
        if (s.Frames == terminateAt) return false;
    }
    return true;
}

int main()
{
    int ok = 1;
    int bandits[4] = { 11, 12, 13, 14 };
    ok &= strcmp(CastName(0), "HERO") == 0 && CastHandle(0, 7, bandits) == 7;
    ok &= strcmp(CastName(4), "BAN4") == 0 && CastHandle(4, 7, bandits) == 14;
    SState a = { false, false, false, 0, 0 };
    ok &= Introduction(a, 6, -1) && a.StartScreens == 1 && a.IntroCutscenePlayed && a.Frames == 6;
    SState b = { true, false, false, 0, 0 };
    ok &= Introduction(b, 4, -1) && b.StartScreens == 0 && b.IntroCutscenePlayed;
    SState c = { false, false, false, 0, 0 };
    ok &= !Introduction(c, 99, 2) && !c.IntroCutscenePlayed && c.StartScreens == 0;
    SState d = { false, false, false, 0, 0 };
    ok &= !Introduction(d, 99, 5) && d.IntroCutscenePlayed && d.StartScreens == 1;
    if (ok) printf("00d6d080_TEST PASS\n"); else printf("00d6d080_TEST FAIL\n");
    return ok ? 0 : 1;
}
