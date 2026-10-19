// Behaviour test for 0x00842C70 CCreatureAction_ShopkeeperGetOut ctor: runs the reconstructed constructor over stubbed
// callees; checks the base arguments (GET_OUT lookup) and the copied TCShop / Goods / Pos / RHSet members.
#include <cstdio>
#include <cstring>
struct CThingCreatureBase { int id; };
struct CTCShop { int id; };
struct CAnimationEntry { int id; };
static int g_strings = 0;
struct CCharString { const char* m_pData; CCharString(const char* t, long = -1) : m_pData(t) { ++g_strings; } ~CCharString() { --g_strings; } };
struct CSoundPair { long FirstSound; long LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };
static CAnimationEntry g_anim; static char g_name[32];
const CAnimationEntry& GetAnimationFromName(CThingCreatureBase*, const CCharString& name) { std::strcpy(g_name, name.m_pData); return g_anim; }
struct CCreatureActionBase {
    CThingCreatureBase* o; float a, b; const CAnimationEntry* an; long c, d;
    virtual void v00() { }
    CCreatureActionBase(CThingCreatureBase* owner, float a_, float b_, const CAnimationEntry& anim, const CSoundPair&, long c_, long d_)
        : o(owner), a(a_), b(b_), an(&anim), c(c_), d(d_) { }
};
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CRightHandedSet { C3DVector Up; C3DVector Forward; CRightHandedSet(const C3DVector& u, const C3DVector& f) : Up(u), Forward(f) { } };
struct CCreatureAction_ShopkeeperGetOut : CCreatureActionBase {
    CTCShop* TCShop; long Goods; C3DVector Pos; CRightHandedSet RHSet;
    CCreatureAction_ShopkeeperGetOut(CThingCreatureBase* owner, CTCShop* shop, const C3DVector& pos, const CRightHandedSet& rhSet, long goods);
};
CCreatureAction_ShopkeeperGetOut::CCreatureAction_ShopkeeperGetOut(CThingCreatureBase* owner, CTCShop* shop, const C3DVector& pos, const CRightHandedSet& rhSet, long goods)
    : CCreatureActionBase(owner, -1.0f, 1.0f, GetAnimationFromName(owner, "GET_OUT"), CSoundPair(), 0, 0)
    , TCShop(shop)
    , Goods(goods)
    , Pos(pos)
    , RHSet(rhSet)
{
}
int main()
{
    CThingCreatureBase owner; CTCShop shop;
    C3DVector p(1, 2, 3); CRightHandedSet rh(C3DVector(4, 5, 6), C3DVector(7, 8, 9));
    CCreatureAction_ShopkeeperGetOut a(&owner, &shop, p, rh, 33);
    bool ok = a.o == &owner && a.a == -1.0f && a.b == 1.0f && a.an == &g_anim && a.c == 0 && a.d == 0 && g_strings == 0 &&
              std::strcmp(g_name, "GET_OUT") == 0 && a.TCShop == &shop && a.Goods == 33 && a.Pos.X == 1 && a.Pos.Z == 3 &&
              a.RHSet.Up.X == 4 && a.RHSet.Up.Z == 6 && a.RHSet.Forward.Y == 8 && a.RHSet.Forward.Z == 9;
    if (!ok) { std::printf("00842c70_TEST FAIL\n"); return 1; }
    std::printf("00842c70_TEST PASS\n");
    return 0;
}
