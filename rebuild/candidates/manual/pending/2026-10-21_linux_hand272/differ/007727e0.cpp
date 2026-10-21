// CTCShop::GetCurrentBasicPrice(long itemIndex, long extraStock) -- 0 when the shop's own def has no shop sub def; otherwise the item def's base value, discounted by the shop's stock level of that item (stock rounded to an int plus the extra stock, times the def's per-item discount, capped by the def's maximum discount) and scaled by the default stock item's price multiplier
// Grade: DIFFER(497v433) -- minimal attempt; retail rounds the stock level with an inline-assembly fistp sequence (fadd 0.5 ; fistp ; fild ; fsub 1.0 ; compare ; dec) that compiled C++ never produces, so this row needs an oracle-lane decision before more work; the rest (defs, stock map find, discount cap, default-stock multiplier) is modelled
// Retail: Fable.exe @ 0x007727E0, 448 bytes (frame-pointer, `ret 8`).
// Callees: CTCBase::GetDefinitions 0x006869D0, CDefinitionManager::GetDef<T> 0x0042AF3C, CDefClassBase::GetSubDef<T> 0x005130A0 / 0x0059350C,
//   CDefinitionManager::GetPDefFromGlobalIndex 0x009AD9E0, stock map find 0x00777170, CTCShop::FindDefaultStockItem 0x00772250.
// Note: retail rounds the stock level with an inline `fistp` sequence (round-to-nearest, minus one when the value was exactly .5 below),
//   which VC7.1 never emits for C++ code; this source uses the compiler's conversion, so the row cannot byte-match without an
//   inline-assembly oracle.
struct CRefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
};
struct CShopDef : CRefBase {
    unsigned char _pad08[0x8c - 0x08];
    float DiscountPerItem;                              // +0x8c
    float MaxDiscount;                                  // +0x90
};
struct CItemValueDef : CRefBase {
    unsigned char _pad08[0x28 - 0x08];
    long Value;                                         // +0x28
};
struct CDefClassBase : CRefBase {
    bool GetShopDef(CDefPointer<CShopDef>* def);        // 0x005130A0
    bool GetValueDef(CDefPointer<CItemValueDef>* def);  // 0x0059350C
};
struct CDefinitionManager {
    bool GetDef(long index, CDefPointer<CDefClassBase>* def);   // 0x0042AF3C
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index);   // 0x009AD9E0
};
struct CStockNode { unsigned char _pad[0x14]; float Stock; };
struct CStockMap {
    CStockNode* Head;
    CStockNode** Find(CStockNode** result, const long& key);   // 0x00777170
};
struct CShopItem { long _pad; float PriceMultiplier; };
class CThing { public: unsigned char _pad[0x8c]; unsigned short DefIndex; };
struct CTCShop {
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[0x34 - 0x08];
    CStockMap Stock;                                    // +0x34
    CDefinitionManager* GetDefinitions();               // 0x006869D0
    CShopItem* FindDefaultStockItem(long itemIndex);    // 0x00772250
    float GetCurrentBasicPrice(long itemIndex, long extraStock);
};

float CTCShop::GetCurrentBasicPrice(long itemIndex, long extraStock)
{
    CDefPointer<CDefClassBase> thingDef;
    GetDefinitions()->GetDef(ParentThing->DefIndex, &thingDef);
    CDefPointer<CShopDef> shopDef;
    if (!thingDef.Object->GetShopDef(&shopDef))
        return 0.0f;
    CDefPointer<CDefClassBase> itemDef = GetDefinitions()->GetPDefFromGlobalIndex(itemIndex);
    CDefPointer<CItemValueDef> valueDef;
    float price;
    if (itemDef.Object->GetValueDef(&valueDef))
        price = (float)valueDef.Object->Value;
    else
        price = 1.0f;
    CStockNode* node;
    Stock.Find(&node, itemIndex);
    if (node != Stock.Head) {
        float stock = node->Stock;
        long count = (long)(stock + 0.5f);
        if (stock == (float)count - 1.0f)
            --count;
        float discount = (float)(count + extraStock) * shopDef.Object->DiscountPerItem;
        const float* cap = &shopDef.Object->MaxDiscount;
        if (!(*cap < discount))
            cap = &discount;
        price *= 1.0f - *cap;
    }
    CShopItem* item = FindDefaultStockItem(itemIndex);
    if (item)
        price *= item->PriceMultiplier;
    return price;
}
