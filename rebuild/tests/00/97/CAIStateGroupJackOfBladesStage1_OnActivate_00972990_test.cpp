#include <cstdio>
#include <vector>
// Behaviour model of CAIStateGroup_JackOfBladesStage1::OnActivate (0x00972990).
struct Vec { float X, Y, Z; };
struct Thing { bool dead; bool killed; Vec pos; unsigned long def; };
struct HitLoc { bool enabled; };
struct Boss { Vec phaseStart; };
struct World { std::vector<Thing*> things; };
static bool InBox(const Thing& t, long cx, long cy, long r)
{
    long x = (long)t.pos.X, y = (long)t.pos.Y;
    return x >= cx - r && x < cx + r && y >= cy - r && y < cy + r;
}
struct Jack {
    Thing* creature; Thing* target; Thing* arenaCentre; Thing* shield; long speechCounter;
    HitLoc big, forcefield, plain; Boss boss; bool hasBoss; World* world; int registered;
    void OnActivate()
    {
        registered++;
        big.enabled = false; forcefield.enabled = false; plain.enabled = true;
        long cx = (long)creature->pos.X, cy = (long)creature->pos.Y;
        std::vector<Thing*> lights;
        for (unsigned i = 0; i < world->things.size(); ++i)
            if (world->things[i]->def == 1 && InBox(*world->things[i], cx, cy, 50)) lights.push_back(world->things[i]);
        unsigned long count = lights.size();
        if (count > 0)
            for (unsigned long i = 0; i < count; ++i)
                if (lights[i] && !lights[i]->dead) lights[i]->killed = true;
        Thing* found = 0;
        for (unsigned i = 0; i < world->things.size(); ++i)
            if (world->things[i]->def == 2 && InBox(*world->things[i], cx, cy, 50)) { found = world->things[i]; break; }
        if (found) shield = found;
        if (arenaCentre && !arenaCentre->dead && hasBoss) boss.phaseStart = arenaCentre->pos;
        speechCounter = 0;
    }
};
int main()
{
    Thing me = {false,false,{100.f,100.f,0.f},0}, tgt = {false,false,{0,0,0},0};
    Thing l1 = {false,false,{120.f,90.f,0},1}, l2 = {true,false,{110.f,110.f,0},1}, l3 = {false,false,{300.f,300.f,0},1};
    Thing sh = {false,false,{130.f,130.f,0},2}, arena = {false,false,{5.f,6.f,7.f},0};
    World w; w.things.push_back(&l1); w.things.push_back(&l2); w.things.push_back(&l3); w.things.push_back(&sh);
    Jack j; j.creature=&me; j.target=&tgt; j.arenaCentre=&arena; j.shield=0; j.speechCounter=9;
    j.big.enabled=j.forcefield.enabled=true; j.plain.enabled=false; j.boss.phaseStart.X=0; j.hasBoss=true; j.world=&w; j.registered=0;
    j.OnActivate();
    if (!l1.killed || l2.killed || l3.killed || j.shield != &sh || j.speechCounter != 0 || j.big.enabled || j.forcefield.enabled ||
        !j.plain.enabled || j.boss.phaseStart.Z != 7.f || j.registered != 1) { std::printf("00972990_TEST FAIL\n"); return 1; }
    std::printf("00972990_TEST PASS\n");
    return 0;
}