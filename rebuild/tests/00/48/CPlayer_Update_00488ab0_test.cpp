// Behaviour test for 0x00488ab0: CPlayer::Update: initial mode once, respawn countdown, world map position.
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct Loader { bool loading; };
struct Map { int calls; long player; const float* pos; long type; void SetPlayerPos(long p, const float* v, long t) { ++calls; player = p; pos = v; type = t; } };
struct Thing { float pos[3]; };
struct Player { bool initSet; Loader* loader; int setups; long RespawnDelay; Thing* creature; Map* map; long Number;
  void Update() { if (!initSet) { if (!loader->loading) { initSet = true; ++setups; } } if (RespawnDelay > 0) --RespawnDelay; if (creature) map->SetPlayerPos(Number, creature->pos, 0); } };
static int run() { Loader l = {true}; Map m = {0,0,0,0}; Thing t = {{1,2,3}}; Player p = {false, &l, 0, 2, &t, &m, 4};
  p.Update(); CHECK(!p.initSet && p.RespawnDelay == 1 && m.calls == 1 && m.player == 4 && m.pos == t.pos);
  l.loading = false; p.Update(); p.Update(); CHECK(p.initSet && p.setups == 1 && p.RespawnDelay == 0);
  p.creature = 0; p.Update(); CHECK(m.calls == 3); return 0; }
int main()
{
    run();
    if (g_fail) { std::printf("00488ab0_TEST FAIL\n"); return 1; }
    std::printf("00488ab0_TEST PASS\n");
    return 0;
}