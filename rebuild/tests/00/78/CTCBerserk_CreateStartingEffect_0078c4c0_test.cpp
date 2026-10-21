// Behaviour test for CTCBerserk::CreateStartingEffect (0x0078C4C0): create the STARTUP_EFFECT and attach it when present.
#include <cstdio>
#include <cstring>
struct Thing { bool dead; Thing* attachedTo; int attachType; };
static Thing g_created = { false, 0, 0 };
static int g_createdId = -1;
static Thing* CreateParticleEffect(long id) { g_createdId = (int)id; return &g_created; }
struct Def { int refs; long startupId; bool GetEffect(const char* name, long* out) { if (std::strcmp(name, "STARTUP_EFFECT") || !startupId) return false; *out = startupId; return true; } };
struct Berserk {
    Thing* ParentThing; Def* subDef; Thing* StartUpEffect;
    void CreateStartingEffect()
    {
        Def* def = subDef; long effect = 0;
        if (def) ++def->refs;
        if (def) {
            if (def->GetEffect("STARTUP_EFFECT", &effect)) {
                StartUpEffect = CreateParticleEffect(effect);
                if (StartUpEffect && !StartUpEffect->dead) { StartUpEffect->attachedTo = ParentThing; StartUpEffect->attachType = 0x28; }
            }
        }
        if (def) --def->refs;
    }
};
int main()
{
    Thing hero = { false, 0, 0 };
    Def d = { 1, 77 };
    Berserk b = { &hero, &d, 0 };
    b.CreateStartingEffect();
    if (g_createdId != 77 || b.StartUpEffect != &g_created || g_created.attachedTo != &hero || g_created.attachType != 0x28 || d.refs != 1)
    { std::printf("0078c4c0_TEST FAIL\n"); return 1; }
    Def none = { 1, 0 }; Berserk c = { &hero, &none, 0 }; c.CreateStartingEffect();
    if (c.StartUpEffect != 0) { std::printf("0078c4c0_TEST FAIL\n"); return 1; }
    std::printf("0078c4c0_TEST PASS\n");
    return 0;
}