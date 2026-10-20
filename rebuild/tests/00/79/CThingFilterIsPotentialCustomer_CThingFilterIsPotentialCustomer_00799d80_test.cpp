// behaviour test for CThingFilter_IsPotentialCustomer::CThingFilter_IsPotentialCustomer (0x00799D80): stored arguments, shop position, building-volume shopper container
#include <cstdio>
#include <cstring>
struct C3DVector { float X, Y, Z; };
struct CCharString { const char* m_text; CCharString(const char* t, long = -1) : m_text(t) {} ~CCharString() {} };
static bool g_hasVolume = false; static int g_meshDtors = 0; static const char* g_askedName = 0;
struct CPhysicsMeshObject { ~CPhysicsMeshObject() { ++g_meshDtors; }
    bool HasContainmentVolume(const CCharString& n) { g_askedName = n.m_text; return g_hasVolume; } };
struct CTCPhysicsBase { void* m_mesh; void* GetPhysicsMesh() const { return m_mesh; } CPhysicsMeshObject GetPhysicsMeshObject() { return CPhysicsMeshObject(); } };
class CThing { public: CTCPhysicsBase* PhysicsTC; C3DVector m_pos; const C3DVector& GetPosition() const { return m_pos; } };
struct CTCShop { CThing* ParentThing; bool StallLike; };
struct CTCShopKeeper { CTCShop* m_shop; CTCShop* GetShopTC() { return m_shop; } };
struct CThingFilter_IsPotentialCustomer {
    CThing* ShopperContainer; C3DVector Pos; CTCShopKeeper* TCShopKeeper; bool AllowExistingShopper;
    CThingFilter_IsPotentialCustomer(CTCShopKeeper* shopKeeper, bool allowExistingShopper) : TCShopKeeper(shopKeeper), AllowExistingShopper(allowExistingShopper)
    {
        CTCShop* shop = TCShopKeeper->GetShopTC();
        CTCPhysicsBase* physics = shop->ParentThing->PhysicsTC;
        ShopperContainer = 0;
        Pos = shop->ParentThing->GetPosition();
        if (!shop->StallLike && physics && physics->GetPhysicsMesh()) {
            bool insideBuilding;
            { CPhysicsMeshObject mesh = physics->GetPhysicsMeshObject(); insideBuilding = mesh.HasContainmentVolume("BUILDING_BOUNDRY"); if (insideBuilding) ShopperContainer = shop->ParentThing; }
        }
    } };
int main()
{
    int dummy = 0;
    CTCPhysicsBase physics; physics.m_mesh = &dummy;
    CThing shopThing; shopThing.PhysicsTC = &physics; shopThing.m_pos.X = 1; shopThing.m_pos.Y = 2; shopThing.m_pos.Z = 3;
    CTCShop shop; shop.ParentThing = &shopThing; shop.StallLike = false;
    CTCShopKeeper keeper; keeper.m_shop = &shop;
    int bad = 0;
    CThingFilter_IsPotentialCustomer a(&keeper, true);           // no boundary volume
    if (a.ShopperContainer != 0 || a.TCShopKeeper != &keeper || !a.AllowExistingShopper || a.Pos.Y != 2.0f) bad = 1;
    if (!g_askedName || std::strcmp(g_askedName, "BUILDING_BOUNDRY") != 0) bad = 1;
    g_hasVolume = true;
    CThingFilter_IsPotentialCustomer b(&keeper, false);
    if (b.ShopperContainer != &shopThing || b.AllowExistingShopper || b.Pos.Z != 3.0f) bad = 1;
    shop.StallLike = true; g_askedName = 0;
    CThingFilter_IsPotentialCustomer c(&keeper, false);          // stall: no mesh query
    if (c.ShopperContainer != 0 || g_askedName) bad = 1;
    shop.StallLike = false; physics.m_mesh = 0;
    CThingFilter_IsPotentialCustomer d(&keeper, false);          // no physics mesh
    if (d.ShopperContainer != 0 || g_askedName) bad = 1;
    if (bad) { std::printf("00799d80_TEST FAIL\n"); return 1; }
    std::printf("00799d80_TEST PASS\n");
    return 0;
}