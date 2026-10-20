// Behaviour test for CTCBuyableHouse::GetPrice (0x006C0120): body copied verbatim, callee stubbed.
#include <stdio.h>

template <class T>
struct CRetailVector
{
    T* _First;
    T* _Last;
    T* _End;
    unsigned int size() const { return (unsigned int)(_Last - _First); }
};
struct CBuyableHouseDef { unsigned char _pad_0x00[0x28]; CRetailVector<long> Price; CRetailVector<long> Rent; long DaysBetweenRentPayments; long MaxRentBags; };
#pragma pack(push, 1)
struct CTCBuyableHouse
{
    void* __vftable; void* ParentThing; unsigned char _pad_0x08[0x4];
    const CBuyableHouseDef* PDef_Object;
    bool IsScripted; bool OwnedByPlayer; bool Rented; unsigned char _pad_0x13[0x1];
    long DaySomeoneWillMoveIn; long DayNextRentIsDue; long CurrentDressLevel;
    long GetValueOfTrophies() const;
    long GetPrice() const;
};
#pragma pack(pop)

static int g_trophyCalls = 0;
static long g_trophyValue = 0;
long CTCBuyableHouse::GetValueOfTrophies() const { ++g_trophyCalls; return g_trophyValue; }

long CTCBuyableHouse::GetPrice() const
{
    const CBuyableHouseDef* def = PDef_Object;
    if (def != 0 && CurrentDressLevel >= 0 && (unsigned int)CurrentDressLevel < def->Price.size())
    {
        const long* prices = def->Price._First;
        return GetValueOfTrophies() + prices[CurrentDressLevel];
    }
    return -1;
}

static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)

int main()
{
    long prices[3] = { 100, 250, 900 };
    CBuyableHouseDef def; def.Price._First = prices; def.Price._Last = prices + 3; def.Price._End = prices + 3;
    CTCBuyableHouse h; h.PDef_Object = &def; h.CurrentDressLevel = 1; g_trophyValue = 40;

    CHECK(h.GetPrice() == 290); CHECK(g_trophyCalls == 1);          // valid level: trophies + Price[level]
    h.CurrentDressLevel = 2; CHECK(h.GetPrice() == 940); CHECK(g_trophyCalls == 2);
    h.CurrentDressLevel = 3; CHECK(h.GetPrice() == -1); CHECK(g_trophyCalls == 2);   // level == size(): out of range
    h.CurrentDressLevel = -1; CHECK(h.GetPrice() == -1); CHECK(g_trophyCalls == 2);  // negative level
    h.CurrentDressLevel = 0; h.PDef_Object = 0; CHECK(h.GetPrice() == -1); CHECK(g_trophyCalls == 2); // no def
    h.PDef_Object = &def; def.Price._Last = prices; CHECK(h.GetPrice() == -1); CHECK(g_trophyCalls == 2); // empty vector
    def.Price._Last = prices + 3; CHECK(h.GetPrice() == 140); CHECK(g_trophyCalls == 3);

    if (fails) { printf("006c0120_TEST FAIL (%d)\n", fails); return 1; }
    printf("006c0120_TEST PASS\n");
    return 0;
}