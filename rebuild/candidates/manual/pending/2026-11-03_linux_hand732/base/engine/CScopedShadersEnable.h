#ifndef FABLE_ENGINE_CSCOPEDSHADERSENABLE_H
#define FABLE_ENGINE_CSCOPEDSHADERSENABLE_H
#include "rebuild_abi.h"
#include "engine/CShaderRenderManager.h"
// New 2026-11-03 (Codex pass 13 row 0x009DA9F0; Ego_r.pdb / FableWin.pdb declarations, Codex evidence/009da9f0_*_types.json).
class CScopedShadersEnable {private:bool OldVertexEnable,OldPixelEnable;
public:
 CScopedShadersEnable(bool vertex,bool pixel) {
  CShaderRenderManager::Get().SetVertexShadersEnable(vertex,&OldVertexEnable);
  CShaderRenderManager::Get().SetPixelShadersEnable(pixel,&OldPixelEnable);
 }
 ~CScopedShadersEnable() {
  CShaderRenderManager::Get().SetVertexShadersEnable(OldVertexEnable,0);
  CShaderRenderManager::Get().SetPixelShadersEnable(OldPixelEnable,0);
 }
};
#endif
