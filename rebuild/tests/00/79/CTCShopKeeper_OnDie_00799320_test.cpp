#include <cstdio>
class CThingCreatureBase { public: int id; };
class CThing { public: unsigned char m_flags; };
class CTCShop { public: CThingCreatureBase* m_keeper; int m_calls; void SetShopKeeper(CThingCreatureBase* k) { m_keeper = k; ++m_calls; } };
class CTCShopKeeper { public:
    CThing* m_pOwner; CTCShop* m_pShop;
    CTCShop* GetShopTC() { return m_pShop; }
    void OnDie()
    {
        if ((m_pOwner->m_flags & 0x10) && GetShopTC())
            GetShopTC()->SetShopKeeper(0);
    }
};
int main()
{
    CThingCreatureBase keeper; CThing owner; owner.m_flags = 0;
    CTCShop shop; shop.m_keeper = &keeper; shop.m_calls = 0;
    CTCShopKeeper k; k.m_pOwner = &owner; k.m_pShop = &shop;
    k.OnDie();
    if (shop.m_calls != 0) { std::printf("00799320_TEST FAIL\n"); return 1; }
    owner.m_flags = 0x10; k.m_pShop = 0; k.OnDie();
    k.m_pShop = &shop; k.OnDie();
    if (shop.m_calls != 1 || shop.m_keeper != 0) { std::printf("00799320_TEST FAIL\n"); return 1; }
    std::printf("00799320_TEST PASS\n");
    return 0;
}