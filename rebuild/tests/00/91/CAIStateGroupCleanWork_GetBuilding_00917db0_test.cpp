// Behaviour test for 0x00917DB0: CleanWork::GetBuilding.
#include <cstdio>
struct CThingBuilding { int id; };
struct CTCShop { unsigned char m_bClosedDown; bool open; bool IsOpeningHours() { return open; } };
struct CTCShopKeeper { CTCShop* shop; CTCShop* GetShopTC() { return shop; } };
struct CCreature { CTCShopKeeper* keeper; CThingBuilding* building;
    bool GetInterface(CTCShopKeeper** out, long t) { if (t == 0x90 && keeper) { *out = keeper; return true; } return false; }
    CThingBuilding* GetWorkBuilding() { return building; } };
static CThingBuilding* GetBuilding(CCreature* c)
{
    CTCShopKeeper* shopKeeper;
    if (c->GetInterface(&shopKeeper, 0x90)) {
        CTCShop* shop = shopKeeper->GetShopTC();
        if (shop == 0 || !shop->IsOpeningHours() || shop->m_bClosedDown) return 0;
    }
    return c->GetWorkBuilding();
}
int main()
{
    CThingBuilding b = { 1 };
    CCreature c = { 0, &b };
    int bad = 0;
    if (GetBuilding(&c) != &b) bad = 1;
    CTCShop shop = { 0, true }; CTCShopKeeper k = { &shop }; c.keeper = &k;
    if (GetBuilding(&c) != &b) bad = 2;
    shop.open = false; if (GetBuilding(&c) != 0) bad = 3;
    shop.open = true; shop.m_bClosedDown = 1; if (GetBuilding(&c) != 0) bad = 4;
    k.shop = 0; if (GetBuilding(&c) != 0) bad = 5;
    if (bad) { std::printf("00917db0_TEST FAIL\n"); return 1; }
    std::printf("00917db0_TEST PASS\n");
    return 0;
}