#ifndef FABLE_ENGINE_CSCOPEDVERTEXSHADERCONSTANTLAYOUT_RENDER2D_H
#define FABLE_ENGINE_CSCOPEDVERTEXSHADERCONSTANTLAYOUT_RENDER2D_H
#include "rebuild_abi.h"
#include "engine/CShaderRenderManager.h"
// Original Ego_r and FableWin declarations: evidence/009da9f0_*_types.json and *_dependencies.json.
class CScopedVertexShaderConstantLayout {private:long OldIndex;
public:
 CScopedVertexShaderConstantLayout(long index) {
  CShaderRenderManager& manager=CShaderRenderManager::Get();
  OldIndex=manager.PeekVertexShaderConstantLayout();manager.SetVertexShaderConstantLayout(index);
 }
 ~CScopedVertexShaderConstantLayout() { CShaderRenderManager& manager=CShaderRenderManager::Get();manager.SetVertexShaderConstantLayout(OldIndex); }
};
#endif
