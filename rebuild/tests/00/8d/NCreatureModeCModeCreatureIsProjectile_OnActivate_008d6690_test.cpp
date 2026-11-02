// test: NCreatureMode::CModeCreatureIsProjectile::OnActivate -- record the start position, suspend the AI, disable the navigator, base OnActivate
#include <stdio.h>
#include <string.h>
static char trace[32];
struct Vec { float x, y, z; };
struct Physics { bool nav; void EnableNavigator(bool on) { nav = on; strcat(trace, on ? "N1" : "N0"); } };
struct Creature { Physics phys; bool suspended; Vec pos;
    void SuspendAI(bool s) { suspended = s; strcat(trace, "S"); }
    Physics* GetPhysicsTC() { return &phys; }
    const Vec& GetPos() const { strcat(trace, "G"); return pos; } };
struct StandBase { virtual ~StandBase() { } virtual void OnActivate() { strcat(trace, "B"); } };
struct Projectile : StandBase {
    Vec StartThrow; Creature* ParentThing; Creature* PCreature;
    void OnActivate() {
        StartThrow = ParentThing->GetPos();
        Creature* creature = PCreature;
        creature->SuspendAI(true);
        creature->GetPhysicsTC()->EnableNavigator(false);
        StandBase::OnActivate();
    }
};
int main() {
    Creature c; c.phys.nav = true; c.suspended = false; c.pos.x = 1; c.pos.y = 2; c.pos.z = 3;
    Projectile m; m.ParentThing = &c; m.PCreature = &c; m.StartThrow.x = m.StartThrow.y = m.StartThrow.z = 0;
    StandBase* b = &m; b->OnActivate();
    if (strcmp(trace, "GSN0B") != 0 || !c.suspended || c.phys.nav || m.StartThrow.x != 1 || m.StartThrow.y != 2 || m.StartThrow.z != 3) {
        printf("008d6690_TEST FAIL %s\n", trace); return 1; }
    printf("008d6690_TEST PASS\n");
    return 0;
}