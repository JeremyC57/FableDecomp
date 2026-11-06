// Behaviour test for NScript::CV_GhostGrannyNecklaceScript::CGhostGrannyNecklace::Main (0x00E8DFA0): the ghost granny's brain-state
// loop. INTRO: once the quest card is given and the hero comes near (talk timer clear), play CS_GHOSTGRANNY_GRANNY, set objective 2 and
// go to GIVING_CLUES; GIVING_CLUES: a clue when the clue timer is clear and the hero is near, the brief reprise when talked to, and
// NECKLACE_NOT_RETURNED once the bandit is killed or GOING_TO_REST once the necklace is back; GOING_TO_REST: the three farewell lines,
// the ghost effect, the fade-out kill and the final wait. A state model, run for the necklace path, the bandit path and a termination.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
enum { GS_INTRO, GS_GIVING_CLUES, GS_NECKLACE_NOT_RETURNED, GS_GOING_TO_REST };
struct Ghost {
    int BrainState, Frames, TerminateAt, NearFrom, CardAt, ReturnAt, BanditAt;
    bool QuestCardGiven, NecklaceReturned, BanditKilled, GhostToRest, HeroKnows;
    int ClueTimer, TalkTimer;
    void NewScriptFrame()
    {
        ++Frames;
        if (ClueTimer > 0) --ClueTimer;
        if (TalkTimer > 0) --TalkTimer;
        if (Frames == CardAt) QuestCardGiven = true;
        if (Frames == ReturnAt) NecklaceReturned = true;
        if (Frames == BanditAt) BanditKilled = true;
    }
    bool T() { return Frames >= TerminateAt; }
    bool Near() { return Frames >= NearFrom; }
    void Main()
    {
        NewScriptFrame();
        if (T()) return;
        while (BrainState == GS_INTRO) {
            NewScriptFrame();
            if (T()) return;
            if (Near() && TalkTimer == 0 && QuestCardGiven) {
                Log("CS_GHOSTGRANNY_GRANNY"); HeroKnows = true; Log("obj02");
                BrainState = GS_GIVING_CLUES; TalkTimer = 2;
            }
        }
        if (T()) return;
        while (BrainState == GS_GIVING_CLUES) {
            NewScriptFrame();
            if (T()) return;
            if (ClueTimer == 0 && Near()) { Log("clue"); ClueTimer = 3; }
            if (NecklaceReturned) BrainState = GS_GOING_TO_REST;
            if (BanditKilled) BrainState = GS_NECKLACE_NOT_RETURNED;
        }
        if (T()) return;
        while (BrainState == GS_NECKLACE_NOT_RETURNED) {
            NewScriptFrame();
            if (T()) return;
            if (NecklaceReturned) BrainState = GS_GOING_TO_REST;
            if (TalkTimer == 0 && Near()) { Log("not_returned"); TalkTimer = 3; }
        }
        if (T()) return;
        while (BrainState == GS_GOING_TO_REST) {
            NewScriptFrame();
            if (T()) return;
            if (Near() && !GhostToRest) {
                GhostToRest = true;
                Log("FAREWELL_10"); Log("FAREWELL_20"); Log("FAREWELL_30"); Log("Ghost_Appear_01"); Log("fade_kill");
                for (;;) { NewScriptFrame(); if (T()) return; }
            }
        }
    }
};
int main()
{
    Ghost a = { GS_INTRO, 0, 12, 0, 2, 6, -1, false, false, false, false, false, 0, 0 };
    a.Main();
    bool ok = !std::strcmp(g_log, "CS_GHOSTGRANNY_GRANNY;obj02;clue;clue;FAREWELL_10;FAREWELL_20;FAREWELL_30;Ghost_Appear_01;fade_kill;")
        && a.GhostToRest && a.HeroKnows && a.Frames == 12;
    g_log[0] = 0;
    Ghost b = { GS_INTRO, 0, 14, 0, 2, 9, 4, false, false, false, false, false, 0, 0 };
    b.Main();
    ok = ok && !std::strcmp(g_log, "CS_GHOSTGRANNY_GRANNY;obj02;clue;not_returned;not_returned;FAREWELL_10;FAREWELL_20;FAREWELL_30;Ghost_Appear_01;fade_kill;")
        && b.GhostToRest;
    g_log[0] = 0;
    Ghost c = { GS_INTRO, 0, 3, 0, 9, -1, -1, false, false, false, false, false, 0, 0 };
    c.Main();
    ok = ok && !std::strcmp(g_log, "") && c.BrainState == GS_INTRO && c.Frames == 3;
    std::printf(ok ? "00e8dfa0_TEST PASS\n" : "00e8dfa0_TEST FAIL\n");
    return ok ? 0 : 1;
}