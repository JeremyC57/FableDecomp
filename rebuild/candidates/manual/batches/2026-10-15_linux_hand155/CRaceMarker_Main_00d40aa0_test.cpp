// Behaviour test for 0x00D40AA0: CRaceMarker::Main loops frames until done; when the hero is near (and not done) it flags the platform and removes the marker.
#include <cstdio>
struct Thing { int id; };
struct Game { int frames, removed, nearAt; Thing hero; const Thing& GetHero() const { return hero; } void MiniMapRemoveMarker(const Thing&) { ++removed; } void NewScriptFrame() { ++frames; } };
struct Parent { bool ReachedPlatform; };
static int g_doneAfter, g_doneCalls;
struct Marker { Game* game; Thing Me; Parent* ParentClass; bool IsDone() { return ++g_doneCalls > g_doneAfter; }
  bool Near() { return game->frames >= game->nearAt; }
  void Main() { while (!IsDone()) { if (Near()) { if (IsDone()) break; ParentClass->ReachedPlatform = true; game->MiniMapRemoveMarker(Me); } game->NewScriptFrame(); } } };
int main()
{
    Game g = {0, 0, 2, {1}}; Parent p = {false}; Marker m = {&g, {2}, &p};
    g_doneAfter = 6; g_doneCalls = 0; m.Main();
    // calls: f0 (1) no near, f1 (2) no, f2 (3) near check done(4) ok -> removed, f3 (5) near done(6) ok -> removed, (7) done
    bool ok = p.ReachedPlatform && g.removed == 2 && g.frames == 4;
    Parent q = {false}; Marker n = {&g, {2}, &q}; g_doneAfter = 0; g_doneCalls = 0; g.removed = 0; n.Main();
    ok = ok && !q.ReachedPlatform && g.removed == 0;
    if (!ok) { std::printf("00d40aa0_TEST FAIL\n"); return 1; }
    std::printf("00d40aa0_TEST PASS\n");
    return 0;
}
