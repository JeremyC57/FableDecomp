// Behaviour test for NScript::CPersonalScript_EmptyScript::CTestPerson::Main (0x00EF4B40): a copy of the Main logic over a mock game
// that logs calls; checks the scripted-control start and walk to TestPos, then per loop pass the talked-to wait, the paused daydream
// loop and the hero sent to HeroStandPos, the second talked-to wait, the clear and the "no camera tags" line only while alive.
#include <cstdio>
#include <cstring>

struct MockGame {
    char log[64][48]; int n, frames, terminateAt, startAt, talkFrames[8], nTalk; float health;
    void Log(const char* s) { if (n < 64) { std::strncpy(log[n], s, 47); log[n][47] = 0; ++n; } }
    void NewScriptFrame() { ++frames; }
    bool StartScriptingEntity() { return frames >= startAt; }
    bool TalkedTo() { for (int i = 0; i < nTalk; ++i) if (talkFrames[i] == frames) { talkFrames[i] = -1; return true; } return false; }
};
struct Person {
    MockGame* Game; int performing;
    bool T() const { return Game->terminateAt && Game->frames >= Game->terminateAt; }
    bool IsPerformingScriptTask() { return performing-- > 0; }
    void Main()
    {
        Game->Log("Predicate"); Game->NewScriptFrame(); if (T()) return;
        while (!Game->StartScriptingEntity()) { Game->NewScriptFrame(); if (T()) return; }
        if (T()) return;
        Game->Log("MoveToThing TestPos"); performing = 2;
        while (IsPerformingScriptTask()) { Game->NewScriptFrame(); if (T()) return; }
        if (T()) return;
        while (!T()) {
            while (!Game->TalkedTo()) { Game->NewScriptFrame(); if (T()) return; }
            if (T()) return;
            Game->Log("Movie"); Game->Log("MS18_SEATED_DAYDREAM_LOOP"); Game->Log("MoveToPosition HeroStandPos");
            while (!Game->TalkedTo()) { Game->NewScriptFrame(); if (T()) return; }
            if (T()) return;
            Game->Log("Movie"); Game->Log("ClearAllActions");
            if (Game->health > 0.0f) {
                Game->Log("This speech doesn't have any camera tags in it!"); performing = 1;
                while (IsPerformingScriptTask()) { Game->NewScriptFrame(); if (T()) return; }
                if (T()) return;
            }
            Game->NewScriptFrame();
        }
    }
};
static int Fail(int n) { std::printf("00ef4b40_TEST FAIL %d\n", n); return 1; }
int main()
{
    static const char* expect[] = { "Predicate", "MoveToThing TestPos", "Movie", "MS18_SEATED_DAYDREAM_LOOP", "MoveToPosition HeroStandPos",
        "Movie", "ClearAllActions", "This speech doesn't have any camera tags in it!" };
    MockGame g; std::memset(&g, 0, sizeof g); g.startAt = 3; g.health = 10.0f; g.nTalk = 2; g.talkFrames[0] = 7; g.talkFrames[1] = 9; g.terminateAt = 13;
    Person p; p.Game = &g; p.performing = 0; p.Main();
    if (g.n != 8) return Fail(1);
    for (int i = 0; i < g.n; ++i) if (std::strcmp(g.log[i], expect[i]) != 0) return Fail(2);
    MockGame d; std::memset(&d, 0, sizeof d); d.startAt = 1; d.health = 0.0f; d.nTalk = 2; d.talkFrames[0] = 4; d.talkFrames[1] = 5; d.terminateAt = 8;
    Person q; q.Game = &d; q.performing = 0; q.Main();
    if (d.n != 7 || std::strcmp(d.log[6], "ClearAllActions") != 0) return Fail(3);
    std::printf("00ef4b40_TEST PASS\n");
    return 0;
}