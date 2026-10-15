// Behaviour test for 0x0084C170 CCreatureAction_ShopkeeperGetOut copy constructor.
#include <cstdio>
struct CTCShop { int id; };
struct CCreatureActionBase { int m_base; CCreatureActionBase() : m_base(5) { } CCreatureActionBase(const CCreatureActionBase& o) : m_base(o.m_base) { } };
struct CAction : CCreatureActionBase {
    CTCShop* TCShop; int Goods;
    float PosX, PosY, PosZ, UpX, UpY, UpZ, ForwardX, ForwardY, ForwardZ;
    CAction() { }
    CAction(const CAction& other)
        : CCreatureActionBase(other), TCShop(other.TCShop), Goods(other.Goods),
          PosX(other.PosX), PosY(other.PosY), PosZ(other.PosZ), UpX(other.UpX), UpY(other.UpY), UpZ(other.UpZ),
          ForwardX(other.ForwardX), ForwardY(other.ForwardY), ForwardZ(other.ForwardZ) { }
};
int main()
{
    CTCShop shop; CAction a; a.TCShop = &shop; a.Goods = 3;
    a.PosX = 1; a.PosY = 2; a.PosZ = 3; a.UpX = 4; a.UpY = 5; a.UpZ = 6; a.ForwardX = 7; a.ForwardY = 8; a.ForwardZ = 9;
    CAction b(a);
    if (b.m_base != 5 || b.TCShop != &shop || b.Goods != 3 || b.PosX != 1 || b.PosZ != 3 || b.UpY != 5 || b.ForwardX != 7 || b.ForwardZ != 9) { std::printf("0084c170_TEST FAIL\n"); return 1; }
    std::printf("0084c170_TEST PASS\n");
    return 0;
}
