#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __cdecl
#endif
class CDefClassBase { public: int m_unused; };
static int g_baseCopies = 0;
struct CDefBase { unsigned long m_words[10]; CDefBase& operator=(const CDefBase& o) { ++g_baseCopies; for (int i = 0; i < 10; ++i) m_words[i] = o.m_words[i]; return *this; } };
class CShopItem { public: float Quantity, PriceMultiplier, MaxStock; long RestockPeriod; float RestockPeriodRandomness, SalesPerDay, SalesPerDayRandomness; };
class CShopItemDef { public:                           // the header's flattened layout
    CDefBase base;                              // +0x00..+0x27 in retail
    long  Type;
    float Item_Quantity, Item_PriceMultiplier, Item_MaxStock;
    long  Item_RestockPeriod;
    float Item_RestockPeriodRandomness, Item_SalesPerDay, Item_SalesPerDayRandomness;
};
struct CShopItemDef_Methods : CShopItemDef {
    CShopItem&       Item()       { return *reinterpret_cast<CShopItem*>(&Item_Quantity); }
    const CShopItem& Item() const { return *reinterpret_cast<const CShopItem*>(&Item_Quantity); }
    void Copy(const CDefClassBase* source)
    {
        const CShopItemDef_Methods* other = reinterpret_cast<const CShopItemDef_Methods*>(source);
        base = other->base;
        Type = other->Type;
        Item() = other->Item();
    }
};
int main()
{
    CShopItemDef_Methods a, b;
    for (int i = 0; i < 10; ++i) { a.base.m_words[i] = i; b.base.m_words[i] = 0; }
    a.Type = 7; a.Item_Quantity = 1.5f; a.Item_PriceMultiplier = 2.0f; a.Item_MaxStock = 30.0f; a.Item_RestockPeriod = 12;
    a.Item_RestockPeriodRandomness = 0.25f; a.Item_SalesPerDay = 4.0f; a.Item_SalesPerDayRandomness = 0.5f;
    b.Copy(reinterpret_cast<const CDefClassBase*>(&a));
    if (g_baseCopies != 1 || b.base.m_words[9] != 9 || b.Type != 7 || b.Item_Quantity != 1.5f || b.Item_MaxStock != 30.0f || b.Item_RestockPeriod != 12 || b.Item_SalesPerDayRandomness != 0.5f) { std::printf("004e069f_TEST FAIL\n"); return 1; }
    std::printf("004e069f_TEST PASS\n");
    return 0;
}