// Behaviour test for CTCPhysicsControlled::UpdateAdditionalImpulse (0x00725B70), g++ model of the retail logic.
#include <cstdio>
#include <vector>
struct Thing { int id; bool creature; };
struct World { bool nonScriptedPause, allPause; std::vector<Thing*> things; };
struct Physics { Thing* parent; World* world; std::vector<int> combatHits, playerHits; };
static void UpdateAdditionalImpulse(Physics& p)
{
    if (p.world->nonScriptedPause) return;
    if (p.world->allPause) return;
    std::vector<Thing*> creatures;
    for (size_t i = 0; i < p.world->things.size(); ++i)
        if (p.world->things[i]->creature && p.world->things[i] != p.parent) creatures.push_back(p.world->things[i]);
    for (size_t i = 0; i < creatures.size(); ++i) p.combatHits.push_back(creatures[i]->id);
    for (size_t i = 0; i < creatures.size(); ++i) p.playerHits.push_back(creatures[i]->id);
}
int main()
{
    Thing a = { 1, true }, b = { 2, true }, c = { 3, false };
    World w = { false, false };
    w.things.push_back(&a); w.things.push_back(&b); w.things.push_back(&c);
    Physics p; p.parent = &a; p.world = &w;
    UpdateAdditionalImpulse(p);
    int ok = p.combatHits.size() == 1 && p.combatHits[0] == 2 && p.playerHits.size() == 1 && p.playerHits[0] == 2;
    w.allPause = true; Physics q; q.parent = &c; q.world = &w;
    UpdateAdditionalImpulse(q);
    ok &= q.combatHits.empty() && q.playerHits.empty();
    w.allPause = false; w.nonScriptedPause = true;
    UpdateAdditionalImpulse(q);
    ok &= q.combatHits.empty();
    std::printf(ok ? "00725b70_TEST PASS\n" : "00725b70_TEST FAIL\n");
    return ok ? 0 : 1;
}