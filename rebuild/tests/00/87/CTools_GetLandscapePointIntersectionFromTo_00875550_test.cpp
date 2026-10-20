// Behaviour test for CTools::GetLandscapePointIntersectionFromTo (00875550).
#include <cstdio>
struct C3DVector { float x, y, z; };
struct Map {
    float ground, water;
    float GetGroundSizeZAt(const C3DVector&) const { return ground; }
    float GetWaterSizeZAt(const C3DVector&) const { return water; }
    C3DVector GetGroundFaceNormalAt(const C3DVector& p) const { C3DVector n = { 0, p.x, 1 }; return n; }
};
static int g_fallback = 0;
static bool Fallback(const C3DVector*, const C3DVector*, C3DVector* pos, C3DVector*, bool) { ++g_fallback; pos->z = -5; return true; }
inline float HigherOf(float c, float cur) { return c > cur ? c : cur; }
static bool Get(const Map* map, const C3DVector& from, const C3DVector& to, C3DVector& hitPos, C3DVector& hitNormal, bool includeWater)
{
    float ground = map->GetGroundSizeZAt(from);
    float height = includeWater ? HigherOf(map->GetWaterSizeZAt(from), ground) : ground;
    if (height > from.z) { hitPos = from; hitPos.z = height; hitNormal = map->GetGroundFaceNormalAt(hitPos); return true; }
    return Fallback(&from, &to, &hitPos, &hitNormal, includeWater) ? true : false;
}
int main()
{
    Map m = { 2, 5 };
    C3DVector from = { 3, 0, 3 }, to = { 0, 0, 0 }, p, n;
    bool ok = Get(&m, from, to, p, n, true) && p.z == 5 && p.x == 3 && n.y == 3 && g_fallback == 0;   // under water
    ok = ok && Get(&m, from, to, p, n, false) && p.z == -5 && g_fallback == 1;                        // above ground
    m.ground = 4; ok = ok && Get(&m, from, to, p, n, false) && p.z == 4 && g_fallback == 1;
    if (!ok) { std::printf("00875550_TEST FAIL\n"); return 1; }
    std::printf("00875550_TEST PASS\n");
    return 0;
}