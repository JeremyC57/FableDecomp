#include <cstdio>
struct CBase { int baseCtor; CBase() : baseCtor(1) { } };
struct CGroup : CBase { bool DroppingGoods; long PickUpGoodsActionID; CGroup() : CBase(), PickUpGoodsActionID(-1) { DroppingGoods = false; } };
int main()
{
    CGroup g; bool ok = g.baseCtor == 1 && !g.DroppingGoods && g.PickUpGoodsActionID == -1;
    if (!ok) { std::printf("008fda5d_TEST FAIL\n"); return 1; }
    std::printf("008fda5d_TEST PASS\n"); return 0;
}