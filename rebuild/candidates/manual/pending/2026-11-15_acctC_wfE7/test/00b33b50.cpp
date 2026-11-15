// Behaviour test for CEnginePrimitiveRenderer::CEnginePrimitiveRenderer (0x00B33B50): a model of the constructor body -- two scenes
// (main 0.7 / repeated-mesh 1.2 cell-size ratio, map size 128, 6 levels, 6 primitives per cell), 21 managers registered in the retail
// order (every one exactly once, sprite trails handed to the particle sprite-trail group), the render defaults, software vertex
// processing (usage flag 0x10 and init flag 1) only below vertex-shader version 1100, DP3 bump enable passed to an existing main mesh
// bank only, the colour-buffer shadow shaders only for pixel-shader version >= 1400, the shadow pointers aimed at the depth-buffer set,
// the LOD tolerances copied from the engine def and the shared-data global published last.
#include <cstdio>
#include <string>
#include <vector>

struct Scene { float CellRatio; std::string Name; };
struct Model {
    std::vector<Scene> Scenes; std::vector<std::string> Registered; bool SpriteTrailLink;
    unsigned long VBUsage, IBUsage, InitFlags; bool SoftwareVS, SoftwareFF; int Dp3Calls; bool Dp3Value;
    std::vector<std::string> Shaders; const std::string* ShadowRender; std::string DepthShadow; float LODErr, CharLODErr; long LODForced;
    float MaxPreload, MinPreload; bool Published;
};
static void Construct(Model& m, unsigned long vs, unsigned long ps, bool has_mesh_bank, float lod, float char_lod)
{
    Scene a = { 0.7f, "MainScene" }, b = { 1.2f, "RepMeshScene" }; m.Scenes.push_back(a); m.Scenes.push_back(b);
    static const char* order[21] = { "RepeatedZSprites", "StaticMeshes", "AnimatedMeshes", "RepeatedStaticMeshes", "Sprites", "FlareSprites",
        "WeaponTrails", "SpriteTrails", "Lines", "Text", "OcclusionGroup", "PrimitiveGroup", "Changer", "ParticleLineGroup", "ParticleMeshGroup",
        "ParticleSpriteGroup", "ParticleSpriteTrailGroup", "ParticleGroup", "Decal", "DecalGroup", "WeatherMask" };
    for (int i = 0; i < 21; ++i) m.Registered.push_back(order[i]);
    m.SpriteTrailLink = true;
    m.VBUsage = 0; m.IBUsage = 0; m.InitFlags = 0; m.SoftwareVS = false; m.SoftwareFF = false;
    if (vs < 1100) { m.SoftwareVS = true; m.VBUsage |= 0x10; m.IBUsage |= 0x10; m.InitFlags |= 1; }
    m.Dp3Calls = 0; m.Dp3Value = true;
    if (has_mesh_bank) { ++m.Dp3Calls; m.Dp3Value = false; }
    m.Shaders.push_back("PSHADER_DIFFUSE_ONLY"); m.Shaders.push_back("PSHADER_SHADOW_BUFFER_RENDER");
    if (ps >= 1400) m.Shaders.push_back("PSHADER_TEXTURE_DIFFUSE_FOG_COLBUFF_SHADOW");
    m.DepthShadow = "PSHADER_SHADOW_BUFFER_RENDER"; m.ShadowRender = &m.DepthShadow;
    m.LODErr = lod; m.CharLODErr = char_lod; m.LODForced = -1; m.MaxPreload = 15.0f; m.MinPreload = 5.0f;
    m.Published = true;
}
static int g_fail = 0;
#define CHECK(x) do { if (!(x)) { std::printf("FAIL line %d: %s\n", __LINE__, #x); g_fail = 1; } } while (0)
int main()
{
    Model a; Construct(a, 1099, 1400, true, 0.5f, 0.25f);
    CHECK(a.Scenes.size() == 2 && a.Scenes[0].Name == "MainScene" && a.Scenes[1].CellRatio == 1.2f);
    CHECK(a.Registered.size() == 21 && a.Registered[0] == "RepeatedZSprites" && a.Registered[20] == "WeatherMask" && a.SpriteTrailLink);
    CHECK(a.SoftwareVS && a.VBUsage == 0x10 && a.IBUsage == 0x10 && a.InitFlags == 1);
    CHECK(a.Dp3Calls == 1 && !a.Dp3Value && a.Shaders.size() == 3 && *a.ShadowRender == "PSHADER_SHADOW_BUFFER_RENDER");
    CHECK(a.LODErr == 0.5f && a.CharLODErr == 0.25f && a.LODForced == -1 && a.MaxPreload == 15.0f && a.Published);
    Model b; Construct(b, 1100, 1399, false, 1.0f, 2.0f);
    CHECK(!b.SoftwareVS && b.VBUsage == 0 && b.InitFlags == 0 && b.Dp3Calls == 0 && b.Shaders.size() == 2);
    if (!g_fail) std::printf("00b33b50_TEST PASS\n");
    return g_fail;
}
