// Behaviour test for 0x0078AE30 CCreatureAction_HeroLoadRangedWeapon ctor: the base gets (creature, -1, 1, the random
// "PROJECTILE_WEAPON_LOAD" animation, an empty sound pair, 0, 0); PRangedWeapon holds the weapon and PAmmo is null.
#include <cstdio>
#include <cstring>
struct Anim { const char* Name; };
struct Creature { Anim Load; };
struct SoundPair { long First, Last; SoundPair() : First(0), Last(0) { } };
static const Anim& GetRandomAnimFromName(const Creature& c, const char* name)
{
    return std::strcmp(name, "PROJECTILE_WEAPON_LOAD") == 0 ? c.Load : *(const Anim*)0;
}
struct Base {
    Creature* Owner; float Speed, EndTime; const Anim* Animation; SoundPair Sound; long C, D;
    Base(Creature& o, float s, float e, const Anim& a, const SoundPair& sp, long c, long d)
        : Owner(&o), Speed(s), EndTime(e), Animation(&a), Sound(sp), C(c), D(d) { }
};
struct Thing { int Id; };
struct Action : Base {
    Thing* PRangedWeapon; void* PAmmo;
    Action(Creature& c, Thing& w) : Base(c, -1.0f, 1.0f, GetRandomAnimFromName(c, "PROJECTILE_WEAPON_LOAD"), SoundPair(), 0, 0),
        PRangedWeapon(&w), PAmmo(0) { }
};
int main()
{
    Creature hero; hero.Load.Name = "load";
    Thing bow; bow.Id = 7;
    Action a(hero, bow);
    bool ok = a.Owner == &hero && a.Speed == -1.0f && a.EndTime == 1.0f && a.Animation == &hero.Load && a.Sound.First == 0 &&
              a.Sound.Last == 0 && a.C == 0 && a.D == 0 && a.PRangedWeapon == &bow && a.PAmmo == 0;
    if (!ok) { std::printf("0078ae30_TEST FAIL\n"); return 1; }
    std::printf("0078ae30_TEST PASS\n");
    return 0;
}