// Behaviour test for CTCPhysicalShieldSpell::~CTCPhysicalShieldSpell (0x0079E990): live effects are faded, dead or
// missing ones are left alone, the def reference is released.
#include <cstdio>

struct Emitter { int faded; void FadeOrKill() { ++faded; } };
struct Thing { bool dead; bool hasEmitter; Emitter emitter; };
struct Def { long refs; int destroyed; };
struct Shield {
    Thing* shieldEffect; Thing* swirlyEffect; Def* def;
    static void FadeOrKillEffect(Thing* thing)
    {
        if (thing && !thing->dead) {
            Emitter* e = 0;
            if (thing->hasEmitter) e = &thing->emitter;
            if (e) e->FadeOrKill();
        }
    }
    ~Shield()
    {
        FadeOrKillEffect(shieldEffect);
        FadeOrKillEffect(swirlyEffect);
        if (def) { if (--def->refs == 0) def->destroyed = 1; def = 0; }
    }
};

int main()
{
    Thing a = { false, true, { 0 } }, b = { true, true, { 0 } };
    Def d = { 1, 0 };
    { Shield s = { &a, &b, &d }; }
    if (a.emitter.faded != 1 || b.emitter.faded != 0 || d.refs != 0 || !d.destroyed) { std::printf("0079e990_TEST FAIL\n"); return 1; }
    { Shield s = { 0, &a, 0 }; }
    if (a.emitter.faded != 2) { std::printf("0079e990_TEST FAIL\n"); return 1; }
    std::printf("0079e990_TEST PASS\n");
    return 0;
}