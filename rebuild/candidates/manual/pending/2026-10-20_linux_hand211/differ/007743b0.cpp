// CTCShop::GetCurrentSellPrice(long itemDefIndex, long count) -- the whole-gold price the shop pays for count items: the basic price scaled by the shop def's sell multiplier, the attitude sell multiplier and a constant, truncated per item
// Leaf: GetCurrentSellPrice
// Grade: DIFFER(264v238) -- (the x87 multiplier chain matches) retail sinks `push ebx` into the found branch (lane never does), so the lane re-colours every register and releases the shop def through eax in the long form
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007743B0, 238 bytes (`ret 8`).
// Label: the catalog says GetCurrentBuyPrice (propagated from the 0x007742C0 twin); this body reads SellPriceMultiplier (+0x5c) and
// the attitude SELL multiplier, so it is GetCurrentSellPrice.
// Callees: CTCBase::GetDefinitionManager @ 0x006869D0, CDefinitionManager::GetDef_0042af3c @ 0x0042AF3C (result ignored),
// CDefClassBase::GetSubDef_005130a0 @ 0x005130A0 (the 'CShopDef' instantiation), CTCShop::GetAttitudeSellPriceMultiplier const
// @ 0x00773790, the float constant @ 0x013833D4, CTCShop::GetCurrentBasicPrice(long, long) @ 0x007727E0 (float), __ftol2 @ 0x00BFEA70; the defs' slot 1.
// Layout: CThing def index (unsigned short) +0x8c; retail CShopDef sits 8 below Ego_r.pdb here (two 12-byte vectors before):
// BuyPriceMultiplier +0x58.
// Shape: the price total doubles as the zero register; the thing def is read once from its out slot into a holder, the shop
// def stays a CDefPointer (re-read once on the found path, so its release there has no null test); the multiplier is a
// named float local (its frame word holds the def's copy across the attitude call).
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
template <class T> struct CDefOut {                     // the out slot (released by the holder below)
    T* Object;
    CDefOut() : Object(0) { }
};
template <class T> struct CDefHolder {                  // the def read once from the out slot, released at scope end
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
struct CShopDef : CDefBase {
    unsigned char _pad08[0x58 - 0x08];
    float BuyPriceMultiplier;                           // +0x58
    float SellPriceMultiplier;                          // +0x5c
};
struct CDefClassBase : CDefBase {
    bool GetSubDef_005130a0(CDefPointer<CShopDef>& out);    // 0x005130A0
};
struct CDefinitionManager {
    bool GetDef_0042af3c(long index, CDefOut<CDefClassBase>& out);   // 0x0042AF3C
};
extern const float g_sellPriceScale_013833d4;           // 0x013833D4
struct CThing {
    unsigned char _pad00[0x8c];
    unsigned short DefIndex;                            // +0x8c
};
struct CTCShop {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    CDefinitionManager* GetDefinitionManager() const;   // 0x006869D0 (CTCBase)
    float GetCurrentBasicPrice(long itemDefIndex, long forSale);   // 0x007727E0
    float GetAttitudeSellPriceMultiplier() const;       // 0x00773790
    long GetCurrentSellPrice(long itemDefIndex, long count);
};

long CTCShop::GetCurrentSellPrice(long itemDefIndex, long count)
{
    long total = 0;
    CDefOut<CDefClassBase> thingDefOut;
    GetDefinitionManager()->GetDef_0042af3c(ParentThing->DefIndex, thingDefOut);
    CDefHolder<CDefClassBase> thingDef(thingDefOut.Object);
    CDefPointer<CShopDef> shopDef;
    if (!thingDef->GetSubDef_005130a0(shopDef))
        return 0;
    float multiplier = shopDef->SellPriceMultiplier;
    multiplier = multiplier * GetAttitudeSellPriceMultiplier();
    multiplier = g_sellPriceScale_013833d4 * multiplier;
    for (long i = count; i > 0; --i)
        total += (long)(GetCurrentBasicPrice(itemDefIndex, 0) * multiplier);
    return total;
}
