// Behaviour test for 007721a0: src/007721a0.cpp copied verbatim; IsPermanentlyStocked looks up the shop thing's def by
// its def index and returns its CShopDef sub-def's IsStockDisplayPermanent || AlwaysOpen (false without the sub-def);
// both defs are released.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCShop::IsPermanentlyStocked() const -- true when the shop thing's CShopDef sub-def says the stock display is permanent or the shop is always open
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007721A0, 170 bytes (plain `ret`).
// Callees: CTCBase::GetDefinitionManager @ 0x006869D0, CDefinitionManager::GetDef_0042af3c(long, CDefPointer&) @ 0x0042AF3C
// (the thing-def instantiation; result ignored), CDefClassBase::GetSubDef_005130a0(CDefPointer&) @ 0x005130A0 (the 'CShopDef'
// instantiation); the defs' virtual slot 1 (destroy on the last release). Names follow the landed bodies of both callees.
// Layout: CThing def index (unsigned short) +0x8c; retail CShopDef sits 0x10 below Ego_r.pdb (two vectors and two maps of 12
// bytes before): IsStockDisplayPermanent +0x98, AlwaysOpen +0x9c.
// Shape: two CDefPointer locals whose destructors release them (the out slots are zeroed after the argument pushes); GetDef
// and GetSubDef are same-TU noinline stand-in bodies that write only the out slot, so each def is read once into a register;
// the bool rides in bl across both releases; on the found path the shop def is already dereferenced, so its release loses
// the null test.
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> struct CDefPointer {                 // a counted def reference (released at scope end)
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
struct CShopDef : CDefBase {
    unsigned char _pad08[0x98 - 0x08];
    bool IsStockDisplayPermanent;                       // +0x98
    bool IsBarberShop;                                  // +0x99
    bool IsTattooShop;                                  // +0x9a
    bool IsTitleShop;                                   // +0x9b
    bool AlwaysOpen;                                    // +0x9c
};
struct CDefClassBase : CDefBase {
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
struct CDefinitionManager {
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
struct CThing {
    unsigned char _pad00[0x8c];
    unsigned short DefIndex;                            // +0x8c
};
struct CTCShop {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    CDefinitionManager* GetDefinitionManager() const;   // 0x006869D0 (CTCBase)
    bool IsPermanentlyStocked() const;
};

bool CTCShop::IsPermanentlyStocked() const
{
    CDefPointer<CDefClassBase> thingDef;
    GetDefinitionManager()->GetDef_0042af3c(ParentThing->DefIndex, thingDef);
    CDefPointer<CShopDef> shopDef;
    if (thingDef->GetSubDef_005130a0(shopDef))
        return shopDef->IsStockDisplayPermanent || shopDef->AlwaysOpen;
    return false;
}
// ---- harness ----
static int g_destroyed = 0; static long g_index = -1;
static void* g_thingDef = 0; static void* g_shopDef = 0; static const void* g_subOwner = 0;
static CDefinitionManager g_manager;
void CDefBase::v00() { }
void CDefBase::Destroy() { ++g_destroyed; }
void CTCShop::v00() { }
CDefinitionManager* CTCShop::GetDefinitionManager() const { return &g_manager; }
void* Lookup_CDefinitionManager_GetDef(const void* manager, long index)
{ CHECK(manager == &g_manager); g_index = index; if (g_thingDef) ++((CDefBase*)g_thingDef)->RefCount; return g_thingDef; }
void* Find_CDefClassBase_GetSubDef(const void* owner)
{ g_subOwner = owner; if (g_shopDef) ++((CDefBase*)g_shopDef)->RefCount; return g_shopDef; }
int main()
{
    static CDefClassBase thingDef; thingDef.RefCount = 1;
    static CShopDef shopDef; shopDef.RefCount = 1;
    static unsigned char thingBuf[sizeof(CThing)]; CThing& thing = *(CThing*)thingBuf; thing.DefIndex = 0x1234;
    static unsigned char shopBuf[sizeof(CTCShop)]; CTCShop& shop = *(CTCShop*)shopBuf; shop.ParentThing = &thing;
    g_thingDef = &thingDef; g_shopDef = &shopDef;

    shopDef.IsStockDisplayPermanent = true; shopDef.AlwaysOpen = false;
    CHECK(shop.IsPermanentlyStocked() && g_index == 0x1234 && g_subOwner == &thingDef);
    shopDef.IsStockDisplayPermanent = false; shopDef.AlwaysOpen = true;
    CHECK(shop.IsPermanentlyStocked());
    shopDef.AlwaysOpen = false;
    CHECK(!shop.IsPermanentlyStocked());
    CHECK(thingDef.RefCount == 1 && shopDef.RefCount == 1 && g_destroyed == 0);

    // no shop sub-def
    g_shopDef = 0;
    CHECK(!shop.IsPermanentlyStocked() && thingDef.RefCount == 1 && g_destroyed == 0);

    // last references: both destroyed
    g_shopDef = &shopDef; thingDef.RefCount = 0; shopDef.RefCount = 0; shopDef.AlwaysOpen = true;
    CHECK(shop.IsPermanentlyStocked() && g_destroyed == 2);

    if (fails == 0) printf("007721a0_TEST PASS\n");
    return fails;
}