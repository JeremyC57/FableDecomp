// Behaviour test for 0x00946400 CAIStateGroup_PrepareMeal::IsRunnable(): the row's logic over a flat model (the intelligent
// pointer as a plain pointer, the cooking pot's action points as a free-slot flag); checks the reset, the home and meal-time
// gates, the missing-component gate, the claim and the dead-pot result.
#include <cstdio>
struct CThing {
    bool HasActionPoints, FreePoint, Dead;
    long FreeIndex;
    bool IsDead() const { return Dead; }
};
struct CAIStateGroup_PrepareMeal {
    CThing* PCookingPot;
    long ActionPointIndex;
    bool HasHome, MealTime;
    CThing* FreePot;
    bool IsTimeToPrepareMeal() const { return MealTime; }
    CThing* GetFreeCookingPot() const { return FreePot; }
    bool IsRunnable();
};
static bool GetFreeActionPoint(CThing& pot, long& index)
{
    if (!pot.FreePoint) return false;
    index = pot.FreeIndex;
    return true;
}
bool CAIStateGroup_PrepareMeal::IsRunnable()
{
    PCookingPot = 0;
    ActionPointIndex = -1;
    if (HasHome && IsTimeToPrepareMeal()) {
        CThing* pot = GetFreeCookingPot();
        if (pot && pot->HasActionPoints) {
            if (GetFreeActionPoint(*pot, ActionPointIndex))
                PCookingPot = pot;
        }
    }
    if (PCookingPot && !PCookingPot->IsDead())
        return true;
    return false;
}
static int Run(bool home, bool meal, CThing* pot, CThing* old, long* index)
{
    CAIStateGroup_PrepareMeal s; s.PCookingPot = old; s.ActionPointIndex = 99; s.HasHome = home; s.MealTime = meal; s.FreePot = pot;
    bool r = s.IsRunnable();
    *index = s.ActionPointIndex;
    if (r && s.PCookingPot != pot) return -1;
    return r ? 1 : 0;
}
int main()
{
    CThing good = { true, true, false, 3 }, dead = { true, true, true, 2 }, bare = { false, true, false, 1 }, full = { true, false, false, 4 };
    long index = 0;
    bool ok = true;
    ok = ok && Run(true, true, &good, &dead, &index) == 1 && index == 3;    // claims point 3
    ok = ok && Run(false, true, &good, &good, &index) == 0 && index == -1;  // no home: reset, not runnable
    ok = ok && Run(true, false, &good, &good, &index) == 0 && index == -1;  // not meal time
    ok = ok && Run(true, true, 0, &good, &index) == 0 && index == -1;       // no free pot
    ok = ok && Run(true, true, &bare, 0, &index) == 0 && index == -1;       // pot without action points
    ok = ok && Run(true, true, &full, 0, &index) == 0 && index == -1;       // no free point
    ok = ok && Run(true, true, &dead, 0, &index) == 0 && index == 2;        // claimed, but the pot is dead
    std::printf(ok ? "00946400_TEST PASS\n" : "00946400_TEST FAIL\n");
    return ok ? 0 : 1;
}