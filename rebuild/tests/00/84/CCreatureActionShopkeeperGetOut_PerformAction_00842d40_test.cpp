// Behaviour test for 0x00842D40 CCreatureAction_ShopkeeperGetOut::PerformAction: the goods place handed to the shop.
#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
struct CTCShop {
    struct GoodsPlace { C3DVector pos; CRightHandedSet rhset; };
    int m_goods; GoodsPlace m_place;
    void AddSaleItem(int goods, const GoodsPlace& place) { m_goods = goods; m_place = place; }
};
struct CAction {
    CTCShop* TCShop; int Goods; C3DVector Pos; CRightHandedSet RHSet;
    void PerformAction()
    {
        CTCShop::GoodsPlace place;
        place.pos = Pos;
        place.rhset = RHSet;
        TCShop->AddSaleItem(Goods, place);
    }
};
int main()
{
    CTCShop shop; shop.m_goods = 0;
    CAction a; a.TCShop = &shop; a.Goods = 17;
    a.Pos.X = 1; a.Pos.Y = 2; a.Pos.Z = 3;
    a.RHSet.Up.X = 0; a.RHSet.Up.Y = 0; a.RHSet.Up.Z = 1; a.RHSet.Forward.X = 0; a.RHSet.Forward.Y = 1; a.RHSet.Forward.Z = 0;
    a.PerformAction();
    if (shop.m_goods != 17 || shop.m_place.pos.X != 1 || shop.m_place.pos.Y != 2 || shop.m_place.pos.Z != 3
        || shop.m_place.rhset.Up.Z != 1 || shop.m_place.rhset.Forward.Y != 1 || shop.m_place.rhset.Forward.Z != 0) { std::printf("00842d40_TEST FAIL\n"); return 1; }
    std::printf("00842d40_TEST PASS\n");
    return 0;
}