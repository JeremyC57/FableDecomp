// Behaviour test for CTCShop::IsShopKeeperNearerThan @ 0x007723F0: keeper alive && hero alive && XY dist^2 < d^2.
#include <cstdio>
class C3DVector { public: float X, Y, Z; };
class C2DVector { public: float X, Y; C2DVector(float x, float y) : X(x), Y(y) { } float GetSquaredLength() const { return X * X + Y * Y; } };
class CThing { public: bool Dead; C3DVector Pos; const C3DVector& GetPos() const { return Pos; } };
struct CIntelligentPointer { CThing* Item; CThing* GetPItem() const { return Item; } };
static bool IS_THING_ALIVE(const CIntelligentPointer& p) { return p.GetPItem() != 0 && !p.GetPItem()->Dead; }
static bool IS_THING_ALIVE(const CThing* const& p) { return p != 0 && !p->Dead; }
static bool IsWithin2DDistance(const CThing* a, const CThing* b, float distance)
{
    const C3DVector& pa = a->GetPos();
    const C3DVector& pb = b->GetPos();
    return C2DVector(pb.X - pa.X, pb.Y - pa.Y).GetSquaredLength() < distance * distance;
}
static const CThing* g_hero;
class CTCShop { public:
    CIntelligentPointer PShopKeeper;
    bool IsShopKeeperNearerThan(float distance) const
    {
        const CThing* hero = g_hero;
        return IS_THING_ALIVE(PShopKeeper) && IS_THING_ALIVE(hero) && IsWithin2DDistance(hero, PShopKeeper.GetPItem(), distance);
    }
};
int main()
{
    CThing hero = { false, { 1.0f, 2.0f, 50.0f } };
    CThing keeper = { false, { 4.0f, 6.0f, -50.0f } };      // XY distance 5, Z ignored
    CTCShop shop; shop.PShopKeeper.Item = &keeper; g_hero = &hero;
    bool ok = shop.IsShopKeeperNearerThan(5.5f) && !shop.IsShopKeeperNearerThan(5.0f) && !shop.IsShopKeeperNearerThan(4.0f);
    keeper.Dead = true; ok = ok && !shop.IsShopKeeperNearerThan(100.0f); keeper.Dead = false;
    hero.Dead = true; ok = ok && !shop.IsShopKeeperNearerThan(100.0f); hero.Dead = false;
    g_hero = 0; ok = ok && !shop.IsShopKeeperNearerThan(100.0f); g_hero = &hero;
    shop.PShopKeeper.Item = 0; ok = ok && !shop.IsShopKeeperNearerThan(100.0f);
    if (!ok) { std::printf("007723f0_TEST FAIL\n"); return 1; }
    std::printf("007723f0_TEST PASS\n");
    return 0;
}