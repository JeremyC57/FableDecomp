// Behaviour test for NScript::CQ_EndGameBossBattleScript::CTheRealGuildmaster::Main (0x00D33730): the dying Guildmaster waits for the
// hero at the cut-scene trigger, plays CS_GM_DEATH once (GuildmasterChat), then each frame offers the HELP_ASIDE line when the hero is
// within 5.5 and the 15 s aside timer has run out, answers a talk with CS_GM_DYING (FIRST_CHAT once, then SECOND_CHAT) and a hero hit
// with CS_GM_DYING (HIT). A frame-stepped model with a timer that counts one per frame.
#include <cstdio>
#include <cstring>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Guildmaster {
    int Frame, TerminateAt, TriggerAt, NearFrom, Talk1, Talk2, HitAt, Timer;
    bool Chat;
    void NSF() { ++Frame; if (Timer > 0) --Timer; }
    bool T() const { return Frame >= TerminateAt; }
    void Main()
    {
        Log("dying");
        while (Frame < TriggerAt) { NSF(); if (T()) return; }
        if (T()) return;
        if (!Chat) { Chat = true; Log("GM_DEATH"); }
        Log("dying"); Log("start_screen");
        bool chatted = false;
        Timer = 15;
        for (;; NSF()) {
            if (T()) return;
            if (Frame >= NearFrom && Timer <= 0) { Log("ASIDE"); Timer = 15; }
            if (Frame == Talk1 || Frame == Talk2) {
                if (chatted) Log("SECOND_CHAT");
                else { chatted = true; Log("FIRST_CHAT"); }
            }
            if (Frame == HitAt) Log("HIT");
        }
    }
};
int main()
{
    // trigger at 2 (timer 15 -> 0 at frame 17); near from 10: aside at 17 and 32; talks at 5 and 20; hit at 25; terminate at 33
    Guildmaster a = { 0, 33, 2, 10, 5, 20, 25, 0, false };
    a.Main();
    bool ok = !std::strcmp(g_log, "dying;GM_DEATH;dying;start_screen;FIRST_CHAT;ASIDE;SECOND_CHAT;HIT;ASIDE;") && a.Chat && a.Frame == 33;
    g_log[0] = 0;
    Guildmaster b = { 0, 6, 1, 100, 3, 100, 100, 0, true };
    b.Main();
    ok = ok && !std::strcmp(g_log, "dying;dying;start_screen;FIRST_CHAT;") && b.Frame == 6;
    g_log[0] = 0;
    Guildmaster c = { 0, 3, 10, 0, 0, 0, 0, 0, false };
    c.Main();
    ok = ok && !std::strcmp(g_log, "dying;") && !c.Chat && c.Frame == 3;
    std::printf(ok ? "00d33730_TEST PASS\n" : "00d33730_TEST FAIL\n");
    return ok ? 0 : 1;
}