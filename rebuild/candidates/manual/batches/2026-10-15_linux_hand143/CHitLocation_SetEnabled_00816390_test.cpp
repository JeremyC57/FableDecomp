// Behaviour test for 0x00816390 (CHitLocation::SetEnabled): the default location registers / unregisters itself with the owner's hit-locations component.
#include <cstdio>
struct HitLocation;
struct HitLocations { HitLocation* PDefaultHitLocation; };
struct Thing { HitLocations* locations; };
struct Def { bool Default; };
struct HitLocation {
    Thing* POwnerThing; Def* PDef; bool Enabled;
    void SetEnabled(bool enabled)
    {
        Enabled = enabled;
        if (PDef->Default) {
            HitLocations* l = POwnerThing->locations;
            if (enabled) l->PDefaultHitLocation = this;
            else if (l->PDefaultHitLocation == this) l->PDefaultHitLocation = 0;
        }
    }
};
int main()
{
    HitLocations locs = { 0 }; Thing owner = { &locs }; Def def = { true }, plain = { false };
    HitLocation a = { &owner, &def, false }, b = { &owner, &def, false }, c = { &owner, &plain, false };
    a.SetEnabled(true); int ok = a.Enabled && locs.PDefaultHitLocation == &a;
    b.SetEnabled(false); ok = ok && locs.PDefaultHitLocation == &a && !b.Enabled;
    c.SetEnabled(true); ok = ok && c.Enabled && locs.PDefaultHitLocation == &a;
    a.SetEnabled(false); ok = ok && locs.PDefaultHitLocation == 0 && !a.Enabled;
    if (!ok) { std::printf("00816390_TEST FAIL\n"); return 1; }
    std::printf("00816390_TEST PASS\n");
    return 0;
}
