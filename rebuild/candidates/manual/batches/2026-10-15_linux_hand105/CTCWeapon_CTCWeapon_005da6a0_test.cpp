#include <cstdio>
struct CThing { int id; };
struct CWeaponDef;
struct CTCBase_Calls {
    virtual void v00() { }
    CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs;
    CTCBase_Calls(CThing* t) : ParentThing(t), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { }
    virtual ~CTCBase_Calls() { }
};
struct CDefPointer_CWeaponDef { const CWeaponDef* Object; CDefPointer_CWeaponDef() : Object(0) { } };
struct CTCWeapon : CTCBase_Calls {
    CDefPointer_CWeaponDef WeaponDef; bool ProjectileWeaponLoaded; long AimingTime; bool AimingTimeOverride;
    long NumThingsKilled; long TimeWielded; bool Wielded;
    virtual void v00() { }
    CTCWeapon(CThing* thing) : CTCBase_Calls(thing), ProjectileWeaponLoaded(false), AimingTimeOverride(false), NumThingsKilled(0), TimeWielded(0), Wielded(false) { AimingTime = -1; }
};
int main()
{
    CThing t; CTCWeapon w(&t);
    if (w.ParentThing != &t || w.WeaponDef.Object != 0 || w.ProjectileWeaponLoaded || w.AimingTime != -1 || w.AimingTimeOverride
        || w.NumThingsKilled != 0 || w.TimeWielded != 0 || w.Wielded) { std::printf("005da6a0_TEST FAIL\n"); return 1; }
    std::printf("005da6a0_TEST PASS\n");
    return 0;
}
