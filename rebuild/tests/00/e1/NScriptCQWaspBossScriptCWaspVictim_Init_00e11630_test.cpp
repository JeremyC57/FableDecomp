// behaviour test for 0x00E11630 CWaspVictim::Init() (model: one interface call on Me with true)
#include <cstdio>
struct Thing { int id; };
static const Thing* g_thing = 0; static int g_value = -1;
struct Game { void Call(const Thing& t, int v) const { g_thing = &t; g_value = v; } };
struct Entity { Game* G; Thing Me; void Init() { G->Call(Me, 1); } };
int main()
{
    Game g; Entity e; e.G = &g; e.Me.id = 3;
    e.Init();
    int ok = g_thing == &e.Me && g_value == 1;
    if (!ok) { std::printf("00e11630_TEST FAIL\n"); return 1; }
    std::printf("00e11630_TEST PASS\n");
    return 0;
}