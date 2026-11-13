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
#ifdef FABLE_RETAIL_CSCOPEDSHADERSENABLE_FORCEINLINE_DTOR
 // Overlay opt-in (row 0x00BAA870 RenderFromSortList): retail expands the destructor there (0x00BABA08..0x00BABA5B, both restores
 // inline, the pixel-shader one tail-duplicated into the epilogue) although the 4600-byte body is past VC7.1's plain-inline budget.
 FABLE_FORCEINLINE ~CScopedShadersEnable() {
#else
 ~CScopedShadersEnable() {
#endif
  CShaderRenderManager::Get().SetVertexShadersEnable(OldVertexEnable,0);
  CShaderRenderManager::Get().SetPixelShadersEnable(OldPixelEnable,0);
 }
};
#endif
