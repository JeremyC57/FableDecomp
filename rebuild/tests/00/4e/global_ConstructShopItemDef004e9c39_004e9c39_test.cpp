#include <cstdio>
#include <new>
static int g_baseCopies = 0;
struct CDefClassBase {
    unsigned char _body[0x21];
    CDefClassBase() { for (int i = 0; i < 0x21; ++i) _body[i] = (unsigned char)i; }
    CDefClassBase(const CDefClassBase& other) { for (int i = 0; i < 0x21; ++i) _body[i] = other._body[i]; ++g_baseCopies; }
    virtual ~CDefClassBase() { }
    virtual int Kind() const { return 1; }
};
struct CShopItem {
    float Quantity; float PriceMultiplier; float MaxStock; long RestockPeriod;
    float RestockPeriodRandomness; float SalesPerDay; float SalesPerDayRandomness;
};
struct CShopItemDef : CDefClassBase {
    long Type;
    CShopItem Item;
    virtual int Kind() const { return 9; }
    CShopItemDef() { }
    CShopItemDef(const CShopItemDef& o) : CDefClassBase(o), Type(o.Type), Item(o.Item) { }
};
void Construct_ShopItemDef_004e9c39(CShopItemDef* p, const CShopItemDef& value)
{
    if (p) new (p) CShopItemDef(value);   // VC null-tests the placement result itself
}
int main()
{
    CShopItemDef src;
    src.Type = 3;
    src.Item.Quantity = 2.0f; src.Item.PriceMultiplier = 1.5f; src.Item.MaxStock = 10.0f; src.Item.RestockPeriod = 600;
    src.Item.RestockPeriodRandomness = 0.25f; src.Item.SalesPerDay = 4.0f; src.Item.SalesPerDayRandomness = 0.5f;
    static double storage[16];
    CShopItemDef* dst = (CShopItemDef*)(void*)storage;
    Construct_ShopItemDef_004e9c39(dst, src);
    if (g_baseCopies != 1 || dst->Kind() != 9 || dst->Type != 3 || dst->Item.PriceMultiplier != 1.5f || dst->Item.RestockPeriod != 600
        || dst->Item.SalesPerDayRandomness != 0.5f) { std::printf("004e9c39_TEST FAIL\n"); return 1; }
    Construct_ShopItemDef_004e9c39(0, src);
    if (g_baseCopies != 1) { std::printf("004e9c39_TEST FAIL\n"); return 1; }
    std::printf("004e9c39_TEST PASS\n");
    return 0;
}