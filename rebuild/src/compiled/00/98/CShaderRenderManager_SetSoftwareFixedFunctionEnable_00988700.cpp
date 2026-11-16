// CShaderRenderManager::SetSoftwareFixedFunctionEnable(bool enable) -- stores the software fixed-function flag
// Leaf: ?SetSoftwareFixedFunctionEnable@CShaderRenderManager@@QAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00988700, length 13 bytes (`mov al,[esp+4] ; mov [ecx+394h],al ; ret 4`).
// Name fix (NF34, 2026-11-16): FableWin.pdb + Ego_r.pdb ?SetSoftwareFixedFunctionEnable@CShaderRenderManager@@QAEX_N@Z (engine/CShaderRenderManager.h
// declares it); caller 0x00B346A2. Was a placeholder stand-in class (C00988700::M).
#include "engine/CShaderRenderManager.h"

void CShaderRenderManager::SetSoftwareFixedFunctionEnable(bool enable)
{
    SoftwareFixedFunctionEnable = enable;
}
