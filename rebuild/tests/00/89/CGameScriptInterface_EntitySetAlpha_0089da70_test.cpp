// Behaviour model of CGameScriptInterface::EntitySetAlpha (0x0089DA70): a live entity with a graphic appearance gets
// alpha clamped to [0, 1] and scaled to a byte (truncated); with includeCarried, every carried thing that is alive and has an
// appearance gets the same byte; dead / appearance-less carried things are skipped.
#include <cstdio>
#include <vector>
struct Appearance { int alpha; Appearance() : alpha(-1) {} void SetAlpha(unsigned char a) { alpha = a; } };
struct Thing {
    bool dead; Appearance* app; bool hasCarrying; std::vector<Thing*> carried;
    Thing() : dead(false), app(0), hasCarrying(false) {}
};
template <class T> inline bool IS_THING_ALIVE(const T& thing) { if (thing) { if (!thing->dead) return true; } return false; }
static void EntitySetAlpha(Thing* entity, bool entityAlive, float alpha, bool includeCarried)
{
    if (!entityAlive) return;
    Appearance* appearance = entity->app;
    if (!appearance) return;
    if (alpha < 0.0f) alpha = 0.0f; else if (alpha > 1.0f) alpha = 1.0f;
    appearance->SetAlpha((unsigned char)(alpha * 255.0f));
    if (includeCarried && entity->hasCarrying) {
        std::vector<Thing*> carried = entity->carried;
        for (unsigned long i = 0; i < carried.size(); ++i)
            if (IS_THING_ALIVE(carried[i])) {
                Appearance* a = carried[i]->app;
                if (a) a->SetAlpha((unsigned char)(alpha * 255.0f));
            }
    }
}
static int fail(int n) { std::printf("0089da70_TEST FAIL %d\n", n); return 1; }
int main()
{
    Appearance a0, a1, a2, a3;
    Thing e, c1, c2, c3; e.app = &a0; c1.app = &a1; c2.app = &a2; c2.dead = true; c3.app = 0;
    e.hasCarrying = true; e.carried.push_back(&c1); e.carried.push_back(&c2); e.carried.push_back(0); e.carried.push_back(&c3);
    EntitySetAlpha(&e, true, 2.0f, false);
    if (a0.alpha != 255 || a1.alpha != -1) return fail(1);
    EntitySetAlpha(&e, true, -1.0f, true);
    if (a0.alpha != 0 || a1.alpha != 0 || a2.alpha != -1) return fail(2);
    EntitySetAlpha(&e, true, 0.25f, true);
    if (a0.alpha != 63 || a1.alpha != 63 || a2.alpha != -1) return fail(3);
    a0.alpha = 7; EntitySetAlpha(&e, false, 1.0f, true);
    if (a0.alpha != 7) return fail(4);
    e.hasCarrying = false; EntitySetAlpha(&e, true, 1.0f, true);
    if (a0.alpha != 255 || a1.alpha != 63) return fail(5);
    e.app = 0; a1.alpha = 1; e.hasCarrying = true; EntitySetAlpha(&e, true, 1.0f, true);
    if (a1.alpha != 1) return fail(6);
    std::printf("0089da70_TEST PASS\n");
    return 0;
}