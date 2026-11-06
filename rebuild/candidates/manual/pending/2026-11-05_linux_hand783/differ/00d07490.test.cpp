// Behaviour test for NScript::CQ_BanditCampBossBattleScript::CGate3Guard::Main() (0x00D07490): a model of the row -- scripted control,
// the quest-card objective, then per frame: a hero hit (plain, or a special ability other than 14) turns the guard hostile for good;
// while Gate3 is shut a timer-gated comment (FIRST, then SECOND every 10 s within 8 units) and, when talked to, a chat line in a
// paused movie (FIRST_CHAT once, then SECOND_CHAT); the thread ends when it is terminated.
#include <stdio.h>
#include <string.h>
struct World {
    int frame, terminateAt, hitAt, talkAt[4], nTalk; bool gate3Open; int timer; bool near;
    const char* lines[16]; int nLines; int paused, unpaused; bool hostile; int objective;
};
static bool Terminating(World& w) { return w.frame >= w.terminateAt; }
static bool TalkedTo(World& w) { for (int i = 0; i < w.nTalk; ++i) if (w.talkAt[i] == w.frame) return true; return false; }
static void Run(World& w)
{
    bool chatted = false, commented = false;
    w.timer = 0; w.objective = 1;
    while (!Terminating(w)) {
        if (w.frame == w.hitAt) {
            w.hostile = true;
            for (;;) { ++w.frame; if (Terminating(w)) return; }
        }
        if (!w.gate3Open) {
            if (w.timer <= 0 && w.near) {
                w.lines[w.nLines++] = commented ? "COMMENT_SECOND" : "COMMENT_FIRST";
                commented = true;
                w.timer = 10;
            }
            if (TalkedTo(w)) {
                ++w.paused;
                w.lines[w.nLines++] = chatted ? "SECOND_CHAT" : "FIRST_CHAT";
                chatted = true;
                ++w.unpaused;
            }
        }
        ++w.frame; if (w.timer > 0) --w.timer;
    }
}
int main()
{
    int ok = 1;
    World a = {}; a.terminateAt = 25; a.hitAt = -1; a.near = true; a.talkAt[0] = 3; a.talkAt[1] = 7; a.nTalk = 2;
    Run(a);
    ok &= a.objective == 1 && a.nLines == 5 && !strcmp(a.lines[0], "COMMENT_FIRST") && !strcmp(a.lines[1], "FIRST_CHAT");
    ok &= !strcmp(a.lines[2], "SECOND_CHAT") && !strcmp(a.lines[3], "COMMENT_SECOND") && a.paused == 2 && a.unpaused == 2 && !a.hostile;
    World b = {}; b.terminateAt = 20; b.hitAt = 2; b.near = false; b.nTalk = 0;
    Run(b);
    ok &= b.hostile && b.frame == 20 && b.nLines == 0;
    World c = {}; c.terminateAt = 10; c.hitAt = -1; c.gate3Open = true; c.near = true; c.talkAt[0] = 1; c.nTalk = 1;
    Run(c);
    ok &= c.nLines == 0 && c.paused == 0;
    printf(ok ? "00d07490_TEST PASS\n" : "00d07490_TEST FAIL\n");
    return ok ? 0 : 1;
}
