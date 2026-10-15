// Behaviour test for 006bf960 CTCBuyableHouse::IsForSale: not owned by the player and the main sign post exists and is alive.
#include <cstdio>

struct CThing
{
    unsigned char Dead : 1;
    bool IsDead() const { return Dead; }
};

struct CBaseIntelligentPointer
{
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};

template <class T>
struct CIntelligentPointer : CBaseIntelligentPointer
{
    T* Get() const { return (T*)GetPItem(); }
};

struct CTCBuyableHouse
{
    bool OwnedByPlayer;
    CIntelligentPointer<CThing> MainSignPost;

    bool IsForSale() const
    {
        if (OwnedByPlayer)
            return false;
        if (MainSignPost.Get() && !MainSignPost.Get()->IsDead())
            return true;
        return false;
    }
};

int main()
{
    CThing sign; sign.Dead = 0;
    CTCBuyableHouse house; house.OwnedByPlayer = false; house.MainSignPost.m_pItem = &sign;
    if (!house.IsForSale()) { std::printf("006bf960_TEST FAIL\n"); return 1; }
    house.OwnedByPlayer = true;
    if (house.IsForSale()) { std::printf("006bf960_TEST FAIL\n"); return 1; }
    house.OwnedByPlayer = false; sign.Dead = 1;
    if (house.IsForSale()) { std::printf("006bf960_TEST FAIL\n"); return 1; }
    house.MainSignPost.m_pItem = 0;
    if (house.IsForSale()) { std::printf("006bf960_TEST FAIL\n"); return 1; }
    std::printf("006bf960_TEST PASS\n");
    return 0;
}