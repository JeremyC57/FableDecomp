// Behaviour test for 0x008D5480 CCreatureAction_ReloadMissileWeapon::FinishAction: a live ammo thing is removed from the
// carrier, killed and cleared, and the live weapon in the primary slot is marked loaded; the base FinishAction always runs.
#include <cstdio>
struct CThing;
struct CTCWeapon { int loaded; void SetProjectileWeaponLoaded(bool l, CThing*) { loaded = l ? 1 : 0; } };
struct CTCCarrying { CThing* removed; CThing* primary; void RemoveThing(CThing* t) { removed = t; } CThing* GetThingInPrimarySlot() { return primary; } };
struct CThing {
    bool dead; bool killed; bool hasCarrying; CTCCarrying carrying; bool hasWeapon; CTCWeapon weapon;
    void Kill(bool) { killed = true; }
    bool GetCarrying(CTCCarrying** out) { if (!hasCarrying) return false; *out = &carrying; return true; }
};
struct CPtr { CThing* item; CThing* GetPItem() const { return item; } void SetPItem(CThing* t) { item = t; } };
inline bool IS_THING_ALIVE(const CPtr& p) { if (p.GetPItem() != 0) { if (!p.GetPItem()->dead) return true; } return false; }
struct CAction {
    CPtr PCreature; CPtr PAmmo; int baseFinish;
    void FinishAction()
    {
        CTCCarrying* carrying;
        if (IS_THING_ALIVE(PAmmo) && PCreature.GetPItem()->GetCarrying(&carrying)) {
            carrying->RemoveThing(PAmmo.GetPItem());
            PAmmo.GetPItem()->Kill(true);
            { CPtr none; none.SetPItem(0); PAmmo.SetPItem(none.GetPItem()); }
            CThing* w = carrying->GetThingInPrimarySlot();
            if (w && !w->dead && w->hasWeapon)
                w->weapon.SetProjectileWeaponLoaded(true, 0);
        }
        ++baseFinish;
    }
};
int main()
{
    CThing bow; bow.dead = false; bow.hasWeapon = true; bow.weapon.loaded = 0;
    CThing hero; hero.hasCarrying = true; hero.carrying.removed = 0; hero.carrying.primary = &bow;
    CThing arrow; arrow.dead = false; arrow.killed = false;
    CAction a; a.PCreature.item = &hero; a.PAmmo.item = &arrow; a.baseFinish = 0;
    a.FinishAction();
    int ok = hero.carrying.removed == &arrow && arrow.killed && a.PAmmo.item == 0 && bow.weapon.loaded == 1 && a.baseFinish == 1;
    a.FinishAction();
    ok = ok && a.baseFinish == 2;
    if (!ok) { std::printf("008d5480_TEST FAIL\n"); return 1; }
    std::printf("008d5480_TEST PASS\n");
    return 0;
}