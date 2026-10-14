#include <cstdio>
struct CDefBase { virtual void v00() {} unsigned char _body[0x24]; CDefBase() {} };
struct CShopItem { float Quantity; float PriceMultiplier; float MaxStock; long RestockPeriod; float RestockPeriodRandomness; float SalesPerDay; float SalesPerDayRandomness;
    CShopItem() : Quantity(0.0f), PriceMultiplier(1.0f), MaxStock(0.0f), RestockPeriod(1), RestockPeriodRandomness(0.0f), SalesPerDay(0.0f), SalesPerDayRandomness(0.0f) {} };
struct CShopItemDef : CDefBase { long Type; CShopItem Item; virtual void v00() {} CShopItemDef(); };
CShopItemDef::CShopItemDef() : CDefBase(), Item() { Type = 0; }
int main()
{
    CShopItemDef d;
    if (d.Type != 0 || d.Item.PriceMultiplier != 1.0f || d.Item.RestockPeriod != 1 || d.Item.Quantity != 0.0f) { std::printf("004d405a_TEST FAIL\n"); return 1; }
    std::printf("004d405a_TEST PASS\n");
    return 0;
}
