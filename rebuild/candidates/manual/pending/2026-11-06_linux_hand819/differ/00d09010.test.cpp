// Behaviour test for NScript::CQ_BanditCampBossBattleScript::CCampHostageGuard::Main (0x00D09010): until the hostages are rescued, the
// hostage chat (counts 0..6, the seventh line back to 5), the pick-pocket flag, the first / second talk answer, and a hit (or a killed
// hostage) turns him on the hero for good; after the rescue, the late chat (0..5, the sixth attacks), the late talk lines and the late
// hit. A simulation of the same control flow with a scripted game.
#include <cstdio>
#include <cstring>
struct Sim {
    char log[2048]; int frame, termFrame, chatCount, rescueAt, hitAt, talkAt[4], nTalk, timer; bool dropPass, talkedTo, attacked, hostageKilled, pickpocket, near;
    bool T() const { return frame >= termFrame; }
    void Ev(const char* e) { std::strcat(log, e); std::strcat(log, ";"); }
    bool Talk() { for (int i = 0; i < nTalk; ++i) if (talkAt[i] == frame) return true; return false; }
    void NSF() { ++frame; if (timer > 0) --timer; }
    void Main()
    {
        static const int next1[7] = {1,2,3,4,5,6,5};
        dropPass = true;
        while (frame < rescueAt) {
            NSF(); if (T()) return;
            if (pickpocket) dropPass = false;
            if (near && timer <= 0) { chatCount = next1[chatCount]; timer = 10; Ev("CHAT"); }
            if (Talk()) { Ev(talkedTo ? "SECOND_CHAT" : "FIRST_CHAT"); talkedTo = true; }
            if (frame == hitAt || hostageKilled) { Ev("ATTACK"); attacked = true; for (;;) { NSF(); if (T()) return; } }
        }
        if (T()) return;
        talkedTo = false; chatCount = 0;
        for (;;) {
            if (near && timer <= 0) { ++chatCount; timer = 10; Ev("LATE"); if (chatCount == 6) { Ev("ATTACK"); attacked = true; for (;;) { NSF(); if (T()) return; } } }
            NSF(); if (T()) return;
        }
    }
};
static void Make(Sim& s) { std::memset(&s, 0, sizeof(s)); s.termFrame = 200; s.rescueAt = 30; s.hitAt = -1; }
int main()
{
    Sim a; Make(a); a.nTalk = 2; a.talkAt[0] = 3; a.talkAt[1] = 6; a.near = true; a.Main();
    bool ok = a.attacked && std::strstr(a.log, "FIRST_CHAT;") && std::strstr(a.log, "SECOND_CHAT;") && a.chatCount == 6 && a.dropPass;
    Sim b; Make(b); b.hostageKilled = true; b.pickpocket = true; b.Main();
    ok = ok && b.attacked && !b.dropPass && std::strcmp(b.log, "ATTACK;") == 0 && b.frame == 200;
    Sim c; Make(c); c.Main();
    ok = ok && !c.attacked && c.log[0] == 0;
    std::printf(ok ? "00d09010_TEST PASS\n" : "00d09010_TEST FAIL\n");
    return ok ? 0 : 1;
}
