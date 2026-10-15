// Behaviour test for 006bf810 CTCBuyableHouse::GetRentsDue: rented house with a def -> (today - due) / interval + 1 once the due day is reached, else 0.
#include <cstdio>

struct CGameTime { long Day; };
struct CWorldSub { CGameTime* GameTime; };
struct CBuyableHouseDef { long DaysBetweenRentPayments; };

static CWorldSub* g_world;

struct CTCBuyableHouse
{
    const CBuyableHouseDef* PDef_Object;
    bool Rented;
    long DayNextRentIsDue;

    CWorldSub* GetWorldSub() const { return g_world; }
    long GetRentsDue() const
    {
        if (PDef_Object != 0 && Rented)
        {
            if (GetWorldSub()->GameTime->Day >= DayNextRentIsDue)
            {
                long rents = (GetWorldSub()->GameTime->Day - DayNextRentIsDue) / PDef_Object->DaysBetweenRentPayments + 1;
                if (rents > 0)
                    return rents;
            }
        }
        return 0;
    }
};

int main()
{
    CGameTime time; time.Day = 20;
    CWorldSub world; world.GameTime = &time; g_world = &world;
    CBuyableHouseDef def; def.DaysBetweenRentPayments = 5;
    CTCBuyableHouse house; house.PDef_Object = &def; house.Rented = true; house.DayNextRentIsDue = 10;
    if (house.GetRentsDue() != 3) { std::printf("006bf810_TEST FAIL\n"); return 1; }
    house.DayNextRentIsDue = 20;
    if (house.GetRentsDue() != 1) { std::printf("006bf810_TEST FAIL\n"); return 1; }
    house.DayNextRentIsDue = 21;
    if (house.GetRentsDue() != 0) { std::printf("006bf810_TEST FAIL\n"); return 1; }
    house.DayNextRentIsDue = 10; house.Rented = false;
    if (house.GetRentsDue() != 0) { std::printf("006bf810_TEST FAIL\n"); return 1; }
    house.Rented = true; house.PDef_Object = 0;
    if (house.GetRentsDue() != 0) { std::printf("006bf810_TEST FAIL\n"); return 1; }
    std::printf("006bf810_TEST PASS\n");
    return 0;
}