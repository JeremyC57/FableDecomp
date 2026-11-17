// CRenderManagerCore::SetMaterial(const CRGBColour& diffuse, const CRGBColour& ambient) -- an empty PC stub (`ret 8`).
// Leaf: ?SetMaterial@CRenderManagerCore@@QAEXABVCRGBColour@@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0AA10, length 3 bytes (`ret 8`).
// Identity (NF56, round 7, queue q10415): Ego_r.pdb 0x00B73FC0 / FableWin.pdb 0x030EA8A0 ?SetMaterial@CRenderManagerCore@@QAEXABVCRGBColour@@0@Z,
//   Ego_r donor order directly before EnableXBoxShaderConstants (Ego_r 0x00B73FD0 = retail 0x00A0AA20, the `ret 4` stub); two stack arguments
//   match `ret 8`; the landed caller CEnginePrimitiveRenderer::InitialiseGlobalLights 0x00B32300 already binds this spelling through
//   engine/CRenderManagerCore.h FABLE_RETAIL_CRENDERMANAGERCORE_SETMATERIAL. Was the placeholder ?M@C00a0aa10@@QAEXHH@Z.
//   Parameter names are descriptive (the PDB declaration is unnamed).
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_SETMATERIAL
#define FABLE_RETAIL_C2DBOXF
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C2DBoxF.h"
#include "engine/CRenderManagerCore.h"

void CRenderManagerCore::SetMaterial(const CRGBColour& /*diffuse*/, const CRGBColour& /*ambient*/)
{
}
