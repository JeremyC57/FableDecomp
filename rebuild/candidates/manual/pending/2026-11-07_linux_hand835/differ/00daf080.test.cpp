// Behaviour test for NScript::CQ_NewOakValeIntroScript::CNOVI_TeddyGirl::Main (0x00DAF080): the girl who lost her teddy. Talked to
// while the hero carries the ungiveable teddy: the first time she tells her story (PLEA_POST_BEATEN if the hero hit her, else
// LOST_TEDDY), then asks GIVE_TEDDY (yes: FOUND_TEDDY + GivenTeddy; no: REPEAT_PLEA). Presented with the teddy she takes it; with any
// other item she says DONT_WANT until she has the teddy. Once the hero spoke of the teddy near the bully she says TEDDY_RUINED, sets
// the master TeddySolution to "C", runs off and is removed. Talked to without the teddy: POST_BEATEN / LOST_TEDDY / REPEAT_FOUND /
// BAD_FEELING / REPEAT_PLEA (the talk message holds for the frame, so a hand-over talk also gets REPEAT_FOUND). A hit makes her an ally, sets HeroHitMe, costs a bad deed and plays DONT_HIT. A state model.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Girl {
    int Frames, TerminateAt, TalkAt, HitAt, PresentAt, RuinAt; bool HasTeddy; const char* Present; int Answer;
    bool DoneIntro, HeroHitMe, FoundTeddy, Spoke, TeddyRuined, Removed; char Solution[4]; int BadDeeds;
    bool T() const { return Frames >= TerminateAt; }
    void GivenTeddy() { FoundTeddy = true; HasTeddy = false; Log("GivenTeddy"); }
    void Main()
    {
        for (; !T(); ++Frames) {
            if (Frames == RuinAt) Spoke = true;
            bool talked = Frames == TalkAt;
            if (talked && HasTeddy) {
                if (!DoneIntro && HeroHitMe) { Log("PLEA_POST_BEATEN"); DoneIntro = true; }
                else if (!DoneIntro) { Log("LOST_TEDDY"); DoneIntro = true; }
                Log("GIVE_TEDDY");
                if (Answer == 1) { Log("FOUND_TEDDY"); GivenTeddy(); } else Log("REPEAT_PLEA");
            } else if (Frames == PresentAt && !std::strcmp(Present, "OBJECT_TEDDY_BEAR_UNGIVEABLE")) {
                DoneIntro = true; Log("FOUND_TEDDY"); GivenTeddy();
            } else if (Frames == PresentAt) {
                if (!FoundTeddy) Log("DONT_WANT");
            }
            if (Spoke && !FoundTeddy) { Log("TEDDY_RUINED"); std::strcpy(Solution, "C"); Log("remove"); Removed = true; return; }
            else if (talked && !HasTeddy) {
                if (!FoundTeddy && HeroHitMe) { Log("PLEA_POST_BEATEN"); DoneIntro = true; }
                else if (!DoneIntro) { Log("LOST_TEDDY"); DoneIntro = true; }
                else if (FoundTeddy) Log("REPEAT_FOUND");
                else if (TeddyRuined) Log("BAD_FEELING");
                else Log("REPEAT_PLEA");
            }
            if (Frames == HitAt) { HeroHitMe = true; ++BadDeeds; Log("DONT_HIT"); }
        }
    }
};
int main()
{
    Girl a = { 0, 10, 3, 1, -1, -1, true, "", 1 };
    a.Main();
    bool ok = !std::strcmp(g_log, "DONT_HIT;PLEA_POST_BEATEN;GIVE_TEDDY;FOUND_TEDDY;GivenTeddy;REPEAT_FOUND;") && a.FoundTeddy && a.BadDeeds == 1;
    g_log[0] = 0;
    Girl b = { 0, 10, 4, -1, 2, 6, false, "OBJECT_APPLE", 0 };
    b.Main();
    ok = ok && !std::strcmp(g_log, "DONT_WANT;LOST_TEDDY;TEDDY_RUINED;remove;") && b.Removed && !std::strcmp(b.Solution, "C");
    g_log[0] = 0;
    Girl c = { 0, 10, 5, -1, 2, -1, false, "OBJECT_TEDDY_BEAR_UNGIVEABLE", 0 };
    c.Main();
    ok = ok && !std::strcmp(g_log, "FOUND_TEDDY;GivenTeddy;REPEAT_FOUND;") && c.DoneIntro && c.Frames == 10;
    std::printf(ok ? "00daf080_TEST PASS\n" : "00daf080_TEST FAIL\n");
    return ok ? 0 : 1;
}
