// Behaviour test for 0x00961640 CAIStateGroup_BanditKingThrowWeapons::Update: a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
struct G { int busy; long result; int terminated; int actions; float health, maxHealth; long phase; long percent[4]; bool boss; };
static void Update(G& g)
{
    while (g.busy > 0) { g.busy--; if (g.result != 4 && g.result != 2) { g.terminated++; return; } }
    g.actions += 2;
    if (g.boss) { long p = g.phase; g.health = (float)g.percent[p] * g.maxHealth * 0.01f; g.phase = p + 1; }
    g.terminated++;
}
int main() {
    G g = { 2, 4, 0, 0, 0.0f, 200.0f, 1, { 100, 50, 25, 0 }, true }; Update(g);
    int ok = g.actions == 2 && g.health == 100.0f && g.phase == 2 && g.terminated == 1;
    G h = { 1, 3, 0, 0, 0.0f, 1.0f, 0, { 0 }, true }; Update(h); ok &= h.actions == 0 && h.terminated == 1;
    std::printf(ok ? "00961640_TEST PASS\n" : "00961640_TEST FAIL\n");
    return ok ? 0 : 1;
}
