// Behaviour test for CTCShop::SetShopKeeper (0x007751A0): a model of the row -- a new keeper replaces the old one: the old keeper's
// workplace (alive, not a tavern) is unlocked and made non-trespassable and the stock is removed; the new keeper's workplace is
// locked and made trespassable, and a permanently stocked shop restocks its display (stock state 0 when the keeper is the shop's
// own thing or the shop is a tavern). Setting the same keeper does nothing.
#include <stdio.h>
struct Thing { bool dead; bool tavern; bool lockOverridden; bool hasTrespass; bool trespassable; Thing* workplace; };
struct Shop { Thing* parent; Thing* keeper; bool permanent; int stockState; int removed; int displays; };
static bool Alive(Thing* t) { return t && !t->dead; }
static void Workplace(Thing* keeper, bool lock)
{
    Thing* w = keeper->workplace;
    if (w && !w->dead && !w->tavern) { w->lockOverridden = lock; if (w->hasTrespass) w->trespassable = lock; }
}
static void SetShopKeeper(Shop& s, Thing* keeper)
{
    if (keeper == s.keeper) return;
    if (Alive(s.keeper)) { Workplace(s.keeper, false); ++s.removed; }
    s.keeper = keeper;
    if (Alive(s.keeper)) {
        if (s.permanent) { ++s.displays; if (s.keeper == s.parent || s.parent->tavern) s.stockState = 0; }
        Workplace(s.keeper, true);
    }
}
int main()
{
    Thing house1 = { false, false, true, true, true, 0 }, house2 = { false, false, false, true, false, 0 };
    Thing oldK = { false, false, false, false, false, &house1 }, newK = { false, false, false, false, false, &house2 };
    Thing shopThing = { false, true, false, false, false, 0 };
    Shop s = { &shopThing, &oldK, true, 5, 0, 0 };
    SetShopKeeper(s, &oldK);
    bool same = s.removed == 0 && s.displays == 0;
    SetShopKeeper(s, &newK);
    bool ok = same && !house1.lockOverridden && !house1.trespassable && s.removed == 1 && s.keeper == &newK
        && house2.lockOverridden && house2.trespassable && s.displays == 1 && s.stockState == 0;
    printf(ok ? "007751a0_TEST PASS\n" : "007751a0_TEST FAIL\n");
    return ok ? 0 : 1;
}