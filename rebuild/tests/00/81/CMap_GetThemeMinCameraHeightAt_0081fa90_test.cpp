// Behaviour test for 0081fa90: body copied from src/0081fa90.cpp (CMap::GetThemeMinCameraSizeZOffsetAt, the MinCameraHeight copy), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct C2DCoordI {
    long X, Y;
};
static int g_destroyed = 0;
struct CDefPointeeBase {
    virtual void DestroySelf() { ++g_destroyed; }
    long RefCount;
    void Release() { --RefCount; if (RefCount == 0) DestroySelf(); }
};
struct CEngineThemeDef : CDefPointeeBase {
    unsigned char _pad08[0x80 - 8];
    float Height;
    float MinCameraHeight;
};
static CEngineThemeDef g_def; static int g_getDef = 0; static long g_getDefIndex = -1;
struct CDefinitionManager {
    bool GetDef(long index, CEngineThemeDef** out) { ++g_getDef; g_getDefIndex = index; ++g_def.RefCount; *out = &g_def; return true; }
};
static CDefinitionManager g_manager;
static long g_blend = 0; static long g_theme = 0; static const C2DCoordI* g_pos = 0;
struct CMap {
    unsigned char       _pad00[0x1c];
    CDefinitionManager* DefinitionManager;
    long GetDominantEngineThemeAt(const C2DCoordI& pos, long* blend) { g_pos = &pos; *blend = g_blend; return g_theme; }
    float GetThemeMinCameraSizeZOffsetAt(const C2DCoordI& pos);
};
float CMap::GetThemeMinCameraSizeZOffsetAt(const C2DCoordI& pos)
{
    long blend;
    long themeIndex = GetDominantEngineThemeAt(pos, &blend);
    if (blend >= 255.0f) {
        CEngineThemeDef* def = 0;
        DefinitionManager->GetDef(themeIndex, &def);
        float value = def->MinCameraHeight;
        if (--def->RefCount == 0)
            def->DestroySelf();
        return value;
    }
    return 0.0f;
}
int main()
{
    g_def.RefCount = 1; g_def.Height = 12.5f; g_def.MinCameraHeight = 7.25f;
    CMap map; memset(&map, 0, sizeof(map)); map.DefinitionManager = &g_manager; C2DCoordI pos = { 3, 4 };
    // partial blend: 0 without a def lookup
    g_blend = 200; g_theme = 6; CHECK(map.GetThemeMinCameraSizeZOffsetAt(pos) == 0.0f); CHECK(g_pos == &pos && g_getDef == 0);
    // full blend: the theme def's value, the def released
    g_blend = 255; CHECK(map.GetThemeMinCameraSizeZOffsetAt(pos) == 7.25f); CHECK(g_getDef == 1 && g_getDefIndex == 6 && g_def.RefCount == 1 && g_destroyed == 0);
    g_blend = 300; CHECK(map.GetThemeMinCameraSizeZOffsetAt(pos) == 7.25f);
    // a def held only by the lookup is destroyed
    g_def.RefCount = 0; CHECK(map.GetThemeMinCameraSizeZOffsetAt(pos) == 7.25f); CHECK(g_destroyed == 1);
    if (fails == 0) printf("0081fa90_TEST PASS\n"); else printf("0081fa90_TEST FAIL\n");
    return fails;
}