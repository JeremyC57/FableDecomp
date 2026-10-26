// Behaviour test for 007743b0: src/007743b0.cpp copied verbatim; GetCurrentSellPrice returns 0 without the shop sub-def, else the sum
// over count items of (long)(basic price * the def's SellPriceMultiplier * the attitude multiplier * the global sell scale); both
// defs are released.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCShop::GetCurrentSellPrice(long itemDefIndex, long count) -- the whole-gold price the shop pays for count items: the basic price scaled by the shop def's sell multiplier, the attitude sell multiplier and a constant, truncated per item
// Leaf: GetCurrentSellPrice
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007743B0, 238 bytes (`ret 8`).
// Label: the catalog says GetCurrentBuyPrice (propagated from the 0x007742C0 twin); this body reads SellPriceMultiplier (+0x5c) and
// the attitude SELL multiplier, so it is GetCurrentSellPrice.
// Callees: CTCBase::GetDefinitionManager @ 0x006869D0, CDefinitionManager::GetDef_0042af3c @ 0x0042AF3C (result ignored),
// CDefClassBase::GetSubDef_005130a0 @ 0x005130A0 (the 'CShopDef' instantiation), CTCShop::GetAttitudeSellPriceMultiplier const
// @ 0x00773790, the float constant @ 0x013833D4, CTCShop::GetCurrentBasicPrice(long, long) @ 0x007727E0 (float), __ftol2 @ 0x00BFEA70; the defs' slot 1.
// Layout: CThing def index (unsigned short) +0x8c; retail CShopDef sits 8 below Ego_r.pdb here (two 12-byte vectors before):
// BuyPriceMultiplier +0x58.
// Shape: both defs are dtor-carrying CDefPointer locals; GetDef and GetSubDef are same-TU noinline stand-in bodies that write
// only the out slot, so the thing def is read once into ebp and the shop def is re-read once on the found path (its release
// there has no null test); the price total is declared just before the loop, so its zero is the out slots' zero register
// (edi) and `push ebx` sinks into the found branch; the multiplier is a named float local.
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
class CShopDef : public CDefBase { public:
    unsigned char _pad08[0x58 - 0x08];
    float BuyPriceMultiplier;                           // +0x58
    float SellPriceMultiplier;                          // +0x5c
};
class CDefClassBase : public CDefBase { public:
    bool GetSubDef_005130a0(CDefPointer<CShopDef>& out);    // 0x005130A0
};
void* Find_CDefClassBase_GetSubDef(const void* owner);   // stand-in callee of the stand-in body below
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef_005130a0(CDefPointer<CShopDef>& out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out.Object = (CShopDef*)def;
    return true;
}
#pragma auto_inline(on)
class CDefinitionManager { public:
    bool GetDef_0042af3c(long index, CDefPointer<CDefClassBase>& out);   // 0x0042AF3C
};
void* Lookup_CDefinitionManager_GetDef(const void* manager, long index);   // stand-in callee
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef_0042af3c(long index, CDefPointer<CDefClassBase>& out)
{
    void* def = Lookup_CDefinitionManager_GetDef(this, index);
    if (!def)
        return false;
    out.Object = (CDefClassBase*)def;
    return true;
}
#pragma auto_inline(on)
extern const float g_sellPriceScale_013833d4;           // 0x013833D4
class CThing { public:
    unsigned char _pad00[0x8c];
    unsigned short DefIndex;                            // +0x8c
};
class CTCShop { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    CDefinitionManager* GetDefinitionManager() const;   // 0x006869D0 (CTCBase)
    float GetCurrentBasicPrice(long itemDefIndex, long forSale);   // 0x007727E0
    float GetAttitudeSellPriceMultiplier() const;       // 0x00773790
    long GetCurrentSellPrice(long itemDefIndex, long count);
};

long CTCShop::GetCurrentSellPrice(long itemDefIndex, long count)
{
    CDefPointer<CDefClassBase> thingDef;
    GetDefinitionManager()->GetDef_0042af3c(ParentThing->DefIndex, thingDef);
    CDefPointer<CShopDef> shopDef;
    if (!thingDef->GetSubDef_005130a0(shopDef))
        return 0;
    float multiplier = shopDef->SellPriceMultiplier;
    multiplier = multiplier * GetAttitudeSellPriceMultiplier();
    multiplier = g_sellPriceScale_013833d4 * multiplier;
    long total = 0;
    for (long i = count; i > 0; --i)
        total += (long)(GetCurrentBasicPrice(itemDefIndex, 0) * multiplier);
    return total;
}
// ---- harness ----
extern const float g_sellPriceScale_013833d4 = 0.5f;
static int g_destroyed = 0, g_basicCalls = 0; static long g_index = -1, g_item = -1, g_forSale = -1;
static void* g_thingDef = 0; static void* g_shopDef = 0; static const void* g_subOwner = 0;
static CDefinitionManager g_manager; static float g_attitude = 1.0f; static float g_basic = 10.0f;
void CDefBase::v00() { }
void CDefBase::Destroy() { ++g_destroyed; }
void CTCShop::v00() { }
CDefinitionManager* CTCShop::GetDefinitionManager() const { return &g_manager; }
float CTCShop::GetAttitudeSellPriceMultiplier() const { return g_attitude; }
float CTCShop::GetCurrentBasicPrice(long itemDefIndex, long forSale) { ++g_basicCalls; g_item = itemDefIndex; g_forSale = forSale; return g_basic; }
void* Lookup_CDefinitionManager_GetDef(const void* manager, long index)
{ CHECK(manager == &g_manager); g_index = index; if (g_thingDef) ++((CDefBase*)g_thingDef)->RefCount; return g_thingDef; }
void* Find_CDefClassBase_GetSubDef(const void* owner)
{ g_subOwner = owner; if (g_shopDef) ++((CDefBase*)g_shopDef)->RefCount; return g_shopDef; }
int main()
{
    static CDefClassBase thingDef; thingDef.RefCount = 1;
    static CShopDef shopDef; shopDef.RefCount = 1; shopDef.SellPriceMultiplier = 1.5f;
    static unsigned char thingBuf[sizeof(CThing)]; CThing& thing = *(CThing*)thingBuf; thing.DefIndex = 0x4321;
    static unsigned char shopBuf[sizeof(CTCShop)]; CTCShop& shop = *(CTCShop*)shopBuf; shop.ParentThing = &thing;
    g_thingDef = &thingDef; g_shopDef = &shopDef; g_attitude = 2.0f; g_basic = 10.25f;

    long price = shop.GetCurrentSellPrice(77, 3);
    // per item: (long)(10.25 * multiplier) with multiplier = 1.5 * 2.0 * 0.5
    CHECK(price == 3 * 15 && g_basicCalls == 3 && g_item == 77 && g_forSale == 0);
    CHECK(g_index == 0x4321 && g_subOwner == &thingDef && thingDef.RefCount == 1 && shopDef.RefCount == 1 && g_destroyed == 0);

    // no items
    g_basicCalls = 0;
    CHECK(shop.GetCurrentSellPrice(77, 0) == 0 && g_basicCalls == 0);

    // no shop sub-def
    g_shopDef = 0; g_basicCalls = 0;
    CHECK(shop.GetCurrentSellPrice(77, 2) == 0 && g_basicCalls == 0 && thingDef.RefCount == 1 && g_destroyed == 0);

    // last references: both destroyed
    g_shopDef = &shopDef; thingDef.RefCount = 0; shopDef.RefCount = 0;
    CHECK(shop.GetCurrentSellPrice(77, 1) == 15 && g_destroyed == 2);

    if (fails == 0) printf("007743b0_TEST PASS\n");
    return fails;
}