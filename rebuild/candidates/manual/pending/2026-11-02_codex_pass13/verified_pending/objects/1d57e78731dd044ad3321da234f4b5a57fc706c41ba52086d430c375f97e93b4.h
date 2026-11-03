#ifndef FABLE_ENGINE_CSCOPEDSHADERSENABLE_RENDER2D_H
#define FABLE_ENGINE_CSCOPEDSHADERSENABLE_RENDER2D_H
#include "rebuild_abi.h"
#include "engine/CShaderRenderManager.h"
// Original Ego_r and FableWin declarations: evidence/009da9f0_*_types.json and *_dependencies.json.
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
