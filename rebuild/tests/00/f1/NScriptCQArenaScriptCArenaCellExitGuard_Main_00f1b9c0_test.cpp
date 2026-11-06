// Behaviour test for CQ_ArenaScript::CArenaCellExitGuard::Main (0x00F1B9C0): talked to, the guard asks whether the hero stays;
// "no" fails the arena quest (MissionFailed); hit, he heals and turns ally; otherwise he walks back to CellExitMarker and turns once.
#include <cstdio>

struct Guard {
    bool PreArena, MissionFailed, QuestFailed, Turned, Ally; long Lines; float Health;
    void Talk(bool stays)
    {
        Lines += PreArena ? 2 : 1;
        Lines += 1;
        if (stays) { Lines += 1; return; }
        Lines += 1;
        QuestFailed = true; MissionFailed = true;
    }
    void Hit() { Lines += 1; Health += 10000.0f; Ally = true; }
    void Idle(bool farFromMarker) { if (farFromMarker) Turned = false; else if (!Turned) Turned = true; }
};

int main()
{
    Guard g = { true, false, false, false, false, 0, 1.0f };
    g.Talk(true);
    bool ok = !g.MissionFailed && g.Lines == 4;
    g.PreArena = false; g.Talk(false);
    ok = ok && g.MissionFailed && g.QuestFailed && g.Lines == 7;
    g.Hit();
    ok = ok && g.Ally && g.Health > 10000.0f;
    g.Idle(false); ok = ok && g.Turned;
    g.Idle(true); ok = ok && !g.Turned;
    if (!ok) { std::printf("00f1b9c0_TEST FAIL\n"); return 1; }
    std::printf("00f1b9c0_TEST PASS\n");
    return 0;
}