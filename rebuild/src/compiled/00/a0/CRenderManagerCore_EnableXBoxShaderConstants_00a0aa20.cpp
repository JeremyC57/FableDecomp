// CRenderManagerCore::EnableXBoxShaderConstants(bool enable) -- an empty PC stub (`ret 4`).
// Leaf: ?EnableXBoxShaderConstants@CRenderManagerCore@@QAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0AA20, length 3 bytes (`ret 4`).
// Identity (NF133, q4727): Ego_r.pdb 0x00B73FD0 / FableWin.pdb 0x030EAE60 ?EnableXBoxShaderConstants@CRenderManagerCore@@QAEX_N@Z, Ego_r donor
//   order directly after SetMaterial (Ego_r 0x00B73FC0 = retail 0x00A0AA10, the landed `ret 8` stub); one stack argument matches `ret 4`;
//   the callers CShaderRenderManager::EnableVertexShaders 0x00987FE0 (true) / DisableVertexShaders 0x009880E0 (false) pass the flag on
//   the shader manager's RenderManager (+0x1D0). Declared by engine/CRenderManagerCore.h (FABLE_RETAIL_CRENDERMANAGERCORE). Was the
//   placeholder ?M@C00a0aa20@@QAEXH@Z. Parameter name descriptive (the PDB declaration is unnamed).
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_C2DBOXF
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C2DBoxF.h"
#include "engine/CRenderManagerCore.h"

void CRenderManagerCore::EnableXBoxShaderConstants(bool /*enable*/)
{
}
