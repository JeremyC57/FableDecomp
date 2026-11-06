// Behaviour test for CTCActionUseBed::OnActionUse (0x007EBEA0): a model of the row's decisions -- silent refusal while the
// bed's building belongs to a village in limbo / hostile for crime, with no hero, in melee or during a fade; "fail" when sleeping
// is disabled, the user is berserk or the bed is taken; "check" (cost 0) for an owned bed, an unoccupied villager's bed or a bought
// (not rented) house; the tavern's priced offer; "illegal" (cost -1) otherwise; the confirmation flag and game event follow an ask.
#include <stdio.h>
enum Result { SILENT, FAIL, CHECK, COST, ILLEGAL };
struct Bed {
    bool VillageLimbo, VillageCrime, HaveHero, HeroInMelee, Fading, SleepEnabled, Berserk, HaveBed, BedEmpty;
    bool OwnedByHero, UseableByHero, HaveBuilding, Occupied, HouseOwned, HouseRented, Tavern;
    long TavernCost, Cost; bool WaitingForConfirmation; int Events;
};
static Result Use(Bed& b)
{
    if (b.VillageLimbo || b.VillageCrime) return SILENT;
    if (!b.HaveHero || b.HeroInMelee || b.Fading) return SILENT;
    if (!b.SleepEnabled) return FAIL;
    if (b.Berserk) return FAIL;
    if (b.HaveBed && !b.BedEmpty) return FAIL;
    Result r;
    if (b.OwnedByHero) { r = CHECK; b.Cost = 0; }
    else if (b.UseableByHero) {
        if (!b.HaveBuilding) { r = ILLEGAL; b.Cost = -1; }
        else if (!b.Occupied) { r = CHECK; b.Cost = 0; }
        else if (b.HouseOwned && !b.HouseRented) { r = CHECK; b.Cost = 0; }
        else if (b.Tavern) { r = COST; b.Cost = b.TavernCost; }
        else { r = ILLEGAL; b.Cost = -1; }
    }
    else return FAIL;
    b.WaitingForConfirmation = true;
    ++b.Events;
    return r;
}
int main()
{
    int ok = 1;
    Bed base = { false, false, true, false, false, true, false, true, true,
                 false, true, true, true, false, false, true, 25, 99, false, 0 };
    Bed b = base; ok &= Use(b) == COST && b.Cost == 25 && b.WaitingForConfirmation && b.Events == 1;
    b = base; b.VillageCrime = true; ok &= Use(b) == SILENT && !b.WaitingForConfirmation;
    b = base; b.HeroInMelee = true; ok &= Use(b) == SILENT;
    b = base; b.SleepEnabled = false; ok &= Use(b) == FAIL;
    b = base; b.Berserk = true; ok &= Use(b) == FAIL && b.Events == 0;
    b = base; b.BedEmpty = false; ok &= Use(b) == FAIL;
    b = base; b.OwnedByHero = true; ok &= Use(b) == CHECK && b.Cost == 0;
    b = base; b.Occupied = false; ok &= Use(b) == CHECK;
    b = base; b.HouseOwned = true; ok &= Use(b) == CHECK;
    b = base; b.HouseOwned = true; b.HouseRented = true; ok &= Use(b) == COST;
    b = base; b.Tavern = false; ok &= Use(b) == ILLEGAL && b.Cost == -1;
    b = base; b.HaveBuilding = false; ok &= Use(b) == ILLEGAL;
    b = base; b.UseableByHero = false; ok &= Use(b) == FAIL && !b.WaitingForConfirmation;
    printf(ok ? "007ebea0_TEST PASS\n" : "007ebea0_TEST FAIL\n");
    return ok ? 0 : 1;
}
