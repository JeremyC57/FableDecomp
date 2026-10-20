// Behaviour test for CTCCoopSpirit::SetAppearanceParticle (0x006705F0).
#include <cstdio>

struct Thing { bool dead; int killed; bool hasEmitter; int attached; long bone; float scale; };
static Thing g_created; static bool g_createFails = false;
static Thing* Create() { if (g_createFails) return 0; g_created.dead = false; return &g_created; }
struct Spirit {
    Thing* particle;
    void SetAppearanceParticle()
    {
        if (particle && !particle->dead) ++particle->killed;
        particle = Create();
        if (particle && !particle->dead) {
            if (particle->hasEmitter) { ++particle->attached; particle->bone = 0x88; particle->scale = 1.1f; }
        }
    }
};

int main()
{
    Thing old = { false, 0, false, 0, 0, 0 };
    g_created.hasEmitter = true;
    Spirit s = { &old };
    s.SetAppearanceParticle();
    if (old.killed != 1 || s.particle != &g_created || g_created.attached != 1 || g_created.bone != 0x88) { std::printf("006705f0_TEST FAIL\n"); return 1; }
    g_createFails = true; s.SetAppearanceParticle();
    if (g_created.killed != 1 || s.particle != 0) { std::printf("006705f0_TEST FAIL\n"); return 1; }
    std::printf("006705f0_TEST PASS\n");
    return 0;
}