#include <cstdio>
#include <cstring>
// behaviour test: CTCWillLightning::CTCWillLightning -- defaults, ability marker, init sound, effect lookups, level and starting state
struct Thing {
    bool HasAbilityUser, HasSound, HasAbilities, HasStats;
    unsigned char ThingType;
    bool UserActive; long UserAbility;
    long SoundsPlayed;
    long AbilityLevel;
    long WillPower;
};
struct SpellDef { float CombatMultiplierTime; bool HasEffects; };
static const long kInvalidSound = 0x7fffffffL;
static long g_LargeEffect = -1, g_BeamEffect = -1;
static long g_FrameRate = 30;
static long LookupEffect(const char* name)
{
    if (std::strcmp(name, "LIGHTNING_BEAM_LARGE_EFFECT") == 0) return 11;
    if (std::strcmp(name, "LIGHTNING_BEAM_EFFECT") == 0) return 12;
    if (std::strcmp(name, "LIGHTNING_HAND_EFFECT") == 0) return 13;
    if (std::strcmp(name, "LIGHTNING_HAND_EFFECT_NO_LIGHT") == 0) return 14;
    return 0;
}
struct WillLightning {
    Thing* Parent; long WeaponState; bool LeftHandAnim; long PrimaryHandEffect, SecondaryHandEffect;
    SpellDef* Def; bool TimerIsPlayer; Thing* Caster; long State; float Time, TimeBase;
    unsigned long InitSound, LoopingSound; bool StartCostingStamina, BeamsInitialised, PlayedSound;
    long OnhitEveryFrame, CombatMultiplierTimer, Level;
    void SetLevel(long level) { Level = level; }
    void Construct(Thing* parent, SpellDef* def)
    {
        Parent = parent; WeaponState = 0; LeftHandAnim = true; PrimaryHandEffect = 0; SecondaryHandEffect = 0;
        StartCostingStamina = false; LoopingSound = kInvalidSound; OnhitEveryFrame = 0; CombatMultiplierTimer = 0;
        if (Parent->HasAbilityUser) { Parent->UserActive = true; Parent->UserAbility = 11; }
        InitSound = kInvalidSound;
        Def = def;
        if (Parent->HasSound) ++Parent->SoundsPlayed;
        if (Def->HasEffects) {
            g_LargeEffect = LookupEffect("LIGHTNING_BEAM_LARGE_EFFECT");
            g_BeamEffect = LookupEffect("LIGHTNING_BEAM_EFFECT");
            PrimaryHandEffect = LookupEffect("LIGHTNING_HAND_EFFECT");
            SecondaryHandEffect = LookupEffect("LIGHTNING_HAND_EFFECT_NO_LIGHT");
        }
        Caster = Parent;
        long level = 0;
        if (Caster->HasAbilities) level = Caster->AbilityLevel;
        SetLevel(level);
        TimerIsPlayer = false;
        if (Parent->ThingType == 1) TimerIsPlayer = true;
        if (Caster->HasStats && Caster->WillPower <= 0) {
            State = 6;
        } else {
            State = 2; Time = 0.0f; TimeBase = 0.0f; PlayedSound = false;
        }
        BeamsInitialised = false;
        CombatMultiplierTimer = (long)(g_FrameRate * Def->CombatMultiplierTime);
    }
};
int main()
{
    Thing hero; std::memset(&hero, 0, sizeof(hero));
    hero.HasAbilityUser = true; hero.HasSound = true; hero.HasAbilities = true; hero.HasStats = true;
    hero.ThingType = 1; hero.AbilityLevel = 3; hero.WillPower = 50;
    SpellDef def; def.CombatMultiplierTime = 2.5f; def.HasEffects = true;
    WillLightning w; std::memset(&w, 0, sizeof(w));
    w.Construct(&hero, &def);
    bool ok = w.LeftHandAnim && w.WeaponState == 0 && hero.UserActive && hero.UserAbility == 11 && hero.SoundsPlayed == 1;
    ok = ok && w.PrimaryHandEffect == 13 && w.SecondaryHandEffect == 14 && g_LargeEffect == 11 && g_BeamEffect == 12;
    ok = ok && w.Level == 3 && w.TimerIsPlayer && w.State == 2 && w.CombatMultiplierTimer == 75 && !w.BeamsInitialised;
    ok = ok && w.InitSound == (unsigned long)kInvalidSound && w.LoopingSound == (unsigned long)kInvalidSound;
    Thing npc; std::memset(&npc, 0, sizeof(npc));
    npc.HasStats = true; npc.WillPower = 0; npc.ThingType = 2;
    def.HasEffects = false;
    WillLightning n; std::memset(&n, 0, sizeof(n));
    n.Construct(&npc, &def);
    ok = ok && n.State == 6 && n.Level == 0 && !n.TimerIsPlayer && n.PrimaryHandEffect == 0 && npc.SoundsPlayed == 0;
    std::printf(ok ? "007a09b0_TEST PASS\n" : "007a09b0_TEST FAIL\n");
    return ok ? 0 : 1;
}
