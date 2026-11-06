// Behaviour test for NScript::CQ_RansomVictimScript::CVictimsFatherGuard::Main (0x00DE2610): until extra assistance is given the chief's
// guard answers talk with the protecting-the-chief line, counts the hero's hits (lines ON_HIT_10 / 20 / 30; the third makes him an
// enemy, the others keep him an ally) and turns on the hero when the chief was hit three times; then he follows the hero and answers talk
// with the waiting-outside / guards-killed / on-talk line, goes on counting hits, and stops following when told to wait outside until the
// victim is rescued. A state model, run for the follow path, the third-hit path and the chief-hit path.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Guard {
    int Frames, TerminateAt, TalkAt, HitAt1, HitAt2, HitAt3, AssistAt, ChiefHitAt, WaitAt, RescueAt;
    long TimesHit, HitChief, FollowState;
    bool ExtraAssistanceGiven, ToldGuardToWaitOutside, RescuedVictim, HelperAlive, Hostile;
    void NewScriptFrame()
    {
        ++Frames;
        if (Frames == AssistAt) ExtraAssistanceGiven = true;
        if (Frames == ChiefHitAt) HitChief = 3;
        if (Frames == WaitAt) ToldGuardToWaitOutside = true;
        if (Frames == RescueAt) RescuedVictim = true;
    }
    bool T() { return Frames >= TerminateAt; }
    bool Talked() { return Frames == TalkAt; }
    bool Hit() { return Frames == HitAt1 || Frames == HitAt2 || Frames == HitAt3; }
    void OnHit()
    {
        ++TimesHit;
        Log(TimesHit == 1 ? "ON_HIT_10" : TimesHit == 2 ? "ON_HIT_20" : "ON_HIT_30");
    }
    void Main()
    {
        NewScriptFrame();
        if (T()) return;
        while (!ExtraAssistanceGiven) {
            NewScriptFrame();
            if (T()) return;
            if (Talked()) Log("PROTECTING_CHIEF");
            if (Hit()) {
                OnHit();
                if (TimesHit >= 3) { Hostile = true; Log("enemy"); for (;;) { NewScriptFrame(); if (T()) return; } }
                Log("ally");
            }
            if (HitChief > 2) { Log("HIT_CHIEF_THIRD_TIME"); Hostile = true; for (;;) { NewScriptFrame(); if (T()) return; } }
        }
        if (T()) return;
        Log("follow");
        for (;;) {
            if (Talked()) {
                if (ToldGuardToWaitOutside && !RescuedVictim && HelperAlive) Log("WAITIING_OUTSIDE");
                else if (ToldGuardToWaitOutside && !RescuedVictim && !HelperAlive) Log("KILLED_GUARDS");
                else Log("ON_TALK");
            }
            if (Hit()) {
                OnHit();
                if (TimesHit >= 3) { Hostile = true; Log("enemy"); for (;;) { NewScriptFrame(); if (T()) return; } }
                Log("ally");
            }
            if (FollowState == 0 && ToldGuardToWaitOutside && !RescuedVictim) { Log("stop"); FollowState = 1; }
            if (FollowState == 1 && RescuedVictim) { Log("refollow"); FollowState = 2; }
            NewScriptFrame();
            if (T()) return;
        }
    }
};
int main()
{
    Guard a = { 0, 12, 7, 3, -1, -1, 5, -1, 8, 10, 0, 0, 0, false, false, false, true, false };
    a.Main();
    bool ok = !std::strcmp(g_log, "ON_HIT_10;ally;follow;ON_TALK;stop;refollow;") && a.FollowState == 2 && a.TimesHit == 1 && !a.Hostile;
    g_log[0] = 0;
    Guard b = { 0, 12, 2, 3, 4, 7, 5, -1, 6, -1, 0, 0, 0, false, false, false, false, false };
    b.Main();
    ok = ok && !std::strcmp(g_log, "PROTECTING_CHIEF;ON_HIT_10;ally;ON_HIT_20;ally;follow;stop;ON_HIT_30;enemy;") && b.Hostile && b.TimesHit == 3;
    g_log[0] = 0;
    Guard c = { 0, 9, -1, -1, -1, -1, 20, 4, -1, -1, 0, 0, 0, false, false, false, true, false };
    c.Main();
    ok = ok && !std::strcmp(g_log, "HIT_CHIEF_THIRD_TIME;") && c.Hostile && c.Frames == 9;
    std::printf(ok ? "00de2610_TEST PASS\n" : "00de2610_TEST FAIL\n");
    return ok ? 0 : 1;
}
