// Behaviour test for 006bf920 CTCBuyableHouse::GetPriceForNextDressLevel: Price[level+1] - Price[level] while a next level exists, else -1.
#include <cstdio>

template <class T>
struct CRetailVector
{
    T* _First; T* _Last; T* _End;
    unsigned int size() const { return (unsigned int)(_Last - _First); }
    const T& operator[](unsigned int i) const { return _First[i]; }
};

class CBuyableHouseDef { public: CRetailVector<long> Price; };

class CTCBuyableHouse
{ public:
    const CBuyableHouseDef* PDef_Object;
    long CurrentDressLevel;

    long GetPriceForNextDressLevel() const
    {
        if (PDef_Object != 0 && CurrentDressLevel >= 0 && (unsigned int)(CurrentDressLevel + 1) < PDef_Object->Price.size())
            return PDef_Object->Price[CurrentDressLevel + 1] - PDef_Object->Price[CurrentDressLevel];
        return -1;
    }
};

int main()
{
    long prices[3] = { 1000, 1500, 2500 };
    CBuyableHouseDef def; def.Price._First = prices; def.Price._Last = prices + 3; def.Price._End = prices + 3;
    CTCBuyableHouse house; house.PDef_Object = &def; house.CurrentDressLevel = 0;
    if (house.GetPriceForNextDressLevel() != 500) { std::printf("006bf920_TEST FAIL\n"); return 1; }
    house.CurrentDressLevel = 1;
    if (house.GetPriceForNextDressLevel() != 1000) { std::printf("006bf920_TEST FAIL\n"); return 1; }
    house.CurrentDressLevel = 2;
    if (house.GetPriceForNextDressLevel() != -1) { std::printf("006bf920_TEST FAIL\n"); return 1; }
    house.CurrentDressLevel = -1;
    if (house.GetPriceForNextDressLevel() != -1) { std::printf("006bf920_TEST FAIL\n"); return 1; }
    house.CurrentDressLevel = 0; house.PDef_Object = 0;
    if (house.GetPriceForNextDressLevel() != -1) { std::printf("006bf920_TEST FAIL\n"); return 1; }
    std::printf("006bf920_TEST PASS\n");
    return 0;
}