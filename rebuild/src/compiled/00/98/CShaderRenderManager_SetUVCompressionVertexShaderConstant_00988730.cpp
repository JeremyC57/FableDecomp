// CShaderRenderManager::SetUVCompressionVertexShaderConstant(float, float, float, float) -- empty on PC (`ret 10h`)
// Leaf: ?SetUVCompressionVertexShaderConstant@CShaderRenderManager@@QAEXMMMM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00988730, length 3 bytes (`ret 10h`).
// Name fix (NF127, 2026-11-20, q4728): Ego_r.pdb 0x00AE4DA0 + FableWin.pdb 0x03035050
//   ?SetUVCompressionVertexShaderConstant@CShaderRenderManager@@QAEXMMMM@Z (engine/CShaderRenderManager.h declares it); the one retail
//   caller 0x00B258C6 pushes four floats and calls it on the shader manager (no other call or absolute reference: not an ICF fold of
//   another empty `ret 10h` body). Was the placeholder stand-in C00988730::M (catalog _global::Nop_ret16_00988730, file E2_E2_00988730.cpp).
#include "engine/CShaderRenderManager.h"

void CShaderRenderManager::SetUVCompressionVertexShaderConstant(float, float, float, float)
{
}
