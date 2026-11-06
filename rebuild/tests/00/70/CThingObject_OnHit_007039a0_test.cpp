// Behaviour test for CThingObject::OnHit (0x007039A0): melee + directional + localised forwards to the shot definition; a destroyed
// object hit by a deed-log holder posts vandalism (village member / vandalism door / owned by a village member), incomprehension for a
// player-owned unrented house, an alarm + shopkeeper witness for a shop door, docks morality on vandalism; otherwise a BARREL def posts
// vandalism (in a village) or incomprehension; a damageable object gets a wobble unless it is a door the hero cannot break down or a door
// of a village in limbo.
#include <cstdio>
#include <cstring>
struct Obj {
    // inputs
    bool hitterAlive, melee, directional, localised, hasShot, destroyed, hitterHasLog, villageMember, doorVandal, ownedByVillager;
    bool ownerHouseOwnedUnrented, isDoor, shopWithKeeper, barrel, damageable, heroCannotBreakDoors, villageInLimbo, hasWobble;
    // outputs
    char log[256]; float morality;
    void Ev(const char* e) { std::strcat(log, e); std::strcat(log, ";"); }
    void OnHit()
    {
        if (hitterAlive) {
            if (melee && directional && localised && hasShot) Ev("OnHitByMelee");
            if (destroyed && hitterHasLog) {
                bool posted = false;
                if (villageMember || (isDoor && doorVandal) || ownedByVillager) {
                    if (ownerHouseOwnedUnrented) Ev("INCOMPREHENSION/2");
                    else {
                        if (isDoor && ownedByVillager && shopWithKeeper) { Ev("ALARM"); Ev("VANDALISM/13+KEEPER"); }
                        else Ev("VANDALISM/2");
                        morality += -1.0f;
                    }
                    posted = true;
                }
                if (!posted && barrel) {
                    if (villageMember) { Ev("VANDALISM/2"); morality += -1.0f; }
                    else Ev("INCOMPREHENSION/2");
                }
            }
        }
        if (damageable) {
            if (isDoor && hitterAlive && heroCannotBreakDoors) return;
            if (isDoor && ownedByVillager && villageInLimbo) return;
            if (!hasWobble) { Ev("CTCWobble"); hasWobble = true; }
        }
    }
};
static void Make(Obj& o) { std::memset(&o, 0, sizeof(o)); o.hitterAlive = true; o.hitterHasLog = true; o.damageable = true; }
int main()
{
    Obj a; Make(a); a.melee = a.directional = a.localised = a.hasShot = true; a.OnHit();
    bool ok = std::strcmp(a.log, "OnHitByMelee;CTCWobble;") == 0;
    Obj b; Make(b); b.destroyed = true; b.villageMember = true; b.OnHit();
    ok = ok && std::strcmp(b.log, "VANDALISM/2;CTCWobble;") == 0 && b.morality < -0.5f;
    Obj c; Make(c); c.destroyed = true; c.isDoor = true; c.ownedByVillager = true; c.shopWithKeeper = true; c.OnHit();
    ok = ok && std::strcmp(c.log, "ALARM;VANDALISM/13+KEEPER;CTCWobble;") == 0;
    Obj d; Make(d); d.destroyed = true; d.ownedByVillager = true; d.ownerHouseOwnedUnrented = true; d.OnHit();
    ok = ok && std::strcmp(d.log, "INCOMPREHENSION/2;CTCWobble;") == 0 && d.morality == 0.0f;
    Obj e; Make(e); e.destroyed = true; e.barrel = true; e.hasWobble = true; e.OnHit();
    ok = ok && std::strcmp(e.log, "INCOMPREHENSION/2;") == 0;
    Obj f; Make(f); f.isDoor = true; f.heroCannotBreakDoors = true; f.OnHit();
    ok = ok && f.log[0] == 0;
    Obj g; Make(g); g.isDoor = true; g.ownedByVillager = true; g.villageInLimbo = true; g.hitterAlive = false; g.OnHit();
    ok = ok && g.log[0] == 0;
    std::printf(ok ? "007039a0_TEST PASS\n" : "007039a0_TEST FAIL\n");
    return ok ? 0 : 1;
}