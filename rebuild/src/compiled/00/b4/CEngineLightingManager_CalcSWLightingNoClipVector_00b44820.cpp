// CEngineLightingManager::CalcSWLightingNoClipVector(CRGBFloatColour* colours, const C4DVector* positions, const float* normalX, const float* normalY, const float* normalZ, long count, const CEngineLocalLightArray& lights) -- SSE software lighting of a vertex stream four at a time: global diffuse / backlight / ambient from the SIMD var block, then every local light's attenuated N.L term, colours written unclipped
// Grade: MATCH (572 bytes)
// Flags: none
// Leaf: ?CalcSWLightingNoClipVector@CEngineLightingManager@@QAEXPAVCRGBFloatColour@@PBVC4DVector@@PBM22JABVCEngineLocalLightArray@@@Z
// Retail-asm: 0x00B44839..0x00B44A52 -- the whole vertex loop is Lionhead's hand-written SSE block: 0x00B44839 `mov esi,0`,
//   0x00B44853 `movaps xmm0,[eax+esi]`, 0x00B44857 `prefetcht0 [eax+esi+20h]`, `rsqrtps xmm1,xmm3` (0x00B449AF), `rcpps xmm3,xmm3`
//   (0x00B449C4), `subps xmm1,xmm1` zeroing, `shufps` transposes and an in-block `push ebx ; push edi` / `pop edi ; pop ebx`
//   (0x00B448EA / 0x00B44A14) around the light loop, in a frame with no `and esp,-16`.
// Helper (2026-11-05 asm round 2): the block is the shared engine/CEngineLightingSSE.h (GF_SSE_CALC_SW_LIGHTING_NO_CLIP:
//   byte-matching path + FABLE_PORTABLE twin with exact 1/sqrt and 1/x, checked within the rsqrtps / rcpps bound); the row
//   has no asm of its own. Plain C++ and intrinsic attempts (none reproduces it):
//   rebuild/candidates/manual/naming/2026-11-05_asm_helpers_r2/ATTEMPTS_LIGHTING.tsv.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B44820, 572 bytes, ebp frame, `ret 1Ch`.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?CalcSWLightingNoClipVector@CEngineLightingManager@@QAEXPAVCRGBFloatColour@@PBVC4DVector@@PBM22JABVCEngineLocalLightArray@@@Z
//   -- seven arguments (`ret 1Ch`), the colour array written at +0x40 per four vertices, three float streams stepped by 16 bytes,
//   the C4DVector stream by 64, the local light array {count, lights of 0x34 bytes} by reference; donor order puts it first of the
//   CalcSWLighting family, as retail 0x00B44820 before ...SeparateMainLightVector 0x00B44A60.
// C++ around the block: `count /= 4` (cdq / and edx,3 / sar 2 into the argument slot) and the `this` home at [ebp-4] that the
//   block's `mov eax,this` reads; the SIMD var block pointer is CEngineLightingManager::SIMDVarBlock (+0x4690,
//   engine/CEngineLightingManager.h).
#include "engine/CEngineLightingSSE.h"
#define CEngineLightingManager CEngineLightingManagerLayout
#include "engine/CEngineLightingManager.h"
#undef CEngineLightingManager
class CRGBFloatColour;
class C4DVector;
class CEngineLocalLightArray;
class CEngineLightingManager : public CEngineLightingManagerLayout {
public:
    void CalcSWLightingNoClipVector(CRGBFloatColour* colours, const C4DVector* positions, const float* normalX, const float* normalY,
                                    const float* normalZ, long count, const CEngineLocalLightArray& lights);
};

void CEngineLightingManager::CalcSWLightingNoClipVector(CRGBFloatColour* colours, const C4DVector* positions, const float* normalX,
                                                        const float* normalY, const float* normalZ, long count,
                                                        const CEngineLocalLightArray& lights)
{
    count /= 4;
    GF_SSE_CALC_SW_LIGHTING_NO_CLIP(this, colours, positions, normalX, normalY, normalZ, count, lights);
}