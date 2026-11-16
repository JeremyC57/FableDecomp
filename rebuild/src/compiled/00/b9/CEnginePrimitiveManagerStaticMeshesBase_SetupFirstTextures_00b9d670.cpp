// CEnginePrimitiveManagerStaticMeshesBase::SetupFirstTextures(C3DPrimitive2& primitive, C3DTextureStages& textureStages) -- bind the first static block's diffuse and bump textures to the stages the material uses (a stage of -1 is skipped), through the render core's cached SetTexture
// Leaf: ?SetupFirstTextures@CEnginePrimitiveManagerStaticMeshesBase@@SIXAAVC3DPrimitive2@@AAVC3DTextureStages@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B9D670, 179 bytes, static __fastcall (primitive ecx, textureStages edx), `ret`.
// Identity: Ego_r.pdb ?SetupFirstTextures@CEnginePrimitiveManagerStaticMeshesBase@@SIXAAVC3DPrimitive2@@AAVC3DTextureStages@@@Z;
//   callers CEngineBillboardGenerator::RenderBillboard 0x00B8C033, CEnginePrimitiveManagerStaticMeshesBase::
//   RenderMeshPrimitive 0x00BA5E8F and ::RenderPrimitiveToShadowBuffer 0x00BA6CA1. Catalog name agrees.
// Callees: IDirect3DDevice9::SetTexture (vtable slot 65, +0x104) through the inline CRenderManagerCore::SetTexture.
// Layout: C3DPrimitive2::StaticBlocks at retail +0x44 (Ego_r +0x48, less 4 for the 12-byte ClothPrimitives CArray);
//   CStaticBlock (Ego_r.pdb) DiffuseTexture +0x14, BumpTexture +0x18; CTexture::PD3DTexture +0; C3DTextureStages
//   {DiffuseTextureStage, BumpTextureStage, UseBlankTextures}; g_pRenderManagerCore 0x01436E18 (CurrentTextures +0x3D00,
//   NumTextureStagesUsed +0x3CB8, PD3DDevice +0x3CF0) as in the landed CEngine::Uninitialise 0x00B25D80.
// Shape: two inlined cached SetTexture calls (the core pointer re-read after each device call); each texture is
//   `texture ? texture->PD3DTexture : 0` inside a __forceinline CRenderManagerCore::SetTexture(long, const CTexture*)
//   overload (the landed 0x00BA7DB0 device) with the -1 stage tests outside it: that loads g_pRenderManagerCore at entry
//   and names block/stages ebx/ebp as retail; the select written in this body loads the core after it.
#include "fable_compiler.h"
// NF45 (round 7): the render core is NEngineInternal::GPRenderManager 0x01436E18 (Ego_r.pdb / FableWin.pdb
//   ?GPRenderManager@NEngineInternal@@3PAVCRenderManager@@A, engine/CRenderManager.h; CRenderManagerCore at offset 0) and the texture
//   binds are the PDB inline ?AttachTextureToStage@CRenderManagerCore@@QAEXJPBVCTexture@@@Z expanded in place (repo opt-in
//   FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS; AttachD3DTextureToStage: CurrentlySelectedTextures +0x3D00, PD3DDevice +0x3CF0,
//   TextureStagesInUse +0x3CB8) on the shared CTexture / IDirect3DDevice9 (was a local CRenderManagerCore with two SetTexture
//   overloads, local CTexture / IDirect3DDevice9 models and g_pRenderManagerCore).
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#include "engine/CRenderManager.h"
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }   // 0x01436E18

class CStaticBlock {                               // Ego_r.pdb, 32 bytes
public:
    unsigned char _pad00[0xc];
    unsigned long DiffuseTextureFixupOffset;       // +0x0c
    unsigned long BumpTextureFixupOffset;          // +0x10
    CTexture*     DiffuseTexture;                  // +0x14
    CTexture*     BumpTexture;                     // +0x18
    long          MaterialIndex;                   // +0x1c
};

class C3DPrimitive2 {
public:
    unsigned char _pad00[0x44];
    CStaticBlock* StaticBlocks;                    // +0x44
};

class C3DTextureStages {                           // Ego_r.pdb, 12 bytes
public:
    long DiffuseTextureStage;                      // +0x00
    long BumpTextureStage;                         // +0x04
    bool UseBlankTextures;                         // +0x08
};

class CEnginePrimitiveManagerStaticMeshesBase {
public:
    static void __fastcall SetupFirstTextures(C3DPrimitive2& primitive, C3DTextureStages& textureStages);
};

void __fastcall CEnginePrimitiveManagerStaticMeshesBase::SetupFirstTextures(C3DPrimitive2& primitive, C3DTextureStages& textureStages)
{
    CStaticBlock* block = primitive.StaticBlocks;
    if (textureStages.DiffuseTextureStage != -1)
        NEngineInternal::GPRenderManager->AttachTextureToStage(textureStages.DiffuseTextureStage, block->DiffuseTexture);
    if (textureStages.BumpTextureStage != -1)
        NEngineInternal::GPRenderManager->AttachTextureToStage(textureStages.BumpTextureStage, block->BumpTexture);
}