// Behaviour test for CThingCreatureBase::DrawPreparePrimitivesForRendering (0x00663C40): a copy of the decision logic
// over a stub thing (interface bitset + sorted TC map, physical pass result, appearance, LOD distance, firefly property).
#include <cstdio>

enum ETCInterfaceType {
    TC_EMOTE_ICON = 0x45,
    TC_WEAPON_TRAIL = 0x4c,
    TC_CREATURE_HIT_NOTIFICATION = 0xcd,
    TC_CREATURE_FLY_ON_SPLINE = 0xd9
};
enum EAction {
    A_FLY_PREPARE = 1, A_APPEARANCE_DRAW, A_WEAPON_TRAILS, A_HIT_NOTIFICATION, A_EMOTE_PREPARE, A_FIREFLY_SPRITE,
    A_APPEARANCE_CLEAR, A_EMOTE_CLEAR
};

static int g_log[32];
static int g_logCount;
static void Log(int action) { if (g_logCount < 32) g_log[g_logCount++] = action; }

struct Vec { float X, Y, Z; };
struct TCEntry { ETCInterfaceType first; int second; };

struct Thing {
    unsigned int UsingComponents[9];
    TCEntry Components[8];
    int ComponentCount;
    bool PhysicalResult;
    bool HasAppearance;
    float FadeDistance;
    Vec LODCentre;
    Vec Pos;
    int Property;
    int FireflyGraphic;
    int SpriteGraphic;
    float SpriteSize;

    bool HasTC(ETCInterfaceType type) const
    {
        if (UsingComponents[type / 32] & (1U << (type % 32)))
            return true;
        return false;
    }
    const TCEntry* LowerBound(ETCInterfaceType key) const
    {
        int i = 0;
        while (i < ComponentCount && Components[i].first < key)
            ++i;
        return Components + i;
    }
    int GetTC(ETCInterfaceType type) const
    {
        const TCEntry* end = Components + ComponentCount;
        const TCEntry* p = LowerBound(type);
        return (p == end || type < p->first ? end : p)->second;
    }
    void AddTC(ETCInterfaceType type, int id)
    {
        UsingComponents[type / 32] |= 1U << (type % 32);
        int i = ComponentCount;
        while (i > 0 && type < Components[i - 1].first) { Components[i] = Components[i - 1]; --i; }
        Components[i].first = type;
        Components[i].second = id;
        ++ComponentCount;
        Components[ComponentCount].first = (ETCInterfaceType)0;
        Components[ComponentCount].second = -1;   // the end slot
    }
};

static float g_primitiveDrawDistanceScale;
static bool g_ShowWeaponTrails;

static bool DistanceSmallerThan(const Vec& a, const Vec& b, float distance)
{
    float dx = a.X - b.X, dy = a.Y - b.Y, dz = a.Z - b.Z;
    return dx * dx + dy * dy + dz * dz < distance * distance;
}

static bool DrawPreparePrimitivesForRendering(Thing& t)
{
    if (t.HasTC(TC_CREATURE_FLY_ON_SPLINE) && t.GetTC(TC_CREATURE_FLY_ON_SPLINE) >= 0)
        Log(A_FLY_PREPARE);
    if (!t.PhysicalResult)
        return false;
    if (t.HasAppearance) {
        float distance = t.FadeDistance * (g_primitiveDrawDistanceScale + 1.0f);
        if (DistanceSmallerThan(t.LODCentre, t.Pos, distance)) {
            Log(A_APPEARANCE_DRAW);
            if (g_ShowWeaponTrails) {
                if (t.HasTC(TC_WEAPON_TRAIL) && t.GetTC(TC_WEAPON_TRAIL) >= 0)
                    Log(A_WEAPON_TRAILS);
                if (t.HasTC(TC_CREATURE_HIT_NOTIFICATION) && t.GetTC(TC_CREATURE_HIT_NOTIFICATION) >= 0)
                    Log(A_HIT_NOTIFICATION);
            }
            if (t.HasTC(TC_EMOTE_ICON) && t.GetTC(TC_EMOTE_ICON) >= 0)
                Log(A_EMOTE_PREPARE);
            if (t.Property == 4) {
                t.SpriteGraphic = t.FireflyGraphic;
                t.SpriteSize = 0.4f;
                Log(A_FIREFLY_SPRITE);
            }
        } else {
            Log(A_APPEARANCE_CLEAR);
            if (t.HasTC(TC_EMOTE_ICON) && t.GetTC(TC_EMOTE_ICON) >= 0)
                Log(A_EMOTE_CLEAR);
        }
    }
    return true;
}

static void Reset(Thing& t)
{
    for (int i = 0; i < 9; ++i) t.UsingComponents[i] = 0;
    t.ComponentCount = 0;
    t.Components[0].first = (ETCInterfaceType)0;
    t.Components[0].second = -1;
    t.PhysicalResult = true;
    t.HasAppearance = true;
    t.FadeDistance = 10.0f;
    t.LODCentre.X = 0.0f; t.LODCentre.Y = 0.0f; t.LODCentre.Z = 0.0f;
    t.Pos.X = 3.0f; t.Pos.Y = 4.0f; t.Pos.Z = 0.0f;
    t.Property = 0;
    t.FireflyGraphic = 77;
    t.SpriteGraphic = 0;
    t.SpriteSize = 0.0f;
    g_logCount = 0;
}

static bool Expect(const int* actions, int count)
{
    if (g_logCount != count) return false;
    for (int i = 0; i < count; ++i)
        if (g_log[i] != actions[i]) return false;
    return true;
}

int main()
{
    Thing t;
    int fails = 0;
    g_primitiveDrawDistanceScale = 0.0f;
    g_ShowWeaponTrails = true;

    // physical pass false: the fly-on-spline TC still prepares, then false
    Reset(t);
    t.AddTC(TC_CREATURE_FLY_ON_SPLINE, 1);
    t.PhysicalResult = false;
    { int e[] = { A_FLY_PREPARE }; if (DrawPreparePrimitivesForRendering(t) || !Expect(e, 1)) ++fails; }

    // no appearance: true, nothing drawn
    Reset(t);
    t.HasAppearance = false;
    if (!DrawPreparePrimitivesForRendering(t) || g_logCount != 0) ++fails;

    // in range with every TC and a firefly creature
    Reset(t);
    t.AddTC(TC_EMOTE_ICON, 2);
    t.AddTC(TC_CREATURE_HIT_NOTIFICATION, 3);
    t.AddTC(TC_WEAPON_TRAIL, 4);
    t.Property = 4;
    {
        int e[] = { A_APPEARANCE_DRAW, A_WEAPON_TRAILS, A_HIT_NOTIFICATION, A_EMOTE_PREPARE, A_FIREFLY_SPRITE };
        if (!DrawPreparePrimitivesForRendering(t) || !Expect(e, 5) || t.SpriteGraphic != 77 || t.SpriteSize != 0.4f) ++fails;
    }

    // trails hidden: no trail / hit notification
    Reset(t);
    t.AddTC(TC_WEAPON_TRAIL, 4);
    t.AddTC(TC_CREATURE_HIT_NOTIFICATION, 3);
    g_ShowWeaponTrails = false;
    { int e[] = { A_APPEARANCE_DRAW }; if (!DrawPreparePrimitivesForRendering(t) || !Expect(e, 1)) ++fails; }
    g_ShowWeaponTrails = true;

    // out of range (distance 5 vs fade 10 * (scale -0.6 + 1) = 4): clear appearance and emote icon
    Reset(t);
    t.AddTC(TC_EMOTE_ICON, 2);
    t.Property = 4;
    g_primitiveDrawDistanceScale = -0.6f;
    { int e[] = { A_APPEARANCE_CLEAR, A_EMOTE_CLEAR }; if (!DrawPreparePrimitivesForRendering(t) || !Expect(e, 2) || t.SpriteGraphic != 0) ++fails; }
    g_primitiveDrawDistanceScale = 0.0f;

    // GetTC lookup returns the matching entry from the sorted map
    Reset(t);
    t.AddTC(TC_CREATURE_FLY_ON_SPLINE, 9);
    t.AddTC(TC_EMOTE_ICON, 5);
    if (t.GetTC(TC_EMOTE_ICON) != 5 || t.GetTC(TC_CREATURE_FLY_ON_SPLINE) != 9 || t.GetTC(TC_WEAPON_TRAIL) != -1) ++fails;

    if (fails) { std::printf("00663c40_TEST FAIL\n"); return 1; }
    std::printf("00663c40_TEST PASS\n");
    return 0;
}