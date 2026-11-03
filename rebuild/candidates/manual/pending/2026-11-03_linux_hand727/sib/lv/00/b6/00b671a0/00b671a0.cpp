// CEngineLandscapeRenderer::SetupBackgroundStates() -- background landscape lighting: enable the lighting manager's faded-shadow factor exactly when its shadow mode is 1, then update its global lighting states
// Leaf: ?SetupBackgroundStates@CEngineLandscapeRenderer@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B671A0, 58 bytes (int3 padding to the next function 0x00B671E0), thiscall (this unused:
//   both statements go through the global lighting manager), tail jmp UpdateGlobalLightingStates.
// Identity (proven, two sources): (1) Ego_r.pdb donor order of CEngineLandscapeRenderer -- CreateMap / DeleteMap /
//   SetupBackgroundStates / ... / CalcForegroundFadeFactors -- maps one-to-one onto retail 0x00B67140 / 0x00B67180 / 0x00B671A0 /
//   0x00B671E0; the only caller is CEngineLandscapeRenderer::Render (`mov ecx,edi` = this renderer, a member call); private AAE
//   per Ego_r.pdb; (2) FableWin.pdb carries the inline CEngineLightingManager::SetEnableFadedShadowFactor(bool) this body
//   expands, and its out-of-line copy at retail 0x00B66EB0 (0 callers, uncatalogued) is the same instruction sequence
//   (PeekGlobalShadowMode() != 1 -> enable = false; compare with FadedShadowDiffuseFactorEnabled; store and StateUpdateFlags |= 1).
//   The catalog label "_global::UpdateLighting" is a stand-in.
// Callees: CEngineLightingManager::UpdateGlobalLightingStates() 0x00B46C80 (tail; bound spelling correct).
// Globals: 0x01436E9C, the engine lighting manager pointer. Its spelling is NOT proven by two sources (the PDB name sets carry no
//   data symbol here); `g_pLightingManager` follows the bound `?g_pLightingManager@@3PAVCEngineLightingManager@@A`, one of ~8
//   bound variants for this address -- coordinator decision.
// Layout: engine/CEngineLightingManager.h and engine/CEngineLandscapeRenderer.h are flattened (vptr as data), so both are included
//   under the layout-rename pattern and the class is restated over the Fields struct with sizeof asserted equal. Members used
//   (Ego_r.pdb): FadedShadowDiffuseFactorEnabled +0xCE, StateUpdateFlags +0xE4, ShadowMode +0x4694.
// Shape: the PDB inline setter verbatim (by value bool parameter reassigned), called with `true`; the global reloaded for the
//   second call as retail does (no common load across the inline store).
#define CEngineLightingManager CEngineLightingManagerFields      // engine/CEngineLightingManager.h is flattened (vptr as data): layout-rename include
#include "engine/CEngineLightingManager.h"
#undef CEngineLightingManager
#define CEngineLandscapeRenderer CEngineLandscapeRendererFields  // engine/CEngineLandscapeRenderer.h is flattened too
#include "engine/CEngineLandscapeRenderer.h"
#undef CEngineLandscapeRenderer

enum ELightingManagerShadowMode { };                              // Ego_r.pdb enum (enumerator names not in the name sets)

class CEngineLightingManager : public CEngineLightingManagerFields {
public:
    ELightingManagerShadowMode PeekGlobalShadowMode() const { return *(const ELightingManagerShadowMode*)ShadowMode; }   // FableWin QBE (inline)
    void SetEnableFadedShadowFactor(bool enable)                  // FableWin ?SetEnableFadedShadowFactor@CEngineLightingManager@@QAEX_N@Z (inline; out-of-line copy 0x00B66EB0)
    {
        if (PeekGlobalShadowMode() != (ELightingManagerShadowMode)1)
            enable = false;
        if (enable != FadedShadowDiffuseFactorEnabled) {
            FadedShadowDiffuseFactorEnabled = enable;
            StateUpdateFlags |= 1;
        }
    }
    void UpdateGlobalLightingStates();                            // 0x00B46C80
};
FABLE_STATIC_ASSERT(sizeof(CEngineLightingManager) == sizeof(CEngineLightingManagerFields));

extern CEngineLightingManager* g_pLightingManager;                // 0x01436E9C

class CEngineLandscapeRenderer : public CEngineLandscapeRendererFields {
private:
    void SetupBackgroundStates();
};
FABLE_STATIC_ASSERT(sizeof(CEngineLandscapeRenderer) == sizeof(CEngineLandscapeRendererFields));

void CEngineLandscapeRenderer::SetupBackgroundStates()
{
    g_pLightingManager->SetEnableFadedShadowFactor(true);
    g_pLightingManager->UpdateGlobalLightingStates();
}