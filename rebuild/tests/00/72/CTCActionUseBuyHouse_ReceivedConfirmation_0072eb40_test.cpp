// Behaviour test for CTCActionUseBuyHouse::ReceivedConfirmation 0x0072EB40: a copy of the model with mock callees.
#include <cstdio>

class CTCHeroStats { public:
    long Money;
    long NumberOfShopsOwned;
    long NumberOfHousesOwned;
    int  addMoneyCalls;
    bool lastFlag;
    void AddMoney(long amount, bool flag) { Money += amount; ++addMoneyCalls; lastFlag = flag; }
};
enum { TCI_HERO_STATS = 4, TCI_SHOP = 83 };
class CThing { public:
    unsigned long UsingComponents[9];
    CTCHeroStats* Stats;
    bool HasTC(int id) const { return (UsingComponents[id >> 5] & (1UL << (id & 31))) != 0; }
    bool GetTC(int id, CTCHeroStats** out) { if (HasTC(id)) { *out = Stats; return true; } return false; }
};
static CThing* g_hero;
class CPlayer { public: CThing* GetPControlledCreature() const { return g_hero; } };
class CPlayerManager { public: CPlayer Main; CPlayer& GetMainPlayer() { return Main; } };
static CPlayerManager g_pm;
class CTCBase { public: CPlayerManager& GetPlayerManager() { return g_pm; } };
class CTCBuyableHouse { public:
    CThing* ParentThing;
    bool OwnedByPlayer;
    bool Rented;
    long Price;
    long DressPrice;
    long DressLevel;
    int  setRentedCalls;
    bool GetOwnedByPlayer() const { return OwnedByPlayer; }
    bool GetRented() const { return Rented; }
    CThing& GetParentThing() { return *ParentThing; }
    long GetPrice() const { return Price; }
    long GetPriceForNextDressLevel() const { return DressPrice; }
    void IncrementDressLevel() { ++DressLevel; }
    void SetRented(bool r) { Rented = r; ++setRentedCalls; }
    void SetOwnedByPlayer(bool o) { OwnedByPlayer = o; }
};
enum EHouseQuestion { HOUSE_QUESTION_NONE = 0, HOUSE_QUESTION_BUY = 1, HOUSE_QUESTION_RENT = 2, HOUSE_QUESTION_DRESS = 3 };
struct CTCActionUseBuyHouse : CTCBase {
    EHouseQuestion HouseQuestion;
    void ReceivedConfirmation(CTCBuyableHouse& house)
    {
        switch (HouseQuestion) {
        case HOUSE_QUESTION_BUY:
            if (!house.GetOwnedByPlayer()) {
                CThing* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
                CTCHeroStats* stats;
                if (hero && hero->GetTC(TCI_HERO_STATS, &stats)) {
                    stats->AddMoney(-house.GetPrice(), true);
                    house.SetOwnedByPlayer(true);
                    if (house.GetParentThing().HasTC(TCI_SHOP))
                        ++stats->NumberOfShopsOwned;
                    else
                        ++stats->NumberOfHousesOwned;
                }
            } else {
                CThing* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
                CTCHeroStats* stats;
                if (hero && hero->GetTC(TCI_HERO_STATS, &stats)) {
                    stats->AddMoney(house.GetPrice(), true);
                    house.SetOwnedByPlayer(false);
                    if (house.GetRented())
                        house.SetRented(false);
                    if (house.GetParentThing().HasTC(TCI_SHOP))
                        --stats->NumberOfShopsOwned;
                    else
                        --stats->NumberOfHousesOwned;
                }
            }
            break;
        case HOUSE_QUESTION_RENT:
            house.SetRented(!house.GetRented());
            break;
        case HOUSE_QUESTION_DRESS: {
            CThing* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
            CTCHeroStats* stats;
            if (hero && hero->GetTC(TCI_HERO_STATS, &stats)) {
                stats->AddMoney(-house.GetPriceForNextDressLevel(), true);
                house.IncrementDressLevel();
            }
            break;
        }
        }
    }
};

static int fail(int n) { std::printf("0072eb40_TEST FAIL (%d)\n", n); return 1; }

int main()
{
    CTCHeroStats stats = { 1000, 0, 0, 0, false };
    CThing hero = { { 0 }, &stats };
    hero.UsingComponents[0] = 1UL << TCI_HERO_STATS;
    CThing houseThing = { { 0 }, 0 };
    CThing shopThing = { { 0 }, 0 };
    shopThing.UsingComponents[TCI_SHOP >> 5] = 1UL << (TCI_SHOP & 31);
    if (shopThing.UsingComponents[2] != 0x80000UL) return fail(1);
    CTCBuyableHouse house = { &houseThing, false, false, 300, 70, 0, 0 };
    CTCBuyableHouse shop = { &shopThing, false, false, 450, 0, 0, 0 };
    CTCActionUseBuyHouse action;
    g_hero = &hero;

    action.HouseQuestion = HOUSE_QUESTION_BUY;                 // buy a house
    action.ReceivedConfirmation(house);
    if (stats.Money != 700 || !house.OwnedByPlayer || stats.NumberOfHousesOwned != 1 || stats.NumberOfShopsOwned != 0 || !stats.lastFlag) return fail(2);
    action.ReceivedConfirmation(shop);                         // buy a shop
    if (stats.Money != 250 || !shop.OwnedByPlayer || stats.NumberOfHousesOwned != 1 || stats.NumberOfShopsOwned != 1) return fail(3);

    action.HouseQuestion = HOUSE_QUESTION_RENT;                // rent toggles
    action.ReceivedConfirmation(house);
    if (!house.Rented || house.setRentedCalls != 1 || stats.Money != 250) return fail(4);
    action.ReceivedConfirmation(shop);
    action.ReceivedConfirmation(shop);
    if (shop.Rented || shop.setRentedCalls != 2) return fail(5);

    action.HouseQuestion = HOUSE_QUESTION_DRESS;               // dress level
    action.ReceivedConfirmation(house);
    if (stats.Money != 180 || house.DressLevel != 1 || stats.addMoneyCalls != 3) return fail(6);

    action.HouseQuestion = HOUSE_QUESTION_BUY;                 // sell the rented house: rent stops, money back
    action.ReceivedConfirmation(house);
    if (stats.Money != 480 || house.OwnedByPlayer || house.Rented || house.setRentedCalls != 2 || stats.NumberOfHousesOwned != 0 || stats.NumberOfShopsOwned != 1) return fail(7);
    action.ReceivedConfirmation(shop);                         // sell the unrented shop: SetRented not called
    if (stats.Money != 930 || shop.OwnedByPlayer || shop.setRentedCalls != 2 || stats.NumberOfShopsOwned != 0) return fail(8);

    g_hero = 0;                                                // no hero: nothing happens in the buy and dress arms
    action.ReceivedConfirmation(house);
    action.HouseQuestion = HOUSE_QUESTION_DRESS;
    action.ReceivedConfirmation(house);
    if (stats.Money != 930 || house.OwnedByPlayer || house.DressLevel != 1 || stats.addMoneyCalls != 5) return fail(9);
    g_hero = &hero;                                            // hero without the stats component: nothing either
    hero.UsingComponents[0] = 0;
    action.HouseQuestion = HOUSE_QUESTION_BUY;
    action.ReceivedConfirmation(house);
    if (stats.Money != 930 || house.OwnedByPlayer || stats.NumberOfHousesOwned != 0) return fail(10);
    hero.UsingComponents[0] = 1UL << TCI_HERO_STATS;

    action.HouseQuestion = HOUSE_QUESTION_NONE;                // no question pending: nothing
    action.ReceivedConfirmation(house);
    if (stats.Money != 930 || house.OwnedByPlayer || house.Rented || stats.addMoneyCalls != 5) return fail(11);

    std::printf("0072eb40_TEST PASS\n");
    return 0;
}