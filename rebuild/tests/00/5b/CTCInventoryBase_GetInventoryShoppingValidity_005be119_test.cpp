#include <cstdio>
#include <cmath>
// Behaviour test for CTCInventoryBase::GetInventoryShoppingValidity (0x005BE119): same body as src/, with the alive
// tests, interface lookups, player chain and the facing component stubbed and recorded.
using std::atan2; using std::fmod;
class C3DVector { public: float X, Y, Z; };
class C2DVector { public: float X, Y; C2DVector(float x, float y) : X(x), Y(y) { } };
class CTCShop; struct CTCFacing;
class CThing {
public:
    bool alive; C3DVector pos; void* shop; void* facing;
    const C3DVector& GetPosition() const { return pos; }
    template<class T> bool GetInterface(T** ppOut, long type)
    {
        void* p = type == 0x53 ? shop : type == 2 ? facing : 0;
        if (!p) return false;
        *ppOut = (T*)p; return true;
    }
};
static int g_finished = 0;
class CThingCreatureBase : public CThing {
public:
    bool freeToFinish;
    bool IsFreeToFinishAction() { return freeToFinish; }
    void FinishCurrentAction() { ++g_finished; }
};
class CThingHero : public CThing { };
bool IS_THING_ALIVE(CThing* const& t) { return t && t->alive; }
bool IS_THING_ALIVE(CThingCreatureBase* const& t) { return t && t->alive; }
bool IS_THING_ALIVE(CThingHero* const& t) { return t && t->alive; }
struct CBaseIntelligentPointer {
    CThingCreatureBase* item;
    CThingCreatureBase* GetPItem() const { return item; }
};
class CTCShop { public: CBaseIntelligentPointer PKeeper; };
static float g_angle = -5.0f; static int g_sets = 0;
struct CTCFacing {
    float X, Y;
    void SetFacingAngle(float a) { g_angle = a; ++g_sets; }
};
static CThingHero* g_hero = 0;
class CPlayer { public: CThingHero* GetHeroThing() { return g_hero; } };
static CPlayer g_player;
class CPlayerManager { public: CPlayer* GetMainPlayer() { return &g_player; } };
static CPlayerManager g_pm;
struct CTCInventoryBase {
    CThing* ParentThing;
    CThing* PShopKeeper;
    CPlayerManager* GetPlayerManager() const { return &g_pm; }
    bool GetInventoryShoppingValidity();
};

bool CTCInventoryBase::GetInventoryShoppingValidity()
{
    bool valid = IS_THING_ALIVE(PShopKeeper);
    if (valid && IS_THING_ALIVE(PShopKeeper)) {
        CThing* keeper = PShopKeeper;
        CTCShop* shop;
        if (keeper->GetInterface(&shop, 0x53)) {
            CThingCreatureBase* creature = shop->PKeeper.GetPItem();
            if (IS_THING_ALIVE(creature)) {
                if (creature->IsFreeToFinishAction())
                    creature->FinishCurrentAction();
                CThingHero* hero = GetPlayerManager()->GetMainPlayer()->GetHeroThing();
                if (IS_THING_ALIVE(hero)) {
                    CTCFacing* facing;
                    creature->GetInterface(&facing, 2);
                    const C3DVector& target = hero->GetPosition();
                    const C2DVector& difference = C2DVector(target.X - facing->X, target.Y - facing->Y);
                    float angle = atan2(difference.X, difference.Y) * 0.15915494f;
                    if (angle < 0.0f || angle >= 1.0f) {
                        angle = (float)fmod((double)angle, 1.0);
                        if (angle < 0.0f)
                            angle += 1.0f;
                    }
                    facing->SetFacingAngle(angle);
                }
            }
        }
    }
    return valid;
}

int main()
{
    int fails = 0;
    CTCFacing facing = { 1.0f, 1.0f };
    CThingCreatureBase creature; creature.alive = true; creature.freeToFinish = true; creature.facing = &facing; creature.shop = 0;
    CTCShop shop = { { &creature } };
    CThing keeper; keeper.alive = true; keeper.shop = &shop; keeper.facing = 0;
    CThingHero hero; hero.alive = true; hero.pos.X = 0.0f; hero.pos.Y = 1.0f; hero.pos.Z = 0.0f;   // due -X: atan2(-1, 0) = -pi/2
    g_hero = &hero;
    CTCInventoryBase inv; inv.ParentThing = 0; inv.PShopKeeper = &keeper;
    if (!inv.GetInventoryShoppingValidity()) ++fails;
    if (g_finished != 1 || g_sets != 1 || std::fabs(g_angle - 0.75f) > 1e-4f) ++fails;       // -0.25 wraps to 0.75
    hero.pos.X = 2.0f; hero.pos.Y = 1.0f;                                                       // due +X: 0.25
    creature.freeToFinish = false;
    if (!inv.GetInventoryShoppingValidity() || g_finished != 1 || g_sets != 2 || std::fabs(g_angle - 0.25f) > 1e-4f) ++fails;
    hero.alive = false;                                                                         // dead hero: no turn
    if (!inv.GetInventoryShoppingValidity() || g_sets != 2) ++fails;
    keeper.alive = false;                                                                       // dead keeper: invalid
    if (inv.GetInventoryShoppingValidity()) ++fails;
    keeper.alive = true; keeper.shop = 0;                                                       // no shop: valid, nothing else
    if (!inv.GetInventoryShoppingValidity() || g_sets != 2) ++fails;
    if (fails) { std::printf("005be119_TEST FAIL %d\n", fails); return 1; }
    std::printf("005be119_TEST PASS\n");
    return 0;
}