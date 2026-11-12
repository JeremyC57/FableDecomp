// Behaviour test for 0x007BDE31 NThingShot::CTCShotDefinitionFireballSpell::OnCollision: a flat copy of the row's logic over
// small models (burnt-effect reset vs add on type-flag bit 8, hit event only for live things with entity events, level-chosen explosion sound).
#include <cstdio>
#include <cstring>
enum EHeroAbility { HERO_ABILITY_FIREBALL_SPELL = 16 };   // Ego_r TPI value (engine/EHeroAbility.h)
struct Thing { unsigned long typeFlags; bool burnt, events, alive, sound; int resets, adds, hitEvents, ability; const char* played; };
struct Shot { Thing* parent; bool hasSubDef; long spellLevel; int baseCalls; };
static void OnCollision(Shot& s, Thing* thing)
{
    ++s.baseCalls;
    if (thing && (thing->typeFlags & 8)) {
        if (thing->burnt) ++thing->resets; else { ++thing->adds; thing->burnt = true; }
    }
    if (thing && thing->alive && thing->events) { ++thing->hitEvents; thing->ability = HERO_ABILITY_FIREBALL_SPELL; }
    long level = 0;
    if (s.hasSubDef) level = s.spellLevel;
    if (s.parent->sound) {
        switch (level) {
        case 0: s.parent->played = "HERO_ABILITY_FIREBALL_EXPLOSION_NO_POWERUP"; break;
        case 1: s.parent->played = "HERO_ABILITY_FIREBALL_EXPLOSION_LEVEL_1"; break;
        case 2: s.parent->played = "HERO_ABILITY_FIREBALL_EXPLOSION_LEVEL_2"; break;
        case 3: s.parent->played = "HERO_ABILITY_FIREBALL_EXPLOSION_LEVEL_3"; break;
        }
    }
}
int main()
{
    Thing parent = {0x47, false, false, true, true, 0, 0, 0, 0, 0};
    Thing target = {0x1F, false, true, true, false, 0, 0, 0, 0, 0};
    Shot shot = {&parent, true, 2, 0};
    OnCollision(shot, &target);
    if (target.adds != 1 || target.resets != 0 || target.hitEvents != 1 || target.ability != HERO_ABILITY_FIREBALL_SPELL ||
        std::strcmp(parent.played, "HERO_ABILITY_FIREBALL_EXPLOSION_LEVEL_2") != 0) { std::printf("007bde31_TEST FAIL\n"); return 1; }
    shot.hasSubDef = false; target.alive = false;
    OnCollision(shot, &target);
    if (target.resets != 1 || target.adds != 1 || target.hitEvents != 1 ||
        std::strcmp(parent.played, "HERO_ABILITY_FIREBALL_EXPLOSION_NO_POWERUP") != 0) { std::printf("007bde31_TEST FAIL\n"); return 1; }
    Thing building = {0x47, false, false, false, false, 0, 0, 0, 0, 0};
    OnCollision(shot, &building);
    if (building.adds != 0 || building.resets != 0) { std::printf("007bde31_TEST FAIL\n"); return 1; }
    parent.played = 0; shot.hasSubDef = true; shot.spellLevel = 7;
    OnCollision(shot, 0);
    if (parent.played != 0 || shot.baseCalls != 4) { std::printf("007bde31_TEST FAIL\n"); return 1; }
    std::printf("007bde31_TEST PASS\n");
    return 0;
}