// Behaviour test for 0x00839940 CThingGameObject::OnHit: a model of the row's decisions over a fake thing / hit.
// Checked: the hit-location fallback (default location of the hit-locations TC, early exit without one), the armour damage
// material override for creatures (only a positive def index), the electrocute-on-touch exit (melee, creature attacker: hit
// negated, nothing after), the door / window break-down negation, the battle-charge / assassin-rush exit (only the hit response
// is cleared), the block branch (block-parry count), negate-all hits (block material), absorb-hits (health untouched while hits
// remain), the kill count on the weapon, the hero-stat events (one per stat increase of each damage type, none for a summoned
// creature or the ghost sword) and the collision-sound material (weapon material, else the hitter's, replaced by the lightning /
// divine-wrath / unholy-power material, else the hitter's armour damage material).
#include <cstdio>
#include <string>
#include <cstring>
#include <vector>

struct Hit {
    bool HasLocation, Melee, Blocked, Negated, PlayResponse, Localised, Lightning, Divine, Unholy;
    long DamageTypes, StatsPerType;
    float Damage;
};
struct State {
    bool HasHitLocationsTC, DefaultLocation, Creature, HitterCreature, HitterAlive, WeaponAlive, Electrocute, Door, HeroBreaksDoors;
    bool BattleCharge, AssassinRush, NegateAll, Summoned, GhostSword, WeaponHasMaterial, HitterHasMaterial, HasSound, Giving;
    long ArmourDamageMaterial, ArmourBlockMaterial, AbsorbHitsLeft, Kills, BlockParries, StatEvents;
    float Health;
    long Material, SoundMaterial;
};
struct World : State {
    std::string Trace;
    World() { std::memset(static_cast<State*>(this), 0, sizeof(State)); }
};

enum { MAT_OWN = 1, MAT_WEAPON = 2, MAT_HITTER = 3, MAT_LIGHTNING = 4, MAT_DIVINE = 5, MAT_UNHOLY = 6 };

static void OnHit(World& w, Hit& h)
{
    if (!h.HasLocation) {
        if (!w.HasHitLocationsTC || !w.DefaultLocation) { w.Trace += 'x'; return; }
    }
    w.Material = MAT_OWN;
    if (w.Creature && w.ArmourDamageMaterial > 0) w.Material = w.ArmourDamageMaterial;
    if (h.Melee && w.HitterAlive) {
        if (w.HitterCreature && w.Electrocute) { h.Negated = true; w.Trace += 'e'; return; }
        if (w.Door && !w.HeroBreaksDoors) h.Negated = true;
    }
    if (w.BattleCharge || w.AssassinRush) { h.PlayResponse = false; w.Trace += 'c'; return; }
    if (h.Blocked) { ++w.BlockParries; w.Trace += 'b'; }
    else {
        if (w.NegateAll) {
            h.Negated = true;
            if (w.Creature && w.ArmourBlockMaterial > 0) w.Material = w.ArmourBlockMaterial;
        }
        if (h.Negated) h.PlayResponse = false;
        if (!h.Negated) {
            bool already_dead = w.Health < 0.0001f;
            if (w.AbsorbHitsLeft-- <= 0) w.Health -= h.Damage;
            if (!already_dead && w.Health < 0.0001f && w.WeaponAlive) { ++w.Kills; w.Trace += 'k'; }
        }
    }
    if (!h.Negated && !h.Blocked && w.HitterAlive && w.Giving && !w.Summoned && !w.GhostSword)
        w.StatEvents += h.DamageTypes * h.StatsPerType;
    if (w.HasSound && h.Localised) {
        long m = 0;
        if (w.WeaponAlive && w.WeaponHasMaterial) m = MAT_WEAPON;
        if (!m && w.HitterAlive && w.HitterHasMaterial) {
            m = MAT_HITTER;
            if (h.Lightning) m = MAT_LIGHTNING;
            else if (h.Divine) m = MAT_DIVINE;
            else if (h.Unholy) m = MAT_UNHOLY;
            else if (w.HitterCreature) m = w.ArmourDamageMaterial;
        }
        w.SoundMaterial = m; w.Trace += 's';
    }
}

static World Base()
{
    World w; w.HitterAlive = w.WeaponAlive = true; w.Health = 10.0f; w.HasSound = true; w.Giving = true;
    w.HitterHasMaterial = true; w.HeroBreaksDoors = true;
    return w;
}
static Hit BaseHit()
{
    Hit h; std::memset(&h, 0, sizeof(h)); h.HasLocation = true; h.Melee = true; h.PlayResponse = true; h.Localised = true;
    h.DamageTypes = 2; h.StatsPerType = 3; h.Damage = 4.0f;
    return h;
}

int main()
{
    int bad = 0;
    World w = Base(); Hit h = BaseHit(); h.HasLocation = false; OnHit(w, h);
    if (w.Trace != "x") bad |= 1;
    w = Base(); h = BaseHit(); h.HasLocation = false; w.HasHitLocationsTC = w.DefaultLocation = true; OnHit(w, h);
    if (w.Trace != "s" || w.Health != 6.0f || w.StatEvents != 6) bad |= 2;
    w = Base(); h = BaseHit(); w.Creature = true; w.ArmourDamageMaterial = 9; OnHit(w, h);
    if (w.Material != 9) bad |= 4;
    w = Base(); h = BaseHit(); w.Creature = true; w.ArmourDamageMaterial = 0; OnHit(w, h);
    if (w.Material != MAT_OWN) bad |= 8;
    w = Base(); h = BaseHit(); w.HitterCreature = w.Electrocute = true; OnHit(w, h);
    if (w.Trace != "e" || !h.Negated || w.Health != 10.0f) bad |= 16;
    w = Base(); h = BaseHit(); w.Door = true; w.HeroBreaksDoors = false; OnHit(w, h);
    if (!h.Negated || h.PlayResponse || w.Health != 10.0f || w.StatEvents != 0) bad |= 32;
    w = Base(); h = BaseHit(); w.AssassinRush = true; OnHit(w, h);
    if (w.Trace != "c" || h.PlayResponse || w.Health != 10.0f) bad |= 64;
    w = Base(); h = BaseHit(); h.Blocked = true; OnHit(w, h);
    if (w.BlockParries != 1 || w.Health != 10.0f || w.StatEvents != 0) bad |= 128;
    w = Base(); h = BaseHit(); w.NegateAll = w.Creature = true; w.ArmourBlockMaterial = 7; OnHit(w, h);
    if (w.Material != 7 || !h.Negated || w.Health != 10.0f) bad |= 256;
    w = Base(); h = BaseHit(); w.AbsorbHitsLeft = 1; OnHit(w, h); OnHit(w, h);
    if (w.Health != 6.0f) bad |= 512;
    w = Base(); h = BaseHit(); w.Health = 3.0f; OnHit(w, h);
    if (w.Kills != 1 || w.Trace != "ks") bad |= 1024;
    w = Base(); h = BaseHit(); w.Summoned = true; OnHit(w, h);
    if (w.StatEvents != 0) bad |= 2048;
    w = Base(); h = BaseHit(); w.WeaponHasMaterial = true; OnHit(w, h);
    if (w.SoundMaterial != MAT_WEAPON) bad |= 4096;
    w = Base(); h = BaseHit(); h.Divine = true; OnHit(w, h);
    if (w.SoundMaterial != MAT_DIVINE) bad |= 8192;
    w = Base(); h = BaseHit(); w.HitterCreature = true; w.ArmourDamageMaterial = 11; OnHit(w, h);
    if (w.SoundMaterial != 11) bad |= 16384;
    if (bad) { std::printf("00839940_TEST FAIL %d\n", bad); return 1; }
    std::printf("00839940_TEST PASS\n");
    return 0;
}