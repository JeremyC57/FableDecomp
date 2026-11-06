// Behaviour test for NScript::CQ_ArenaHoldingScriptScript::CFanMarker::Main (0x00CF4EC0): until the fans may leave (or Q_Arena is
// done), an attack plays the hit scene (a movie the first time, a conversation when a hit scene runs already) and frees the fans;
// otherwise with the hero close and the chat timer down the next of the 14 early lines (count wraps 12 <-> 13 through the insults);
// then the 7 late lines (the first gives the helmet, the seventh ends the arena), and finally idling. A simulation of the control flow.
#include <cstdio>
#include <cstring>
struct Sim {
    char log[2048]; int frame, termFrame, chatCount, timer, attackAt, questDoneAt, lateEnd; bool canLeave, inHit, insult, arenaOver, helmet, near;
    bool T() const { return frame >= termFrame; }
    void Ev(const char* e) { std::strcat(log, e); std::strcat(log, ";"); }
    void NSF() { ++frame; if (timer > 0) --timer; }
    void Main()
    {
        static const int next[14] = {1,2,3,4,5,6,7,8,9,10,11,12,13,12};
        NSF(); if (T()) return;
        while (!canLeave && frame < questDoneAt) {
            NSF(); if (T()) return;
            if (frame == attackAt) { Ev(inHit ? "HITCONVO" : "HITMOVIE"); canLeave = true; Ev("KILLABLE"); }
            else if (near && timer <= 0) { if (chatCount == 8) insult = true; chatCount = next[chatCount]; timer = 3; Ev("EARLY"); }
        }
        if (T()) return;
        chatCount = 0;
        while (!canLeave && !arenaOver) {
            NSF(); if (T()) return;
            if (near && timer <= 0) { if (chatCount == 0) helmet = true; ++chatCount; timer = 3; Ev("LATE"); if (chatCount == 7) arenaOver = true; }
        }
        while (!T()) NSF();
    }
};
static void Make(Sim& s) { std::memset(&s, 0, sizeof(s)); s.termFrame = 200; s.attackAt = -1; s.questDoneAt = 40; s.near = true; }
int main()
{
    Sim a; Make(a); a.Main();
    bool ok = a.insult && a.helmet && a.arenaOver && a.chatCount == 7 && a.frame == 200;
    Sim b; Make(b); b.attackAt = 5; b.Main();
    ok = ok && b.canLeave && std::strstr(b.log, "HITMOVIE;KILLABLE;") && !b.helmet;
    Sim c; Make(c); c.attackAt = 5; c.inHit = true; c.Main();
    ok = ok && std::strstr(c.log, "HITCONVO;") != 0;
    std::printf(ok ? "00cf4ec0_TEST PASS\n" : "00cf4ec0_TEST FAIL\n");
    return ok ? 0 : 1;
}
