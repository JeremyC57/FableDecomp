// CShaderRenderManager::SetSoftwareVertexShaderEnable(bool enable) -- stores the software vertex shader flag
// Leaf: ?SetSoftwareVertexShaderEnable@CShaderRenderManager@@QAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009886F0, length 13 bytes (`mov al,[esp+4] ; mov [ecx+393h],al ; ret 4`).
// Name fix (NF34, 2026-11-16): FableWin.pdb + Ego_r.pdb ?SetSoftwareVertexShaderEnable@CShaderRenderManager@@QAEX_N@Z (engine/CShaderRenderManager.h
// declares it); callers 0x00B34696 / 0x00B346C0. Was a placeholder free function (EnableNavigator).
#include "engine/CShaderRenderManager.h"

void CShaderRenderManager::SetSoftwareVertexShaderEnable(bool enable)
{
    SoftwareVertexShaderEnable = enable;
}
