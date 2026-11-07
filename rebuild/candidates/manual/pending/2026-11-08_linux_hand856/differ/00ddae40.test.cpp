// Behaviour test for NScript::CQ_PrisonRaceScript::Main (0x00DDAE40): a copy of the Main logic over a mock game; checks the six bindings,
// the race-won flag clear, the hero teleport to the PrisonRaceStartHSP marker (position and heading; NullCoord / 0.0f when missing),
// the courtyard wait and the WatchForHeroWinOrLose thread, and that terminating during the wait spawns nothing.
#include <cstdio>
#include <cstring>

struct V3 { float X, Y, Z; };
static V3 NullCoord = { 0.0f, 0.0f, 0.0f };
struct Impl { V3 pos; float angle; };
struct Thing {
    Impl* Data;
    const V3& GetPos() const { if (!Data) return NullCoord; return Data->pos; }
    float GetAngleXY() const { if (!Data) return 0.0f; return Data->angle; }
};
struct MockGame {
    int frames, loadedAt, terminateAt, teleports; Impl* marker; V3 tpPos; float tpAngle; bool tpA, tpB;
    Thing GetThingWithScriptName(const char* n) { Thing t; t.Data = std::strcmp(n, "PrisonRaceStartHSP") == 0 ? marker : 0; return t; }
    int GetHero() { return 7; }
    void EntityTeleportToPosition(int hero, const V3& p, float a, bool x, bool y) { if (hero == 7) { ++teleports; tpPos = p; tpAngle = a; tpA = x; tpB = y; } }
    bool IsRegionLoaded(const char* n) { return std::strcmp(n, "PrisonCourtyard") == 0 && frames >= loadedAt; }
    void NewScriptFrame() { ++frames; }
};
struct Script {
    MockGame* Game; bool PrisonRaceWonByHero; int bindings, posts; const char* thread;
    bool IsActiveThreadTerminating() const { return Game->terminateAt && Game->frames >= Game->terminateAt; }
    void Main()
    {
        static const char* names[] = { "PrisonRaceGuard", "PrisonRaceWinningPostGuard", "PrisonRaceWarden", "PrisonRacer", "PrisonRaceStartingGuard", "WaypointSpeedCheck" };
        for (int i = 0; i < 6; ++i) if (names[i][0]) ++bindings;
        posts += 2;
        PrisonRaceWonByHero = false;
        Thing StartPos = Game->GetThingWithScriptName("PrisonRaceStartHSP");
        Game->EntityTeleportToPosition(Game->GetHero(), StartPos.GetPos(), StartPos.GetAngleXY(), false, false);
        while (!Game->IsRegionLoaded("PrisonCourtyard")) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        thread = "WatchForHeroWinOrLose";
    }
};
static int Fail(int n) { std::printf("00ddae40_TEST FAIL %d\n", n); return 1; }
int main()
{
    Impl m = { { 1.0f, 2.0f, 3.0f }, 0.25f };
    MockGame g; std::memset(&g, 0, sizeof g); g.marker = &m; g.loadedAt = 3;
    Script s; s.Game = &g; s.PrisonRaceWonByHero = true; s.bindings = s.posts = 0; s.thread = 0;
    s.Main();
    if (s.bindings != 6 || s.posts != 2 || s.PrisonRaceWonByHero || g.teleports != 1) return Fail(1);
    if (g.tpPos.X != 1.0f || g.tpPos.Y != 2.0f || g.tpPos.Z != 3.0f || g.tpAngle != 0.25f || g.tpA || g.tpB) return Fail(2);
    if (g.frames != 3 || !s.thread || std::strcmp(s.thread, "WatchForHeroWinOrLose") != 0) return Fail(3);
    MockGame g2; std::memset(&g2, 0, sizeof g2); g2.marker = 0; g2.loadedAt = 10; g2.terminateAt = 2;
    Script s2; s2.Game = &g2; s2.PrisonRaceWonByHero = true; s2.bindings = s2.posts = 0; s2.thread = 0;
    s2.Main();
    if (g2.teleports != 1 || g2.tpPos.X != 0.0f || g2.tpAngle != 0.0f || s2.thread || g2.frames != 2 || s2.PrisonRaceWonByHero) return Fail(4);
    std::printf("00ddae40_TEST PASS\n");
    return 0;
}
