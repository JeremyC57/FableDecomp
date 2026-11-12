// CEngineLandscapeRenderer::RenderSortedPatches() -- render the z-sorted landscape sub-patches. With vertex and pixel shaders forced on
// and a render-state capture block open, compact the sorter list (entries added this frame move down to the next free slot, keep
// their OcclusionID, clear AddedThisFrame and get their new ZSorterHandle; entries not added give their patch ZSorterHandle -1),
// shrink the list to the kept count, set the common states and the faded-shadow lighting flag (ShadowMode 1), render every patch
// (its OcclusionID / IsOcclusionBox are updated in place through the by-reference Render arguments), then clear the render states,
// restore the two shader enables and finally the capture block.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B68DA0, 607 bytes (plain `ret`, two tail-duplicated epilogues).
// Leaf: ?RenderSortedPatches@CEngineLandscapeRenderer@@AAEXXZ
// Flags: none (header opt-ins below)
// Callees: CShaderRenderManager::EnableVertexShaders 0x00987FE0 / DisableVertexShaders 0x009880E0 / EnablePixelShaders 0x00988110 /
//   DisablePixelShaders 0x00988190 (through the inline SetVertexShadersEnable / SetPixelShadersEnable on NEngineInternal::GPShaderRenderManager
//   0x01436E14), CRenderStateManager::RestoreCaptureBlock 0x00A05840 (CRenderStateScopedCaptureBlock dtor; StartCaptureBlock inline on
//   CRenderStateCaptureBlock::StateManager 0x013CB508), vector<CSubPatchSorterEntry>::_M_fill_insert 0x00B6C840 (inline resize(n)),
//   SetupCommonStates 0x00B67480, CEngineLightingManager::UpdateGlobalLightingStates 0x00B46C80 (NEngineInternal::GPLightingManager
//   0x01436E9C; SetEnableFadedShadowFactor inline), CLandscapeBackgroundPatch::Render(bool, long&, bool&) 0x00BE7BE0,
//   PostRenderClearRenderStates 0x00B67510.
// Shape notes: the shader enables are explicit calls (not CScopedShadersEnable, which uses the singleton object 0x013BC470) so the
//   capture block is restored after them; the compaction takes the start pointer once per iteration and has a single `++count` after
//   the move/clear branches (the two `inc ecx` are VC7.1's tail duplication -- two increments in the source give count eax, not ecx);
//   the faded-shadow step is the PDB inline SetEnableFadedShadowFactor(true) (FableWin; out-of-line copy 0x00B66EB0) whose
//   `if (PeekGlobalShadowMode() != 1) enable = false;` guard gives retail `mov al,1 ; je ; xor al,al ; cmp al,[ecx+0CEh]`
//   (header opt-in FABLE_RETAIL_CENGINELIGHTINGMANAGER_SETFADED_SHADOWGUARD).
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_RENDERSORTED
#define FABLE_RETAIL_LANDSCAPE_BACKGROUND_PATCH
#define FABLE_RETAIL_LANDSCAPE_BACKGROUND_PATCH_RENDER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_SETFADED_SHADOWGUARD
#include "engine/CEngineLandscapeRenderer.h"
#include "engine/CLandscapeBackgroundPatch.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"

namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }  // 0x01436E14 ?GPShaderRenderManager@NEngineInternal@@
namespace NEngineInternal { extern CEngineLightingManager* GPLightingManager; }      // 0x01436E9C ?GPLightingManager@NEngineInternal@@3PAVCEngineLightingManager@@A

void CEngineLandscapeRenderer::RenderSortedPatches()
{
    bool oldVertexShaders;
    bool oldPixelShaders;
    NEngineInternal::GPShaderRenderManager->SetVertexShadersEnable(true, &oldVertexShaders);
    NEngineInternal::GPShaderRenderManager->SetPixelShadersEnable(true, &oldPixelShaders);
    CRenderStateScopedCaptureBlock capture;
    long count = 0;
    for (long i = 0; i < SortedSubPatchList.size(); ++i) {
        CSubPatchSorterEntry* entries = SortedSubPatchList.begin();
        CSubPatchSorterEntry& entry = entries[i];
        if (entry.AddedThisFrame) {
            if (i > count) {
                entries[count].Patch = entry.Patch;
                entries[count].OcclusionID = entry.OcclusionID;
                entries[count].AddedThisFrame = 0;
                entries[count].Patch->ZSorterHandle = count;
            } else {
                entry.AddedThisFrame = 0;
            }
            ++count;
        } else if (entry.Patch) {
            entry.Patch->ZSorterHandle = -1;
        }
    }
    SortedSubPatchList.resize(count);
    SetupCommonStates();
    NEngineInternal::GPLightingManager->SetEnableFadedShadowFactor(true);
    NEngineInternal::GPLightingManager->UpdateGlobalLightingStates();
    for (unsigned long j = 0; j < SortedSubPatchList.size(); ++j) {
        CSubPatchSorterEntry& entry = SortedSubPatchList[j];
        long queryHandle = entry.OcclusionID;
        bool occluded = entry.IsOcclusionBox != 0;
        entry.Patch->Render(true, queryHandle, occluded);
        entry.OcclusionID = queryHandle;
        entry.IsOcclusionBox = occluded;
    }
    PostRenderClearRenderStates();
    NEngineInternal::GPShaderRenderManager->SetVertexShadersEnable(oldVertexShaders, 0);
    NEngineInternal::GPShaderRenderManager->SetPixelShadersEnable(oldPixelShaders, 0);
}