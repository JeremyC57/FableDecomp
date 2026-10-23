#include <cstdio>
#include <cstring>
// behaviour test: CTCTurncoatSpell::StartTurncoatingEffects -- target gate, looping sound, effect-table lookups, attach effects
struct Thing { bool Dead; bool HasSound; long SoundsStarted; long AttachedTo; };
struct EffectTable { long HeroEffect; long EnemyEffect; };
static Thing g_effects[4];
static int g_effectCount = 0;
static Thing* CreateEffect(long effect)
{
    if (effect == 0 || g_effectCount >= 4) return 0;
    Thing* t = &g_effects[g_effectCount++];
    t->Dead = false; t->AttachedTo = 0;
    return t;
}
static bool Lookup(const EffectTable* table, const char* name, long* out)
{
    long v = 0;
    if (std::strcmp(name, "HERO_TURNCOATING_EFFECT") == 0) v = table->HeroEffect;
    else if (std::strcmp(name, "ENEMY_TURNCOATING_EFFECT") == 0) v = table->EnemyEffect;
    if (v == 0) return false;
    *out = v;
    return true;
}
struct TurncoatSpell {
    Thing* Parent; Thing* Target; EffectTable* Table; unsigned long LoopingSound;
    Thing* EffectOnMe; Thing* EffectOnTarget;
    void Start()
    {
        if (!Target || Target->Dead) return;
        if (Parent->HasSound) { ++Parent->SoundsStarted; LoopingSound = 77; }
        long effect = 0;
        if (Table) {
            if (Lookup(Table, "HERO_TURNCOATING_EFFECT", &effect)) {
                EffectOnMe = CreateEffect(effect);
                if (EffectOnMe && !EffectOnMe->Dead) EffectOnMe->AttachedTo = 1;
            }
            if (Lookup(Table, "ENEMY_TURNCOATING_EFFECT", &effect)) {
                EffectOnTarget = CreateEffect(effect);
                if (EffectOnTarget && !EffectOnTarget->Dead) EffectOnTarget->AttachedTo = 2;
            }
        }
    }
};
int main()
{
    Thing caster; std::memset(&caster, 0, sizeof(caster)); caster.HasSound = true;
    Thing target; std::memset(&target, 0, sizeof(target));
    EffectTable table; table.HeroEffect = 5; table.EnemyEffect = 6;
    TurncoatSpell s; std::memset(&s, 0, sizeof(s));
    s.Parent = &caster; s.Target = &target; s.Table = &table;
    s.Start();
    bool ok = caster.SoundsStarted == 1 && s.LoopingSound == 77 && s.EffectOnMe && s.EffectOnMe->AttachedTo == 1;
    ok = ok && s.EffectOnTarget && s.EffectOnTarget->AttachedTo == 2 && g_effectCount == 2;
    target.Dead = true;
    s.Start();
    ok = ok && caster.SoundsStarted == 1 && g_effectCount == 2;
    target.Dead = false; table.EnemyEffect = 0; s.EffectOnTarget = 0; caster.HasSound = false;
    s.Start();
    ok = ok && caster.SoundsStarted == 1 && g_effectCount == 3 && s.EffectOnTarget == 0;
    std::printf(ok ? "007c6aa0_TEST PASS\n" : "007c6aa0_TEST FAIL\n");
    return ok ? 0 : 1;
}