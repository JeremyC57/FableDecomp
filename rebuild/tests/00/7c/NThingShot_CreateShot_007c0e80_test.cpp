// Behaviour test for 0x007C0E80 NThingShot::CreateShot(CThing&, ...): 0 when shots are off, with neither weapon nor shooter,
// or with a negative shot def index; the explicit index wins over the weapon's created-shot index; the shooter stands in
// for a missing weapon; the shot gets the def's TC, Init'ed with (def, shooter, weapon, target, floats, ability); the def
// lock count is balanced.
#include <cstdio>
#include <cstring>
struct ShotDef { const char* ClassName; int Locks; };
static ShotDef g_defs[3] = { { "", 0 }, { "SHOT_ARROW", 0 }, { "SHOT_FIREBALL", 0 } };
static bool g_shots_enabled = true;
struct ShotTC { const char* Name; ShotDef* Def; void* Shooter; void* Weapon; const void* Target; float A, B, C; int Ability; };
struct Thing { bool HasWeapon; long CreatedShot; ShotTC TC; };
static bool GetDef(long index, ShotDef** def) { if (index > 0 && index < 3) { *def = &g_defs[index]; ++(*def)->Locks; return true; } return false; }
static Thing* CreateShot(Thing& shot, Thing* shooter, Thing* weapon, const Thing* target, float a, float b, float c,
                         int ability, long shot_def_index)
{
    if (!g_shots_enabled) return 0;
    if (weapon == 0) { weapon = shooter; if (weapon == 0) return 0; }
    long def_index = shot_def_index > 0 ? shot_def_index : weapon->CreatedShot;
    if (def_index >= 0) {
        ShotDef* def = 0;
        GetDef(def_index, &def);
        ShotTC tc = { def->ClassName, def, shooter, weapon, target, a, b, c, ability };
        shot.TC = tc;
        --def->Locks;
        return &shot;
    }
    return 0;
}
int main()
{
    Thing hero = { false, 0 }, bow = { true, 1 }, wand = { true, 2 }, arrow = { false, 0 }, target = { false, 0 };
    bool ok = CreateShot(arrow, &hero, &bow, &target, 1, 2, 3, 7, 0) == &arrow && std::strcmp(arrow.TC.Name, "SHOT_ARROW") == 0 &&
              arrow.TC.Shooter == &hero && arrow.TC.Weapon == &bow && arrow.TC.Target == &target && arrow.TC.C == 3 &&
              arrow.TC.Ability == 7 && g_defs[1].Locks == 0;
    ok = ok && CreateShot(arrow, &hero, &bow, &target, 1, 2, 3, 7, 2) == &arrow && std::strcmp(arrow.TC.Name, "SHOT_FIREBALL") == 0;
    wand.CreatedShot = 2;
    ok = ok && CreateShot(arrow, &wand, 0, 0, 0, 0, 0, 0, 0) == &arrow && arrow.TC.Weapon == &wand && arrow.TC.Shooter == &wand;
    ok = ok && CreateShot(arrow, 0, 0, 0, 0, 0, 0, 0, 1) == 0;
    bow.CreatedShot = -1;
    ok = ok && CreateShot(arrow, &hero, &bow, 0, 0, 0, 0, 0, 0) == 0;
    g_shots_enabled = false;
    ok = ok && CreateShot(arrow, &hero, &wand, 0, 0, 0, 0, 0, 1) == 0;
    if (!ok) { std::printf("007c0e80_TEST FAIL\n"); return 1; }
    std::printf("007c0e80_TEST PASS\n");
    return 0;
}