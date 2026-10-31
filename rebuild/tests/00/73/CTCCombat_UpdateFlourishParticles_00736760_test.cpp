// behaviour test for 0x00736760 CTCCombat::UpdateFlourishParticles()
#include <cstdio>
struct Thing { bool dead; int killed; bool hasCarry; Thing* primary; bool hasWeapon; bool melee; };
struct Combat { bool valid; long timer; Thing* parts[3]; int n; Thing* owner; Thing* started; };
static bool Alive(Thing* t) { return t && !t->dead; }
static void Update(Combat& c) {
    if (c.valid) {
        for (int i = 0; i < c.n; ++i) if (Alive(c.parts[i])) return;
        if (c.timer > 0) { --c.timer; return; }
        if (!c.owner->hasCarry) return;
        Thing* w = c.owner->primary;
        if (!w || w->dead) return;
        if (!w->hasWeapon) return;
        if (!w->melee) return;
        c.started = w;
        return;
    }
    c.timer = 0;
    for (int i = 0; i < c.n; ++i) if (Alive(c.parts[i])) c.parts[i]->killed++;
}
int main() {
    Thing p1 = {false, 0}, p2 = {true, 0}, sword = {false, 0, false, 0, true, true};
    Thing owner = {false, 0, true, &sword};
    Combat c = {true, 0, {&p2, 0, 0}, 1, &owner, 0};
    Update(c);
    if (c.started != &sword) { std::printf("00736760_TEST FAIL\n"); return 1; }
    c.started = 0; c.parts[0] = &p1; Update(c);
    if (c.started != 0) { std::printf("00736760_TEST FAIL\n"); return 1; }
    c.valid = false; c.timer = 5; Update(c);
    if (c.timer != 0 || p1.killed != 1 || p2.killed != 0) { std::printf("00736760_TEST FAIL\n"); return 1; }
    std::printf("00736760_TEST PASS\n");
    return 0;
}