// CRenderManagerCore::AttachTextureToStage(long stage, const CTexture* texture) -- select the texture's D3D surface (or none) on a
// sampler stage through the cached CurrentlySelectedTextures slot, and raise the TextureStagesInUse high-water mark.
// Leaf: ?AttachTextureToStage@CRenderManagerCore@@QAEXJPBVCTexture@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A0CF0, thiscall, `ret 8`: the out-of-line COMDAT copy of the header inline that
//   CRenderManager2D::Render2DDrawList 0x009DA9F0 and CGPUScratchBufferTextureManager::BeginShutdown 0x00F37170 call.
// Naming (NF129, 2026-11-20, q4547): Ego_r.pdb + FableWin.pdb ?AttachTextureToStage@CRenderManagerCore@@QAEXJPBVCTexture@@@Z;
//   the body is the private inline AttachD3DTextureToStage (engine/CRenderManagerCore.h, retail branch).
#define FABLE_RETAIL_CRENDERMANAGERCORE
#include "engine/CRenderManagerCore.h"

void CRenderManagerCore::AttachTextureToStage(long stage, const CTexture* texture)
{
    AttachD3DTextureToStage(stage, texture ? (IDirect3DBaseTexture9*)texture->PD3DTexture : 0);
}
