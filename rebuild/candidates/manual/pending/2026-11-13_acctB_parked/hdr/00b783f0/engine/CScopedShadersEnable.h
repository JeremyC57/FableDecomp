#ifndef FABLE_ENGINE_CSCOPEDSHADERSENABLE_H
#define FABLE_ENGINE_CSCOPEDSHADERSENABLE_H
#include "rebuild_abi.h"
#include "engine/CShaderRenderManager.h"
// New 2026-11-03 (Codex pass 13 row 0x009DA9F0; Ego_r.pdb / FableWin.pdb declarations, Codex evidence/009da9f0_*_types.json).
class CScopedShadersEnable {private:bool OldVertexEnable,OldPixelEnable;
public:
#ifdef FABLE_RETAIL_CSCOPEDSHADERSENABLE_FORCEINLINE
 // Opt-in (bwf2, row 0x00B783F0 CEngineWaterRenderer::Render): the row expands both members (0x00B785BF, 0x00B7A818) although
 // retail has out-of-line copies (0x009D8AD0 / 0x009D8B40); with plain inline the 9 KB row runs out of /Ob2 budget at the dtor.
 FABLE_FORCEINLINE CScopedShadersEnable(bool vertex,bool pixel) {
  CShaderRenderManager::Get().SetVertexShadersEnable(vertex,&OldVertexEnable);
  CShaderRenderManager::Get().SetPixelShadersEnable(pixel,&OldPixelEnable);
 }
 FABLE_FORCEINLINE ~CScopedShadersEnable() {
  CShaderRenderManager::Get().SetVertexShadersEnable(OldVertexEnable,0);
  CShaderRenderManager::Get().SetPixelShadersEnable(OldPixelEnable,0);
 }
#else
 CScopedShadersEnable(bool vertex,bool pixel) {
  CShaderRenderManager::Get().SetVertexShadersEnable(vertex,&OldVertexEnable);
  CShaderRenderManager::Get().SetPixelShadersEnable(pixel,&OldPixelEnable);
 }
 ~CScopedShadersEnable() {
  CShaderRenderManager::Get().SetVertexShadersEnable(OldVertexEnable,0);
  CShaderRenderManager::Get().SetPixelShadersEnable(OldPixelEnable,0);
 }
#endif
};
#endif
