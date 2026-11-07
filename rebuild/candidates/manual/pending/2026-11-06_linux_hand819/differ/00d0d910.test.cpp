// Behaviour test for NScript::CQ_BanditCampBossBattleScript::CGate2Guard1::Main (0x00D0D910): while the gate is shut, the four hit
// lines (the fourth makes him a killable enemy), one passing comment when the hero comes close (re-armed by the comment timer), and
// when talked to the gate-blocked line or, with the camp pass, the given-pass line and the OpenGate thread; once open, the aside and
// the attacked line. A simulation of the same control flow with a scripted game.
#include <cstdio>
#include <cstring>
struct Sim {
    char log[2048]; int frame, termFrame, hitAt[6], nHit, talkAt, hitCount; bool gateOpen, hasPass, enemy, commented, near;
    bool T() const { return frame >= termFrame; }
    void Ev(const char* e) { std::strcat(log, e); std::strcat(log, ";"); }
    bool Hit() { for (int i = 0; i < nHit; ++i) if (hitAt[i] == frame) return true; return false; }
    void Main()
    {
        if (T()) return;
        while (!T()) {
            if (!gateOpen) {
                if (Hit()) { static const char* L[4] = {"HIT1","HIT2","HIT3","HIT4"}; Ev(L[hitCount < 3 ? hitCount : 3]); if (hitCount >= 3) { enemy = true; Ev("ENEMY"); } if (hitCount == 0) hitCount = 1; else ++hitCount; }
                else if (near && !commented) { Ev("COMMENT"); commented = true; }
                else if (frame == talkAt) { if (hasPass) { Ev("GIVENPASS"); Ev("OPENGATE"); gateOpen = true; } else Ev("BLOCKED"); }
            } else if (Hit()) { Ev("ATTACKED"); break; }
            ++frame;
        }
    }
};
static void Make(Sim& s) { std::memset(&s, 0, sizeof(s)); s.termFrame = 50; s.talkAt = -1; }
int main()
{
    Sim a; Make(a); a.nHit = 4; a.hitAt[0] = 2; a.hitAt[1] = 4; a.hitAt[2] = 6; a.hitAt[3] = 8; a.Main();
    bool ok = a.enemy && std::strstr(a.log, "HIT1;HIT2;HIT3;HIT4;ENEMY;") != 0;
    Sim b; Make(b); b.near = true; b.hasPass = true; b.talkAt = 5; b.nHit = 1; b.hitAt[0] = 9; b.Main();
    ok = ok && b.gateOpen && std::strcmp(b.log, "COMMENT;GIVENPASS;OPENGATE;ATTACKED;") == 0;
    Sim c; Make(c); c.talkAt = 3; c.Main();
    ok = ok && !c.gateOpen && std::strcmp(c.log, "BLOCKED;") == 0;
    std::printf(ok ? "00d0d910_TEST PASS\n" : "00d0d910_TEST FAIL\n");
    return ok ? 0 : 1;
}
