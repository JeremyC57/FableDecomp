// CEngineLightingManager::CalcSWLightingNoClipSeparateMainLightVector(CRGBFloatColour* colours, CRGBFloatColour* mainLightColours, const C4DVector* positions, const float* normalX, const float* normalY, const float* normalZ, long count, const CEngineLocalLightArray& lights) -- the CalcSWLightingNoClipVector SSE loop with the main (diffuse) light written to its own colour stream: mainLightColours gets diffuse * N.L, colours gets backlight + ambient + every local light's attenuated term, both unclipped
// Grade: MATCH (627 bytes)
// Flags: none
// Leaf: ?CalcSWLightingNoClipSeparateMainLightVector@CEngineLightingManager@@QAEXPAVCRGBFloatColour@@0PBVC4DVector@@PBM22JABVCEngineLocalLightArray@@@Z
// Retail-asm: 0x00B44A79..0x00B44CC9 (`movaps` / `mulps` / `rcpps` vertex loop) -- Lionhead's hand-written SSE block, as in
//   CalcSWLightingNoClipVector 0x00B44820: 0x00B44A79 `mov esi,0`, `prefetcht0 [eax+esi+20h]` (0x00B44A94), `subps xmm1,xmm1`
//   zeroing (0x00B44AC3), the `shufps` transposes (0x00B44AF1..0x00B44B1D), `rsqrtps` / `rcpps` in the light loop and the
//   in-block `push ebx` / `pop ebx` (0x00B44B55 / 0x00B44C88), in a frame with no `and esp,-16`.
// Helper (2026-11-05 asm round 2): the block is the shared engine/CEngineLightingSSE.h
//   (GF_SSE_CALC_SW_LIGHTING_NO_CLIP_SEPARATE_MAIN_LIGHT: byte-matching path + FABLE_PORTABLE twin with exact 1/sqrt and 1/x,
//   checked within the rsqrtps / rcpps bound); the row has no asm of its own. Plain C++ and intrinsic attempts (none
//   reproduces it): rebuild/candidates/manual/naming/2026-11-05_asm_helpers_r2/ATTEMPTS_LIGHTING.tsv.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B44A60, 627 bytes, ebp frame, `ret 20h`.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?CalcSWLightingNoClipSeparateMainLightVector@CEngineLightingManager@@QAEXPAVCRGBFloatColour@@0PBVC4DVector@@PBM22JABVCEngineLocalLightArray@@@Z
//   -- eight arguments (`ret 20h`), two CRGBFloatColour streams (+8, +0Ch) each written at +0x40 per four vertices, the C4DVector
//   stream, three float streams and the local light array; donor order puts it right after CalcSWLightingNoClipVector, as retail.
// C++ around the block: `count /= 4` into the argument slot and the `this` home at [ebp-4] read by `mov eax,this`; the SIMD
//   var block pointer is CEngineLightingManager::SIMDVarBlock (+0x4690, engine/CEngineLightingManager.h).
#include "engine/CEngineLightingSSE.h"
#define CEngineLightingManager CEngineLightingManagerLayout
#include "engine/CEngineLightingManager.h"
#undef CEngineLightingManager
class CRGBFloatColour;
class C4DVector;
class CEngineLocalLightArray;
class CEngineLightingManager : public CEngineLightingManagerLayout {
public:
    void CalcSWLightingNoClipSeparateMainLightVector(CRGBFloatColour* colours, CRGBFloatColour* mainLightColours,
                                                     const C4DVector* positions, const float* normalX, const float* normalY,
                                                     const float* normalZ, long count, const CEngineLocalLightArray& lights);
};

void CEngineLightingManager::CalcSWLightingNoClipSeparateMainLightVector(CRGBFloatColour* colours, CRGBFloatColour* mainLightColours,
                                                                         const C4DVector* positions, const float* normalX,
                                                                         const float* normalY, const float* normalZ, long count,
                                                                         const CEngineLocalLightArray& lights)
{
    count /= 4;
    GF_SSE_CALC_SW_LIGHTING_NO_CLIP_SEPARATE_MAIN_LIGHT(this, colours, mainLightColours, positions, normalX, normalY, normalZ, count, lights);
}