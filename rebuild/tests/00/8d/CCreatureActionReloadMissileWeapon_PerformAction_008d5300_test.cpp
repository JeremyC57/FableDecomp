// Behaviour test for CCreatureAction_ReloadMissileWeapon::PerformAction (0x008D5300): a standalone copy of the model. The creature's
// carrying component, the weapon thing in its primary slot, the weapon def (Ammo / MissileLoadCarrySlot) and the created ammunition
// object are small recording stand-ins; the function body is the row's logic line for line.
#include <cstdio>
#include <cstring>

enum ETCInterfaceType { TCI_CARRYING = 70, TCI_WEAPON = 72 };
class C3DVector { public: float X, Y, Z; };
class CWeaponDef { public: long RefCount; long MissileLoadCarrySlot; long Ammo; int deleted; };
struct CDefPointerWeapon {
    CWeaponDef* Object;
    CDefPointerWeapon() : Object(0) { }
    ~CDefPointerWeapon() { if (Object && --Object->RefCount == 0) Object->deleted = 1; }
    const CWeaponDef* operator->() const { return Object; }
};
class CTCCarrying;
class CThing { public:
    unsigned long UsingComponents[9];
    CTCCarrying*  carrying;
    CWeaponDef*   weaponDef;       // the base def's CWeaponDef sub-def (null: none)
    C3DVector     pos;
    signed char   PlayerNumber;
    bool          Dead;
    int           killed;
    CThing() : carrying(0), weaponDef(0), PlayerNumber(0), Dead(false), killed(0) { std::memset(UsingComponents, 0, sizeof(UsingComponents)); pos.X = pos.Y = pos.Z = 0.0f; }
    void SetTC(ETCInterfaceType t) { UsingComponents[t >> 5] |= 1UL << (t & 31); }
    bool HasTC(ETCInterfaceType t) const { return (UsingComponents[t >> 5] & (1UL << (t & 31))) != 0; }
    bool GetTC(CTCCarrying** out) { if (!HasTC(TCI_CARRYING)) return false; *out = carrying; return true; }
    bool GetSubDef(CDefPointerWeapon* out) const { if (!weaponDef) return false; ++weaponDef->RefCount; out->Object = weaponDef; return true; }
    bool IsDead() const { return Dead; }
    const C3DVector& GetPos() const { return pos; }
    unsigned long GetPlayerNumber() const { return PlayerNumber; }
    void Kill(bool) { Dead = true; ++killed; }
};
class CTCCarrying { public:
    CThing* primary;
    CThing* slots[8];
    int removed, put; bool putImmediately; long putSlot; CThing* removedThing;
    CTCCarrying() : primary(0), removed(0), put(0), putImmediately(false), putSlot(-1), removedThing(0) { for (int i = 0; i < 8; ++i) slots[i] = 0; }
    CThing* GetThingInPrimarySlot() const { return primary; }
    CThing* GetThingInCarrySlot(long slot) const { return slots[slot]; }
    void RemoveThing(CThing& thing) { ++removed; removedThing = &thing; for (int i = 0; i < 8; ++i) if (slots[i] == &thing) slots[i] = 0; }
    void PutThingInCarrySlot(CThing& thing, long slot, bool immediately) { ++put; putSlot = slot; putImmediately = immediately; slots[slot] = &thing; }
};
static bool IS_THING_ALIVE(CThing* const& thing)
{
    if (thing != 0) {
        if (!thing->IsDead()) return true;
    }
    return false;
}
// CThingObject::Create stand-in: records its arguments and hands out the prepared object.
static CThing* g_createResult; static int g_creates; static long g_createDef; static unsigned long g_createPlayer; static C3DVector g_createPos;
static const void* g_createSet; static unsigned long g_createFlags; static bool g_createNameEmpty;
static CThing* Create(long defIndex, const C3DVector& position, long playerNumber, const void* rhSet, unsigned long flags, const char* scriptName)
{
    ++g_creates; g_createDef = defIndex; g_createPos = position; g_createPlayer = playerNumber; g_createSet = rhSet; g_createFlags = flags;
    g_createNameEmpty = scriptName[0] == 0;
    return g_createResult;
}
class CCreatureAction_ReloadMissileWeapon { public:
    CThing* PCreature;
    CThing* PAmmo;
    void PerformAction()
    {
        CTCCarrying* ptc_carry;
        if (PCreature->GetTC(&ptc_carry)) {
            CThing* pweapon = ptc_carry->GetThingInPrimarySlot();
            if (pweapon && pweapon->HasTC(TCI_WEAPON)) {
                CDefPointerWeapon weapon_def;
                if (pweapon->GetSubDef(&weapon_def)) {
                    long ammo = weapon_def->Ammo;
                    long slot = weapon_def->MissileLoadCarrySlot;
                    if (ammo > 0 && slot > 0) {
                        PAmmo = Create(ammo, PCreature->GetPos(), PCreature->GetPlayerNumber(), 0, 0, "");
                        if (IS_THING_ALIVE(PAmmo)) {
                            CThing* existing = ptc_carry->GetThingInCarrySlot(slot);
                            if (IS_THING_ALIVE(existing)) {
                                ptc_carry->RemoveThing(*existing);
                                existing->Kill(true);
                            }
                            ptc_carry->PutThingInCarrySlot(*PAmmo, slot, true);
                        }
                    }
                }
            }
        }
    }
};
struct Scene {
    CThing creature, weapon, ammo, old; CTCCarrying carrying; CWeaponDef def; CCreatureAction_ReloadMissileWeapon action;
    Scene()
    {
        creature.SetTC(TCI_CARRYING); creature.carrying = &carrying; creature.PlayerNumber = 2; creature.pos.X = 1.0f; creature.pos.Y = 2.0f; creature.pos.Z = 3.0f;
        weapon.SetTC(TCI_WEAPON); weapon.weaponDef = &def; carrying.primary = &weapon;
        def.RefCount = 1; def.MissileLoadCarrySlot = 5; def.Ammo = 321; def.deleted = 0;
        action.PCreature = &creature; action.PAmmo = 0;
        g_createResult = &ammo; g_creates = 0; g_createDef = 0; g_createPlayer = 99; g_createSet = &g_creates; g_createFlags = 7; g_createNameEmpty = false;
    }
};
static int Fail(int n) { std::printf("008d5300_TEST FAIL (case %d)\n", n); return 1; }
int main()
{
    {   // 1: empty load slot -> ammo created at the creature's position for its player and put in the slot, nothing removed
        Scene s; s.action.PerformAction();
        if (g_creates != 1 || g_createDef != 321 || g_createPlayer != 2 || g_createSet != 0 || g_createFlags != 0 || !g_createNameEmpty) return Fail(1);
        if (g_createPos.X != 1.0f || g_createPos.Y != 2.0f || g_createPos.Z != 3.0f) return Fail(1);
        if (s.action.PAmmo != &s.ammo || s.carrying.put != 1 || s.carrying.putSlot != 5 || !s.carrying.putImmediately || s.carrying.slots[5] != &s.ammo) return Fail(1);
        if (s.carrying.removed != 0 || s.def.RefCount != 1 || s.def.deleted) return Fail(1);
    }
    {   // 2: a living thing in the load slot is removed and killed first
        Scene s; s.carrying.slots[5] = &s.old; s.action.PerformAction();
        if (s.carrying.removed != 1 || s.carrying.removedThing != &s.old || s.old.killed != 1 || !s.old.Dead) return Fail(2);
        if (s.carrying.put != 1 || s.carrying.slots[5] != &s.ammo) return Fail(2);
    }
    {   // 3: a dead thing in the load slot is left alone (not removed, not killed again), the ammo still goes in
        Scene s; s.old.Dead = true; s.carrying.slots[5] = &s.old; s.action.PerformAction();
        if (s.carrying.removed != 0 || s.old.killed != 0 || s.carrying.put != 1 || s.carrying.slots[5] != &s.ammo) return Fail(3);
    }
    {   // 4: the created object is dead (or null) -> stored in PAmmo but never put in the slot
        Scene s; s.ammo.Dead = true; s.carrying.slots[5] = &s.old; s.action.PerformAction();
        if (g_creates != 1 || s.action.PAmmo != &s.ammo || s.carrying.put != 0 || s.carrying.removed != 0 || s.old.killed != 0) return Fail(4);
        Scene n; g_createResult = 0; n.action.PAmmo = &n.old; n.action.PerformAction();
        if (g_creates != 1 || n.action.PAmmo != 0 || n.carrying.put != 0) return Fail(4);
    }
    {   // 5: Ammo <= 0 or slot <= 0 -> nothing created, the def reference is dropped again
        Scene s; s.def.Ammo = 0; s.action.PerformAction();
        if (g_creates != 0 || s.carrying.put != 0 || s.def.RefCount != 1 || s.action.PAmmo != 0) return Fail(5);
        Scene t; t.def.MissileLoadCarrySlot = 0; t.action.PerformAction();
        if (g_creates != 0 || t.carrying.put != 0 || t.def.RefCount != 1) return Fail(5);
        Scene u; u.def.Ammo = -4; u.action.PerformAction();
        if (g_creates != 0) return Fail(5);
    }
    {   // 6: gates -- no carrying component, no weapon, weapon without the weapon component, no weapon sub-def
        Scene a; std::memset(a.creature.UsingComponents, 0, sizeof(a.creature.UsingComponents)); a.action.PerformAction();
        if (g_creates != 0 || a.carrying.put != 0) return Fail(6);
        Scene b; b.carrying.primary = 0; b.action.PerformAction();
        if (g_creates != 0 || b.carrying.put != 0) return Fail(6);
        Scene c; std::memset(c.weapon.UsingComponents, 0, sizeof(c.weapon.UsingComponents)); c.action.PerformAction();
        if (g_creates != 0 || c.def.RefCount != 1) return Fail(6);
        Scene d; d.weapon.weaponDef = 0; d.action.PerformAction();
        if (g_creates != 0 || d.carrying.put != 0) return Fail(6);
    }
    {   // 7: the last def reference is released through the pointee's delete
        Scene s; s.def.RefCount = 0; s.action.PerformAction();
        if (!s.def.deleted || s.carrying.put != 1) return Fail(7);
    }
    std::printf("008d5300_TEST PASS\n");
    return 0;
}