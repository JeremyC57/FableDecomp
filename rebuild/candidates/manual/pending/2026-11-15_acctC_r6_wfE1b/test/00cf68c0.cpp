// Behaviour test for NScript::CQ_ArenaHoldingScriptScript::CEntranceGuard::Main (0x00CF68C0): the Arena entrance guard.
// Until IntroDone, per frame: a talk plays the greeting (only while he has health) and, with Q_Arena active, the entry line and the
// yes / no question (yes: NO_FOLLOWERS while a non-summoned follower is with the hero, else COME_IN, fade, start screen, teleport;
// no: RETURN); without the quest WELL_DONE (completed) / NEED_QUEST (white balverine finished) / NEED_MORE; a talk sets IntroDone.
// A hit is handled ONLY in a frame without a talk (retail: the talk block's end jumps past the hit test, 0x00CF75C0 / 0x00CF83E2):
// outside a hit cut-scene he says ATTACKED, heals (10000, then becomes the hero's ally; after the intro: camera default, heal 100).
// After the intro the RETURN_* lines are used, and once Q_Arena is complete the marker is cleared once (DoneArena), also in a talk frame.
// A state model of the script's decisions (the row's own code needs the game); the mutant "hit tested after the talk" fails.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct World {
    int Frame, TerminateAt, Talk[4], Hit[4];
    bool Active, Completed, WhiteBalverine, Alive;
    int Answer; int Followers[3]; int NFollowers;   // follower entry: 1 = summoned creature
    bool InHit;
};
struct Guard {
    World* W; bool IntroDone, DoneArena; bool HitAfterTalkMutant;
    bool T() const { return W->TerminateAt > 0 && W->Frame >= W->TerminateAt; }
    bool In(const int* v) const { for (int i = 0; i < 4; ++i) if (v[i] == W->Frame) return true; return false; }
    void Say(const char* s) { if (W->Alive) Log(s); }
    bool TalkBlock(bool intro)
    {
        if (T()) return false;
        if (intro) Say("GREETING");
        if (W->Active) {
            if (T()) return false;
            Say(intro ? "ENTRY" : "RETURN_GREETING");
            Log("question");
            if (T()) return false;
            if (W->Answer == 1) {
                if (T()) return false;
                bool has_followers = false;
                for (int i = 0; i < W->NFollowers; ++i) { if (T()) return false; if (W->Followers[i] != 1) has_followers = true; }
                if (T()) return false;
                if (has_followers) { if (T()) return false; Say("NO_FOLLOWERS"); Log("face"); }
                else { if (T()) return false; Say("COME_IN"); Log("fade"); Log("startscreen"); Log("teleport"); }
            } else { if (T()) return false; Say("RETURN"); Log("face"); }
        } else if (W->Completed) { if (T()) return false; Say(intro ? "WELL_DONE" : "RETURN_WELL_DONE"); Log("face"); }
        else if (W->WhiteBalverine) { if (T()) return false; Say(intro ? "NEED_QUEST" : "RETURN_NEED_QUEST"); Log("face"); }
        else { if (T()) return false; Say(intro ? "NEED_MORE" : "RETURN_NEED_MORE"); Log("face"); }
        return true;
    }
    bool HitBlock(bool intro)
    {
        if (T()) return false;
        if (!W->InHit) {
            if (T()) return false;
            W->InHit = true;
            Say("ATTACKED");
            if (intro) { Log("heal10000"); Log("ally"); } else { Log("camera"); Log("heal100"); }
            Log("face");
            W->InHit = false;
        }
        return true;
    }
    void Main()
    {
        if (T()) return;
        Log("setup");
        while (!IntroDone) {
            ++W->Frame;
            if (T()) return;
            bool talked = In(W->Talk);
            if (talked) { if (!TalkBlock(true)) return; IntroDone = true; }
            if ((!talked || HitAfterTalkMutant) && In(W->Hit)) { if (!HitBlock(true)) return; }
        }
        if (T()) return;
        for (;; ++W->Frame) {
            if (T()) return;
            bool talked = In(W->Talk);
            if (talked) { if (!TalkBlock(false)) return; }
            if ((!talked || HitAfterTalkMutant) && In(W->Hit)) { if (!HitBlock(false)) return; }
            if (!DoneArena && W->Completed) { if (T()) return; Log("clearinfo"); DoneArena = true; }
        }
    }
};
static bool Run(World w, bool mutant, const char* expect, bool intro, bool done)
{
    Guard g = { &w, false, false, mutant };
    g_log[0] = 0; g.Main();
    bool ok = !std::strcmp(g_log, expect) && g.IntroDone == intro && g.DoneArena == done;
    if (!ok && !mutant) std::printf("got %s\n", g_log);
    return ok;
}
int main()
{
    // quest active, yes, one non-summoned follower; hit in frame 2 (no talk), talk + hit in frame 3, return talk + hit in frame 5
    World a = { 0, 7, { 3, 5, 0, 0 }, { 2, 3, 5, 6 }, true, false, false, true, 1, { 0, 1 }, 2, false };
    const char* ea = "setup;ATTACKED;heal10000;ally;face;GREETING;ENTRY;question;NO_FOLLOWERS;face;"
                     "RETURN_GREETING;question;NO_FOLLOWERS;face;RETURN_GREETING;question;NO_FOLLOWERS;face;ATTACKED;camera;heal100;face;";
    // quest completed: WELL_DONE, then the marker cleared once; dead guard says nothing
    World b = { 0, 6, { 2, 4, 0, 0 }, { 0, 0, 0, 0 }, false, true, false, true, 0, { 0 }, 0, false };
    const char* eb = "setup;GREETING;WELL_DONE;face;RETURN_WELL_DONE;face;clearinfo;RETURN_WELL_DONE;face;";
    World c = { 0, 5, { 2, 0, 0, 0 }, { 0, 0, 0, 0 }, true, false, false, false, 1, { 1 }, 1, false };
    const char* ec = "setup;question;fade;startscreen;teleport;question;fade;startscreen;teleport;";
    // answer no: RETURN; the talk frame is seen again by the second loop (no NewScriptFrame between the loops)
    World d = { 0, 4, { 3, 0, 0, 0 }, { 1, 0, 0, 0 }, true, false, false, true, 0, { 0 }, 0, false };
    const char* ed = "setup;ATTACKED;heal10000;ally;face;GREETING;ENTRY;question;RETURN;face;RETURN_GREETING;question;RETURN;face;";
    World e = { 0, 3, { 2, 0, 0, 0 }, { 0, 0, 0, 0 }, false, false, true, true, 0, { 0 }, 0, false };
    const char* ee = "setup;GREETING;NEED_QUEST;face;RETURN_NEED_QUEST;face;";
    World f = { 0, 2, { 2, 0, 0, 0 }, { 0, 0, 0, 0 }, false, false, false, true, 0, { 0 }, 0, false };
    World h = { 0, 3, { 1, 0, 0, 0 }, { 0, 0, 0, 0 }, false, false, false, true, 0, { 0 }, 0, false };
    bool ok = Run(a, false, ea, true, false) && Run(b, false, eb, true, true) && Run(c, false, ec, true, false)
           && Run(d, false, ed, true, false) && Run(e, false, ee, true, false)
           && Run(f, false, "setup;", false, false) && Run(h, false, "setup;GREETING;NEED_MORE;face;RETURN_NEED_MORE;face;", true, false);
    bool mutant_caught = !Run(a, true, ea, true, false);
    ok = ok && mutant_caught;
    std::printf(ok ? "00cf68c0_TEST PASS\n" : "00cf68c0_TEST FAIL\n");
    return ok ? 0 : 1;
}
