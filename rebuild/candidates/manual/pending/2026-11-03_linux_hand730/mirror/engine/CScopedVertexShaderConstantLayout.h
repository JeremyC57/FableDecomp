#ifndef FABLE_ENGINE_CSCOPEDVERTEXSHADERCONSTANTLAYOUT_H
#define FABLE_ENGINE_CSCOPEDVERTEXSHADERCONSTANTLAYOUT_H
#include "rebuild_abi.h"
#include "engine/CShaderRenderManager.h"
// New 2026-11-03 (Codex pass 13 row 0x009DA9F0; Ego_r.pdb / FableWin.pdb declarations, Codex evidence/009da9f0_*_types.json).
class CScopedVertexShaderConstantLayout {private:long OldIndex;
public:
 CScopedVertexShaderConstantLayout(long index) {
  CShaderRenderManager& manager=CShaderRenderManager::Get();
  OldIndex=manager.PeekVertexShaderConstantLayout();manager.SetVertexShaderConstantLayout(index);
 }
 ~CScopedVertexShaderConstantLayout() { CShaderRenderManager& manager=CShaderRenderManager::Get();manager.SetVertexShaderConstantLayout(OldIndex); }
};
#endif
