// Behaviour test for CTCShop::GetAttitudeBuyPriceMultiplier (0x007735D0): a copy of the function's logic over a small
// model of the things it reads (the parent thing's house component and def, the shop def's attitude table, the keeper's
// opinion component, the hero's trade skill), checked against hand-computed prices factors.
#include <cstdio>
#include <map>
#include <cmath>

enum EOpinionAttitudeType { ATTITUDE_NEUTRAL = 0, ATTITUDE_LIKE = 3, ATTITUDE_HATE = 5 };

static int g_liveDefs = 0;

struct CDef {                                   // counted def pointee (CDefPointer release: --count == 0 deletes)
    long RefCount;
    CDef() : RefCount(0) { ++g_liveDefs; }
    virtual ~CDef() { --g_liveDefs; }
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) delete this; }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->Release(); }
    void Set(T* object) { if (Object) Object->Release(); Object = object; if (Object) Object->AddRef(); }
    T* operator->() const { return Object; }
};
class CShopDef : public CDef { public:
    float AttitudeBuyPriceMultiplierDefault;
    std::map<EOpinionAttitudeType, float> AttitudeBuyPriceMultiplier;
};
class CThingBaseDef : public CDef { public:
    CShopDef* ShopSubDef;                       // null: the thing's def has no shop sub def
    CThingBaseDef() : ShopSubDef(0) { }
    bool GetSubDef(CDefPointer<CShopDef>* out) const
    {
        if (!ShopSubDef)
            return false;
        out->Set(ShopSubDef);
        return true;
    }
};
class CTCBuyableHouse { public: bool OwnedByPlayer; };
class CTCHeroStats { public: float TradeSkill; float GetTradeSkill() const { return TradeSkill; } };
class CThing { public:
    CTCBuyableHouse* House;                     // TCI_BUYABLE_HOUSE component (null: not a house)
    bool HasOpinionOfHero;                      // TCI_CREATURE_OPINION_OF_HERO present
    CTCHeroStats* HeroStats;                    // TCI_HERO_STATS component (null: absent)
    CThingBaseDef* Def;                         // what the definition manager finds for DefGlobalIndex
    EOpinionAttitudeType AttitudeToPlayer;      // what the opinion manager answers for this thing
    CThing() : House(0), HasOpinionOfHero(false), HeroStats(0), Def(0), AttitudeToPlayer(ATTITUDE_NEUTRAL) { }
};
struct CWorldModel {
    CThing* Hero;                               // the main player's controlled creature (may be null)
    int AttitudeQueries;
    CWorldModel() : Hero(0), AttitudeQueries(0) { }
    EOpinionAttitudeType GetThingAttitudeToPlayer(const CThing& thing) { ++AttitudeQueries; return thing.AttitudeToPlayer; }
};
static const float g_TradeSkillAttitudeScale = 0.84f;   // the float at 0x01382998

class CTCShop { public:
    CThing* ParentThing;
    CThing* ShopKeeper;                         // PShopKeeper.Get()
    CWorldModel* World;
    float GetAttitudeBuyPriceMultiplier() const
    {
        if (ParentThing->House) {
            if (ParentThing->House->OwnedByPlayer)
                return 1.0f;
        }
        CDefPointer<CThingBaseDef> thingDef;
        thingDef.Set(ParentThing->Def);
        CDefPointer<CShopDef> shopDef;
        if (!thingDef->GetSubDef(&shopDef))
            return 1.0f;
        float multiplier = shopDef->AttitudeBuyPriceMultiplierDefault;
        if (ShopKeeper && ShopKeeper->HasOpinionOfHero) {
            CWorldModel& manager = *World;
            const CThing& keeper = *ShopKeeper;
            EOpinionAttitudeType attitude = manager.GetThingAttitudeToPlayer(keeper);
            std::map<EOpinionAttitudeType, float>::const_iterator it = shopDef->AttitudeBuyPriceMultiplier.find(attitude);
            if (it != shopDef->AttitudeBuyPriceMultiplier.end())
                multiplier = it->second;
        }
        CThing* hero = World->Hero;
        if (hero && hero->HeroStats) {
            float skill = hero->HeroStats->GetTradeSkill();
            multiplier += (1.0f - multiplier) * (skill * g_TradeSkillAttitudeScale);
        }
        return multiplier;
    }
};

static bool Near(float a, float b) { return std::fabs(a - b) < 1e-5f; }

int main()
{
    bool ok = true;
    {
        CWorldModel world;
        CShopDef* shopDef = new CShopDef; shopDef->AddRef();
        shopDef->AttitudeBuyPriceMultiplierDefault = 1.5f;
        shopDef->AttitudeBuyPriceMultiplier[ATTITUDE_LIKE] = 0.8f;
        shopDef->AttitudeBuyPriceMultiplier[ATTITUDE_HATE] = 2.0f;
        CThingBaseDef* thingDef = new CThingBaseDef; thingDef->AddRef();
        thingDef->ShopSubDef = shopDef;
        CThingBaseDef* plainDef = new CThingBaseDef; plainDef->AddRef();     // no shop sub def

        CThing shopThing; shopThing.Def = thingDef;
        CThing keeper;
        CTCHeroStats heroStats; heroStats.TradeSkill = 0.5f;
        CThing hero;
        CTCShop shop; shop.ParentThing = &shopThing; shop.ShopKeeper = 0; shop.World = &world;

        // 1. no keeper, no hero: the def's default.
        if (!Near(shop.GetAttitudeBuyPriceMultiplier(), 1.5f)) ok = false;
        // 2. a keeper without an opinion of the hero is not asked for an attitude.
        shop.ShopKeeper = &keeper; keeper.AttitudeToPlayer = ATTITUDE_LIKE;
        if (!Near(shop.GetAttitudeBuyPriceMultiplier(), 1.5f) || world.AttitudeQueries != 0) ok = false;
        // 3. with an opinion: the table entry for the attitude.
        keeper.HasOpinionOfHero = true;
        if (!Near(shop.GetAttitudeBuyPriceMultiplier(), 0.8f) || world.AttitudeQueries != 1) ok = false;
        keeper.AttitudeToPlayer = ATTITUDE_HATE;
        if (!Near(shop.GetAttitudeBuyPriceMultiplier(), 2.0f)) ok = false;
        // 4. an attitude without an entry keeps the default.
        keeper.AttitudeToPlayer = ATTITUDE_NEUTRAL;
        if (!Near(shop.GetAttitudeBuyPriceMultiplier(), 1.5f)) ok = false;
        // 5. a hero without stats changes nothing; with stats the factor eases toward 1 by skill * 0.84.
        world.Hero = &hero; keeper.AttitudeToPlayer = ATTITUDE_HATE;
        if (!Near(shop.GetAttitudeBuyPriceMultiplier(), 2.0f)) ok = false;
        hero.HeroStats = &heroStats;
        if (!Near(shop.GetAttitudeBuyPriceMultiplier(), 2.0f + (1.0f - 2.0f) * (0.5f * 0.84f))) ok = false;   // 1.58
        keeper.AttitudeToPlayer = ATTITUDE_LIKE;
        if (!Near(shop.GetAttitudeBuyPriceMultiplier(), 0.8f + (1.0f - 0.8f) * (0.5f * 0.84f))) ok = false;   // 0.884
        heroStats.TradeSkill = 0.0f;
        if (!Near(shop.GetAttitudeBuyPriceMultiplier(), 0.8f)) ok = false;
        // 6. a thing whose def has no shop sub def: 1, whatever the keeper thinks.
        shopThing.Def = plainDef; heroStats.TradeSkill = 0.5f;
        int queries = world.AttitudeQueries;
        if (shop.GetAttitudeBuyPriceMultiplier() != 1.0f || world.AttitudeQueries != queries) ok = false;
        // 7. a house the player owns: 1 before any def is looked at; a house not owned falls through to the def path.
        shopThing.Def = thingDef;
        CTCBuyableHouse house; house.OwnedByPlayer = true; shopThing.House = &house;
        if (shop.GetAttitudeBuyPriceMultiplier() != 1.0f || world.AttitudeQueries != queries) ok = false;
        house.OwnedByPlayer = false;
        if (!Near(shop.GetAttitudeBuyPriceMultiplier(), 0.884f)) ok = false;
        // 8. every def handle the function took was released again.
        if (shopDef->RefCount != 1 || thingDef->RefCount != 1 || plainDef->RefCount != 1) ok = false;
        shopDef->Release(); thingDef->Release(); plainDef->Release();
    }
    if (g_liveDefs != 0) ok = false;
    if (!ok) { std::printf("007735d0_TEST FAIL\n"); return 1; }
    std::printf("007735d0_TEST PASS\n");
    return 0;
}