// Behaviour test for 0x00BD5C90 CEnginePrimitiveManagerAnimatedMeshesBase::RenderPrimitiveToShadowBuffer(CEngineInternalPrimitiveBase&):
// a host model of the row's logic -- the early-outs (manager disabled, flag 0x2000, caster clip of the interpolated sphere, SetupMesh
// failure), the morphed types' hidden materials / composite surfaces (types 11 and 38), the colour-buffer technique (2) binding the
// depth-pack texture once with the diffuse texture on stage 1 and uploading {128, MinWClamp, 0, 0}, and per mesh primitive: skip empty
// and hidden primitives, alpha test on for transparent materials (skipped when they have no diffuse map; their diffuse texture is
// attached unless it is still loading, and a loading texture also skips the draw), off for opaque ones, then the palette-skinned draw.
#include <cstdio>

enum { RENDERED_NOTHING = 0, RENDERED_OBJECT = 2 };
struct Material { bool Transparent; unsigned long DiffuseIndex; bool Loading; };
struct MeshPrim { unsigned long TriangleCount; long MaterialIndex; };
struct Primitive { unsigned long Flags; long Type; const unsigned char* Hidden; };

static bool g_Enable, g_ClipVisible, g_SetupOk;
static long g_Technique; static float g_MinWClamp;
static Material g_Materials[3]; static MeshPrim g_Prims[4]; static unsigned long g_PrimCount;
static int g_DepthPackAttaches, g_ConstantUploads, g_VertexShaders, g_AlphaOn, g_AlphaOff, g_Draws, g_TextureAttaches, g_SurfaceUses;
static float g_Constant[4]; static long g_LastStage;

static int RenderPrimitiveToShadowBuffer(Primitive& primitive)
{
    if (!g_Enable) return RENDERED_NOTHING;
    if (primitive.Flags & 0x2000) return RENDERED_NOTHING;
    if (!g_ClipVisible) return RENDERED_NOTHING;
    if (!g_SetupOk) return RENDERED_NOTHING;
    const unsigned char* hidden = 0; bool surfaces = false;
    if (primitive.Type == 11 || primitive.Type == 38) { hidden = primitive.Hidden; surfaces = true; }
    long diffuse_stage = 0;
    if (g_Technique == 2) {
        diffuse_stage = 1;
        ++g_DepthPackAttaches;
        g_Constant[0] = 128.0f; g_Constant[1] = g_MinWClamp; g_Constant[2] = 0.0f; g_Constant[3] = 0.0f; ++g_ConstantUploads;
    }
    for (unsigned long i = 0; i < g_PrimCount; ++i) {
        const MeshPrim& mp = g_Prims[i];
        if (mp.TriangleCount == 0) continue;
        const Material& material = g_Materials[mp.MaterialIndex];
        if (hidden && hidden[mp.MaterialIndex]) continue;
        ++g_VertexShaders;
        if (material.Transparent) ++g_AlphaOn; else ++g_AlphaOff;
        bool preloading = false;
        if (material.Transparent) {
            if (material.DiffuseIndex == 0) continue;
            if (surfaces) ++g_SurfaceUses;
            preloading = material.Loading;
            if (!preloading) { ++g_TextureAttaches; g_LastStage = diffuse_stage; }
        }
        if (!preloading) ++g_Draws;
    }
    return RENDERED_OBJECT;
}

int main()
{
    bool ok = true;
    static const unsigned char hidden[3] = { 0, 1, 0 };
    Primitive p = { 0, 11, hidden };
    g_Materials[0].Transparent = false; g_Materials[0].DiffuseIndex = 5; g_Materials[0].Loading = false;
    g_Materials[1].Transparent = false; g_Materials[1].DiffuseIndex = 5; g_Materials[1].Loading = false;
    g_Materials[2].Transparent = true;  g_Materials[2].DiffuseIndex = 9; g_Materials[2].Loading = false;
    g_Prims[0].TriangleCount = 12; g_Prims[0].MaterialIndex = 0;
    g_Prims[1].TriangleCount = 0;  g_Prims[1].MaterialIndex = 0;   // empty: skipped
    g_Prims[2].TriangleCount = 8;  g_Prims[2].MaterialIndex = 1;   // hidden for the morphed types
    g_Prims[3].TriangleCount = 6;  g_Prims[3].MaterialIndex = 2;   // transparent
    g_PrimCount = 4;
    g_Enable = false; ok = ok && RenderPrimitiveToShadowBuffer(p) == RENDERED_NOTHING;
    g_Enable = true; p.Flags = 0x2000; ok = ok && RenderPrimitiveToShadowBuffer(p) == RENDERED_NOTHING;
    p.Flags = 0; g_ClipVisible = false; ok = ok && RenderPrimitiveToShadowBuffer(p) == RENDERED_NOTHING;
    g_ClipVisible = true; g_SetupOk = false; ok = ok && RenderPrimitiveToShadowBuffer(p) == RENDERED_NOTHING;
    ok = ok && g_VertexShaders == 0 && g_ConstantUploads == 0;
    // colour-buffer technique, morphed mesh: one constant upload, primitives 0 and 3 draw
    g_SetupOk = true; g_Technique = 2; g_MinWClamp = 0.5f;
    ok = ok && RenderPrimitiveToShadowBuffer(p) == RENDERED_OBJECT;
    ok = ok && g_ConstantUploads == 1 && g_DepthPackAttaches == 1 && g_Constant[0] == 128.0f && g_Constant[1] == 0.5f;
    ok = ok && g_VertexShaders == 2 && g_Draws == 2 && g_AlphaOn == 1 && g_AlphaOff == 1 && g_TextureAttaches == 1 && g_LastStage == 1;
    ok = ok && g_SurfaceUses == 1;
    // plain animated mesh, other technique, the transparent texture still loading: no attach, no draw for it
    p.Type = 3; g_Technique = 1; g_Materials[2].Loading = true;
    ok = ok && RenderPrimitiveToShadowBuffer(p) == RENDERED_OBJECT;
    ok = ok && g_ConstantUploads == 1 && g_VertexShaders == 5 && g_Draws == 4 && g_TextureAttaches == 1 && g_AlphaOff == 3;
    ok = ok && g_SurfaceUses == 1;
    // transparent without a diffuse map: skipped after the alpha state
    g_Materials[2].DiffuseIndex = 0; g_Materials[2].Loading = false;
    ok = ok && RenderPrimitiveToShadowBuffer(p) == RENDERED_OBJECT;
    ok = ok && g_VertexShaders == 8 && g_Draws == 6 && g_AlphaOn == 3 && g_TextureAttaches == 1;
    if (!ok) { std::printf("00bd5c90_TEST FAIL\n"); return 1; }
    std::printf("00bd5c90_TEST PASS\n");
    return 0;
}