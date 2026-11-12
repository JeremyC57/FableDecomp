// Behaviour test for CTCShop::GetAttitudeSellPriceMultiplier (0x00773790): a copy of the function's logic over a small model.
#include <cstdio>

struct House { bool OwnedByPlayer; };
struct ShopDef {
    float AttitudeSellPriceMultiplierDefault;
    int   keys[4];
    float values[4];
    int   count;
    const float* find(int attitude) const
    {
        for (int i = 0; i < count; ++i)
            if (keys[i] == attitude)
                return &values[i];
        return 0;
    }
};
struct Keeper { bool hasOpinionOfHero; int attitudeToPlayer; };
struct Hero { bool hasStats; float tradeSkill; };
struct Shop {
    const House*   house;      // the parent thing's buyable-house component (0 when absent)
    const ShopDef* shopDef;    // the thing def's shop sub-def (0 when absent)
    const Keeper*  keeper;     // PShopKeeper target
    const Hero*    hero;       // the main player's controlled creature
};

static const float g_TradeSkillAttitudeScale = 0.84f;
static int g_releases = 0;     // def handles released (thing def, and the shop def when found)

static float GetAttitudeSellPriceMultiplier(const Shop& shop)
{
    if (shop.house) {
        if (shop.house->OwnedByPlayer)
            return 1.0f;
    }
    if (!shop.shopDef) {
        g_releases += 1;
        return 1.0f;
    }
    float multiplier = shop.shopDef->AttitudeSellPriceMultiplierDefault;
    if (shop.keeper && shop.keeper->hasOpinionOfHero) {
        int attitude = shop.keeper->attitudeToPlayer;
        const float* it = shop.shopDef->find(attitude);
        if (it != 0)
            multiplier = *it;
    }
    const Hero* hero = shop.hero;
    if (hero) {
        if (hero->hasStats) {
            float skill = hero->tradeSkill;
            multiplier += (1.0f - multiplier) * (skill * g_TradeSkillAttitudeScale);
        }
    }
    g_releases += 2;
    return multiplier;
}

static bool Near(float a, float b)
{
    float d = a - b;
    if (d < 0.0f)
        d = -d;
    return d < 0.00001f;
}

int main()
{
    bool ok = true;
    House owned = { true }, rented = { false };
    ShopDef def = { 0.5f, { 1, 3, 0, 0 }, { 0.25f, 0.75f, 0.0f, 0.0f }, 2 };
    Keeper friendly = { true, 3 }, unknown = { true, 2 }, noOpinion = { false, 3 };
    Hero skilled = { true, 0.5f }, statless = { false, 0.5f }, unskilled = { true, 0.0f };

    Shop s = { &owned, &def, &friendly, &skilled };
    g_releases = 0;
    ok = ok && GetAttitudeSellPriceMultiplier(s) == 1.0f && g_releases == 0;          // owned house: 1, no def lookup

    Shop noDef = { &rented, 0, &friendly, &skilled };
    g_releases = 0;
    ok = ok && GetAttitudeSellPriceMultiplier(noDef) == 1.0f && g_releases == 1;      // no shop def: 1, thing def released

    Shop plain = { 0, &def, 0, 0 };
    g_releases = 0;
    ok = ok && GetAttitudeSellPriceMultiplier(plain) == 0.5f && g_releases == 2;      // default multiplier

    Shop notOwned = { &rented, &def, &noOpinion, 0 };
    ok = ok && GetAttitudeSellPriceMultiplier(notOwned) == 0.5f;                      // keeper without the opinion component

    Shop mapped = { 0, &def, &friendly, 0 };
    ok = ok && GetAttitudeSellPriceMultiplier(mapped) == 0.75f;                       // attitude entry replaces the default

    Shop unmapped = { 0, &def, &unknown, 0 };
    ok = ok && GetAttitudeSellPriceMultiplier(unmapped) == 0.5f;                      // no entry: default kept

    Shop eased = { 0, &def, &friendly, &skilled };
    ok = ok && Near(GetAttitudeSellPriceMultiplier(eased), 0.75f + 0.25f * (0.5f * 0.84f));   // eased toward 1 by the trade skill

    Shop noStats = { 0, &def, &friendly, &statless };
    ok = ok && GetAttitudeSellPriceMultiplier(noStats) == 0.75f;                      // hero without the stats component

    Shop zeroSkill = { 0, &def, &unknown, &unskilled };
    ok = ok && Near(GetAttitudeSellPriceMultiplier(zeroSkill), 0.5f);                 // zero skill: unchanged

    if (!ok) {
        std::printf("00773790_TEST FAIL\n");
        return 1;
    }
    std::printf("00773790_TEST PASS\n");
    return 0;
}