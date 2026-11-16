// Behaviour test for 0x00BA6240 CEnginePrimitiveManagerStaticMeshesBase::RenderPrimitiveToShadowBuffer(CEngineInternalPrimitiveBase&):
// a host model of the row's logic -- the three early-outs (manager disabled, flag 0x2000, no shadow mesh), the caster clip of the
// world-space sphere, the LOD / mesh fetch (failure returns nothing), the render info, the morphed type's hidden materials, and per mesh
// primitive: skip empty and hidden primitives, the colour-buffer technique (2) binds the depth-pack texture to stage 0 with diffuse on
// stage 1 and uploads {128, MinWClamp, 0, 0}, transparent materials without a diffuse map are skipped, transparent ones get their mipmap,
// real textures and the alpha test, opaque ones the blank textures; SetupStaticBlocks success sets the first textures and draws.
#include <cstdio>

enum { RENDERED_NOTHING = 0, RENDERED_OBJECT = 2 };
struct Material { bool Transparent; unsigned long DiffuseIndex; };
struct MeshPrim { unsigned long TriangleCount; long MaterialIndex; unsigned long Mipmap; };
struct Stages { long Diffuse, Bump; bool UseBlank; };
struct Primitive { unsigned long Flags; bool HasShadowMesh; float Radius; long Type; const unsigned char* Hidden; unsigned long Mesh, LOD; };

static bool g_Enable, g_ClipVisible, g_MeshOk, g_SetupOk;
static long g_Technique; static float g_MinWClamp;
static Material g_Materials[3]; static MeshPrim g_Prims[4]; static unsigned long g_PrimCount;
static int g_DepthPackAttaches, g_ConstantUploads, g_VertexShaders, g_AlphaOn, g_AlphaOff, g_Draws, g_FirstTextures, g_Flushes;
static float g_Constant[4]; static Stages g_LastStages; static unsigned long g_LastMipmap;

static bool SetupStaticBlocks(const MeshPrim&, Stages& stages, unsigned long mipmap, bool* used_blank)
{
    *used_blank = stages.UseBlank; g_LastStages = stages; g_LastMipmap = mipmap; return g_SetupOk;
}

static int RenderPrimitiveToShadowBuffer(Primitive& primitive)
{
    if (!g_Enable) return RENDERED_NOTHING;
    if (primitive.Flags & 0x2000) return RENDERED_NOTHING;
    if (!primitive.HasShadowMesh) return RENDERED_NOTHING;
    if (!g_ClipVisible) return RENDERED_NOTHING;
    unsigned long lod = primitive.Radius > 10.0f ? 1 : 0;
    if (!g_MeshOk) return RENDERED_NOTHING;
    primitive.Mesh = 77; primitive.LOD = lod;
    const unsigned char* hidden = 0;
    if (primitive.Type == 10) hidden = primitive.Hidden;
    for (unsigned long i = 0; i < g_PrimCount; ++i) {
        MeshPrim& mp = g_Prims[i];
        if (mp.TriangleCount == 0) continue;
        const Material& material = g_Materials[mp.MaterialIndex];
        if (hidden && hidden[mp.MaterialIndex]) continue;
        long technique = g_Technique;
        unsigned long mipmap = 0;
        Stages stages = { -1, -1, false };
        if (technique == 2) {
            stages.Diffuse = 1;
            ++g_DepthPackAttaches;
            g_Constant[0] = 128.0f; g_Constant[1] = g_MinWClamp; g_Constant[2] = 0.0f; g_Constant[3] = 0.0f; ++g_ConstantUploads;
        } else {
            stages.Diffuse = 0;
        }
        ++g_VertexShaders;
        if (material.Transparent) {
            if (material.DiffuseIndex == 0) continue;
            stages.UseBlank = false;
            mipmap = mp.Mipmap;
            ++g_AlphaOn;
        } else {
            stages.UseBlank = true;
            ++g_AlphaOff;
        }
        bool used_blank;
        if (SetupStaticBlocks(mp, stages, mipmap, &used_blank)) {
            ++g_FirstTextures; ++g_Draws;
        }
    }
    ++g_Flushes;
    return RENDERED_OBJECT;
}

int main()
{
    bool ok = true;
    static const unsigned char hidden[3] = { 0, 1, 0 };
    Primitive p = { 0, true, 4.0f, 10, hidden, 0, 0 };
    g_Materials[0].Transparent = false; g_Materials[0].DiffuseIndex = 5;
    g_Materials[1].Transparent = false; g_Materials[1].DiffuseIndex = 5;
    g_Materials[2].Transparent = true;  g_Materials[2].DiffuseIndex = 9;
    g_Prims[0].TriangleCount = 12; g_Prims[0].MaterialIndex = 0; g_Prims[0].Mipmap = 0;
    g_Prims[1].TriangleCount = 0;  g_Prims[1].MaterialIndex = 0; g_Prims[1].Mipmap = 0;   // empty: skipped
    g_Prims[2].TriangleCount = 8;  g_Prims[2].MaterialIndex = 1; g_Prims[2].Mipmap = 0;   // hidden material: skipped
    g_Prims[3].TriangleCount = 6;  g_Prims[3].MaterialIndex = 2; g_Prims[3].Mipmap = 3;   // transparent
    g_PrimCount = 4;
    // early outs
    g_Enable = false; ok = ok && RenderPrimitiveToShadowBuffer(p) == RENDERED_NOTHING;
    g_Enable = true; p.Flags = 0x2000; ok = ok && RenderPrimitiveToShadowBuffer(p) == RENDERED_NOTHING;
    p.Flags = 0x1000; p.HasShadowMesh = false; ok = ok && RenderPrimitiveToShadowBuffer(p) == RENDERED_NOTHING;
    p.HasShadowMesh = true; g_ClipVisible = false; ok = ok && RenderPrimitiveToShadowBuffer(p) == RENDERED_NOTHING;
    g_ClipVisible = true; g_MeshOk = false; ok = ok && RenderPrimitiveToShadowBuffer(p) == RENDERED_NOTHING;
    ok = ok && g_Flushes == 0 && g_VertexShaders == 0 && p.Mesh == 0;
    // colour-buffer technique, morphed primitive: primitives 0 and 3 draw, 1 (empty) and 2 (hidden) are skipped
    g_MeshOk = true; g_SetupOk = true; g_Technique = 2; g_MinWClamp = 0.25f;
    ok = ok && RenderPrimitiveToShadowBuffer(p) == RENDERED_OBJECT;
    ok = ok && p.Mesh == 77 && g_Flushes == 1 && g_VertexShaders == 2 && g_Draws == 2 && g_FirstTextures == 2;
    ok = ok && g_DepthPackAttaches == 2 && g_ConstantUploads == 2 && g_Constant[0] == 128.0f && g_Constant[1] == 0.25f && g_Constant[3] == 0.0f;
    ok = ok && g_AlphaOff == 1 && g_AlphaOn == 1 && g_LastStages.Diffuse == 1 && g_LastStages.Bump == -1 && !g_LastStages.UseBlank && g_LastMipmap == 3;
    // shadow-volume technique, plain static mesh (hidden ignored), setup fails: no draws; transparent without diffuse map skipped
    p.Type = 3; g_Technique = 1; g_SetupOk = false; g_Materials[2].DiffuseIndex = 0;
    ok = ok && RenderPrimitiveToShadowBuffer(p) == RENDERED_OBJECT;
    ok = ok && g_Draws == 2 && g_VertexShaders == 5 && g_ConstantUploads == 2 && g_AlphaOff == 3 && g_AlphaOn == 1;
    ok = ok && g_LastStages.Diffuse == 0 && g_LastStages.UseBlank && g_LastMipmap == 0 && g_Flushes == 2;
    if (!ok) { std::printf("00ba6240_TEST FAIL\n"); return 1; }
    std::printf("00ba6240_TEST PASS\n");
    return 0;
}