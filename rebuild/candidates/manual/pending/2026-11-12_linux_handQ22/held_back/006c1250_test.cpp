// Behaviour test for CTCBuyableHouse::FrameUpdate (0x006C1250): a standalone model of the frame update -- one-time dressing setup,
// the 75-frame check throttle, main / dressing sign posts, the rent-day bookkeeping and the rent payout trigger.
#include <cstdio>

struct Def { long Price[4]; long PriceCount; long DaysBetweenRentPayments; };

struct Model {
    // inputs
    long frame, day;
    bool needs_main, can_show, gui_busy, sign_alive_after_create, has_shop, used;
    // members
    Def* PDef;
    bool OwnedByPlayer, Rented, InitiallySetupDressLevel, HasPreviouslyBeenBuyable;
    long DayNextRentIsDue, CurrentDressLevel, FrameToNext, VirtualMoneyBags;
    bool main_sign, dressing_sign;
    // effects
    int setups, main_created, dressing_created, main_killed, dressing_killed, refreshes, texts, rent_created;
    bool shop_text;

    long GetRentsDue() const
    {
        if (PDef != 0 && Rented) {
            if (day >= DayNextRentIsDue) {
                long rents = (day - DayNextRentIsDue) / PDef->DaysBetweenRentPayments + 1;
                if (rents > 0)
                    return rents;
            }
        }
        return 0;
    }
    long GetPriceForNextDressLevel() const
    {
        if (PDef != 0 && CurrentDressLevel >= 0 && (unsigned long)(CurrentDressLevel + 1) < (unsigned long)PDef->PriceCount)
            return PDef->Price[CurrentDressLevel + 1] - PDef->Price[CurrentDressLevel];
        return -1;
    }
    bool NeedsDressingSignPost() const { return OwnedByPlayer && GetPriceForNextDressLevel() > 0; }

    void FrameUpdate()
    {
        if (!InitiallySetupDressLevel) {
            ++setups;
            InitiallySetupDressLevel = true;
        }
        if (frame < FrameToNext)
            return;
        FrameToNext = frame + 75;
        if (needs_main) {
            if (!main_sign && can_show && !gui_busy) {
                ++main_created; main_sign = sign_alive_after_create;
                if (main_sign && !HasPreviouslyBeenBuyable) {
                    ++refreshes; ++texts; shop_text = has_shop;
                }
                HasPreviouslyBeenBuyable = true;
            }
        } else if (main_sign) {
            ++main_killed; main_sign = false; ++refreshes;
        }
        if (NeedsDressingSignPost()) {
            if (!dressing_sign) { ++dressing_created; dressing_sign = true; }
        } else if (dressing_sign) {
            ++dressing_killed; dressing_sign = false;
        }
        if (used && Rented) {
            if (DayNextRentIsDue == 0x7fffffff)
                DayNextRentIsDue = day + 1;
        } else
            DayNextRentIsDue = 0x7fffffff;
        if (GetRentsDue() > 0 || VirtualMoneyBags > 0)
            ++rent_created;
    }
};

static Model Make(Def* def)
{
    Model m = Model();
    m.PDef = def; m.frame = 100; m.day = 10; m.FrameToNext = 0; m.DayNextRentIsDue = 0x7fffffff; m.CurrentDressLevel = 0;
    return m;
}

int main()
{
    Def def = { { 100, 250, 250, 0 }, 3, 3 };
    Model a = Make(&def);
    a.needs_main = true; a.can_show = true; a.sign_alive_after_create = true; a.has_shop = true;
    a.FrameUpdate();
    if (a.setups != 1 || a.main_created != 1 || a.texts != 1 || !a.shop_text || !a.HasPreviouslyBeenBuyable || a.FrameToNext != 175) {
        std::printf("006c1250_TEST FAIL\n"); return 1;
    }
    a.frame = 150; a.FrameUpdate();                      // throttled
    if (a.setups != 1 || a.main_created != 1) { std::printf("006c1250_TEST FAIL\n"); return 1; }
    // owned, dearer next level -> dressing sign; rented and used -> rent day set, then rent due later
    Model b = Make(&def);
    b.OwnedByPlayer = true; b.Rented = true; b.used = true; b.main_sign = true;
    b.FrameUpdate();
    if (b.main_killed != 1 || b.dressing_created != 1 || b.DayNextRentIsDue != 11 || b.rent_created != 0) {
        std::printf("006c1250_TEST FAIL\n"); return 1;
    }
    b.frame = 300; b.day = 17; b.CurrentDressLevel = 1;   // no dearer level now; 3 rents due
    b.FrameUpdate();
    if (b.dressing_killed != 1 || b.GetRentsDue() != 3 || b.rent_created != 1) { std::printf("006c1250_TEST FAIL\n"); return 1; }
    // not used: rent day reset; virtual money bags still trigger payout
    b.frame = 400; b.used = false; b.VirtualMoneyBags = 1; b.FrameUpdate();
    if (b.DayNextRentIsDue != 0x7fffffff || b.rent_created != 2) { std::printf("006c1250_TEST FAIL\n"); return 1; }
    std::printf("006c1250_TEST PASS\n");
    return 0;
}
