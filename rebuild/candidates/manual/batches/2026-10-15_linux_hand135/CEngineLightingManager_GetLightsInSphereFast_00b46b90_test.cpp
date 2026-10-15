// Behaviour test for CEngineLightingManager::GetLightsInSphereFast @ 0x00B46B90.
#include <cstdio>
struct CEngineLocalLightArray { int id; };
struct C3DBoundingSphere { float X, Y, Z, Radius; };
struct CEnginePrimitiveThemeChannelDesc { unsigned char ChannelIndex0, ChannelIndex1, ChannelBlendFactor, Pad; };
static const float g_limit = 4.0f;
static CEngineLocalLightArray g_cached = { 1 }, g_full = { 2 };
static int g_cacheCalls = 0, g_fullCalls = 0, g_cacheHit = 1; static float g_minX, g_minY; static long g_mask;
struct CEngineLightingManager {
    bool EnableLightCache;
    CEngineLocalLightArray* LookupSmallObjectLightCache(float x, float y, long m) { ++g_cacheCalls; g_minX = x; g_minY = y; g_mask = m; return g_cacheHit ? &g_cached : 0; }
    CEngineLocalLightArray* GetLightsInSphere(const C3DBoundingSphere&, long, const CEnginePrimitiveThemeChannelDesc&) { ++g_fullCalls; return &g_full; }
    CEngineLocalLightArray* GetLightsInSphereFast(const C3DBoundingSphere& sphere, long layerMask, const CEnginePrimitiveThemeChannelDesc& theme)
    {
        if (sphere.Radius + sphere.Radius <= g_limit && theme.ChannelIndex0 == 0 && theme.ChannelBlendFactor == 0 && EnableLightCache) {
            CEngineLocalLightArray* lights = LookupSmallObjectLightCache(sphere.X - sphere.Radius, sphere.Y - sphere.Radius, layerMask);
            if (lights) return lights;
        }
        return GetLightsInSphere(sphere, layerMask, theme);
    }
};
int main()
{
    CEngineLightingManager m; m.EnableLightCache = true;
    C3DBoundingSphere s = { 10, 20, 30, 2 }; CEnginePrimitiveThemeChannelDesc t = { 0, 5, 0, 0 };
    int ok = m.GetLightsInSphereFast(s, 7, t) == &g_cached && g_minX == 8 && g_minY == 18 && g_mask == 7 && g_fullCalls == 0;
    s.Radius = 2.5f; ok = ok && m.GetLightsInSphereFast(s, 7, t) == &g_full && g_cacheCalls == 1;   // 5 > 4
    s.Radius = 1; t.ChannelIndex0 = 1; ok = ok && m.GetLightsInSphereFast(s, 7, t) == &g_full && g_cacheCalls == 1;
    t.ChannelIndex0 = 0; t.ChannelBlendFactor = 9; ok = ok && m.GetLightsInSphereFast(s, 7, t) == &g_full && g_cacheCalls == 1;
    t.ChannelBlendFactor = 0; m.EnableLightCache = false; ok = ok && m.GetLightsInSphereFast(s, 7, t) == &g_full && g_cacheCalls == 1;
    m.EnableLightCache = true; g_cacheHit = 0; ok = ok && m.GetLightsInSphereFast(s, 7, t) == &g_full && g_cacheCalls == 2 && g_fullCalls == 5;
    if (!ok) { std::printf("00b46b90_TEST FAIL\n"); return 1; }
    std::printf("00b46b90_TEST PASS\n");
    return 0;
}
