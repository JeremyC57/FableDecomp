// CShaderRenderManager::SetTextureTransformCount(long count) -- stores the texture-transform count (+0x38C)
// Leaf: ?SetTextureTransformCount@CShaderRenderManager@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00988630, length 13 bytes (`mov eax,[esp+4] ; mov [ecx+38Ch],eax ; ret 4`).
// Name fix (NF127, 2026-11-20, q4726): Ego_r.pdb 0x00AE4C90 + FableWin.pdb 0x03033610 ?SetTextureTransformCount@CShaderRenderManager@@QAEXJ@Z
//   (engine/CShaderRenderManager.h declares it and TextureTransformCount +0x38C); the one retail caller 0x00B25890 calls it on
//   NEngineInternal::GPShaderRenderManager (0x01436E14) right after SetVertexShaderConstantLayout 0x0098B5E0. Was an invented free
//   __fastcall CObservable::AddExclusiveObserver over a pad struct (catalog _global::SetMember_dword0x38c).
#include "engine/CShaderRenderManager.h"

void CShaderRenderManager::SetTextureTransformCount(long count)
{
    TextureTransformCount = count;
}
