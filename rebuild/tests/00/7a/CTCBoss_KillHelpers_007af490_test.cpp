#include <cstdio>
#include <vector>
// Behaviour model of CTCBoss::KillHelpers (0x007AF490).
struct Creature { bool dead; bool explodes; float health; int killed; int knocked; float kx, ky, kz; };
struct Boss {
    std::vector<Creature*> HelperCreatures;
    void KillHelpers()
    {
        for (unsigned long i = 0; i < HelperCreatures.size(); ++i) {
            Creature* c = HelperCreatures[i];
            if (c && !c->dead) {
                if (c->explodes) { c->killed++; c->dead = true; }
                else {
                    float delta = -(c->health + 1.0f);
                    c->health += delta;
                    c->knocked++; c->kx = 0.0f; c->ky = 0.0f; c->kz = 1.0f;
                }
            }
            HelperCreatures[i] = 0;
        }
    }
};
int main()
{
    Creature a = { false, true, 50.0f, 0, 0, 0, 0, 0 };
    Creature b = { false, false, 30.0f, 0, 0, 0, 0, 0 };
    Creature d = { true, false, 0.0f, 0, 0, 0, 0, 0 };
    Boss boss;
    boss.HelperCreatures.push_back(&a);
    boss.HelperCreatures.push_back(0);
    boss.HelperCreatures.push_back(&b);
    boss.HelperCreatures.push_back(&d);
    boss.KillHelpers();
    bool ok = a.killed == 1 && a.knocked == 0 && b.killed == 0 && b.knocked == 1 && b.health < 0.0f && b.health > -1.5f
           && b.kz == 1.0f && d.knocked == 0 && d.killed == 0 && boss.HelperCreatures.size() == 4;
    for (unsigned long i = 0; i < boss.HelperCreatures.size(); ++i) if (boss.HelperCreatures[i]) ok = false;
    if (!ok) { std::printf("007af490_TEST FAIL\n"); return 1; }
    std::printf("007af490_TEST PASS\n");
    return 0;
}