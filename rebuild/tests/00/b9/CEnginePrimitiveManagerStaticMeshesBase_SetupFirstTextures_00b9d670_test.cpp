// Behaviour test for 0x00B9D670 CEnginePrimitiveManagerStaticMeshesBase::SetupFirstTextures: bind the first static
// block's diffuse / bump D3D textures (null for a missing CTexture) to the material's stages, skipping a -1 stage; the
// core only calls the device when the cached texture changes and raises NumTextureStagesUsed to stage + 1.
#include <cstdio>

static int g_deviceCalls = 0;
static int g_lastStage = -1;
static void* g_lastTexture = 0;

struct CRenderManagerCore {
    long  NumTextureStagesUsed;
    void* CurrentTextures[4];
    void SetTexture(long stage, void* texture)
    {
        if (CurrentTextures[stage] != texture) {
            CurrentTextures[stage] = texture;
            ++g_deviceCalls; g_lastStage = (int)stage; g_lastTexture = texture;
        }
        if (NumTextureStagesUsed < stage + 1)
            NumTextureStagesUsed = stage + 1;
    }
};
static CRenderManagerCore g_core;

struct CTexture { void* PD3DTexture; unsigned long Bits; };
struct CStaticBlock { CTexture* DiffuseTexture; CTexture* BumpTexture; };
struct C3DPrimitive2 { CStaticBlock* StaticBlocks; };
struct C3DTextureStages { long DiffuseTextureStage; long BumpTextureStage; bool UseBlankTextures; };

static void SetupFirstTextures(C3DPrimitive2& primitive, C3DTextureStages& stages)
{
    CStaticBlock* block = primitive.StaticBlocks;
    if (stages.DiffuseTextureStage != -1)
        g_core.SetTexture(stages.DiffuseTextureStage, block->DiffuseTexture ? block->DiffuseTexture->PD3DTexture : 0);
    if (stages.BumpTextureStage != -1)
        g_core.SetTexture(stages.BumpTextureStage, block->BumpTexture ? block->BumpTexture->PD3DTexture : 0);
}

int main()
{
    static int d3dDiffuse, d3dBump;
    CTexture diffuse = { &d3dDiffuse, 0 };
    CTexture bump = { &d3dBump, 0 };
    CStaticBlock block = { &diffuse, &bump };
    C3DPrimitive2 prim = { &block };
    C3DTextureStages stages = { 0, 2, false };
    g_core.NumTextureStagesUsed = 0;
    for (unsigned int i = 0; i < 4; ++i) g_core.CurrentTextures[i] = 0;
    int ok = 1;
    SetupFirstTextures(prim, stages);
    if (g_deviceCalls != 2 || g_core.CurrentTextures[0] != &d3dDiffuse || g_core.CurrentTextures[2] != &d3dBump) ok = 0;
    if (g_core.NumTextureStagesUsed != 3) ok = 0;
    SetupFirstTextures(prim, stages);                        // cached: no device calls
    if (g_deviceCalls != 2) ok = 0;
    block.BumpTexture = 0;                                   // missing texture binds null
    SetupFirstTextures(prim, stages);
    if (g_deviceCalls != 3 || g_lastStage != 2 || g_lastTexture != 0 || g_core.CurrentTextures[2] != 0) ok = 0;
    stages.DiffuseTextureStage = -1; stages.BumpTextureStage = -1; block.DiffuseTexture = 0;
    SetupFirstTextures(prim, stages);                        // both stages unused: untouched
    if (g_deviceCalls != 3 || g_core.CurrentTextures[0] != &d3dDiffuse) ok = 0;
    std::printf(ok ? "00b9d670_TEST PASS\n" : "00b9d670_TEST FAIL\n");
    return ok ? 0 : 1;
}