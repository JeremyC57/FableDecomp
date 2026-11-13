#ifndef FABLE_ENGINE_CSCOPEDSHADERSENABLE_H
#define FABLE_ENGINE_CSCOPEDSHADERSENABLE_H
#include "rebuild_abi.h"
#include "engine/CShaderRenderManager.h"
#ifdef FABLE_RETAIL_CSCOPEDSHADERSENABLE_FORCEINLINE
#include "fable_compiler.h"
#endif
// New 2026-11-03 (Codex pass 13 row 0x009DA9F0; Ego_r.pdb / FableWin.pdb declarations, Codex evidence/009da9f0_*_types.json).
class CScopedShadersEnable {private:bool OldVertexEnable,OldPixelEnable;
public:
 CScopedShadersEnable(bool vertex,bool pixel) {
  CShaderRenderManager::Get().SetVertexShadersEnable(vertex,&OldVertexEnable);
  CShaderRenderManager::Get().SetPixelShadersEnable(pixel,&OldPixelEnable);
 }
#ifdef FABLE_RETAIL_CSCOPEDSHADERSENABLE_FORCEINLINE
 // Per-TU opt-in (bwf2, row 0x00B72180 CEngineWaterRenderer::CreateDynamicTextures): the destructor is expanded in place at
 // 0x00B7369C..0x00B736DE (compare with 0x013BC800 / 0x013BC801, Enable / Disable calls) past the plain-inline budget.
 FABLE_FORCEINLINE
#endif
 ~CScopedShadersEnable() {
  CShaderRenderManager::Get().SetVertexShadersEnable(OldVertexEnable,0);
  CShaderRenderManager::Get().SetPixelShadersEnable(OldPixelEnable,0);
 }
};
#endif
