// 00b27d90 CEngine::Render
// Leaf: ?Render@CEngine@@UAEXABVCEngineInterpolationInfo@@K@Z
// Flags: (none)
#define FABLE_RETAIL_CENGINE
#define FABLE_RETAIL_CIENGINE_RENDER
#define FABLE_RETAIL_CENGINE_RENDER_PASS26
#include "engine/CEngine.h"
#include "engine/CRenderManager.h"
#include "engine/CScopedShadersEnable.h"

namespace NEngineInternal {
    extern CRenderManager* GPRenderManager;
}

// Inline-budget: plain inline leaves a call; the 00b27db6 device reset is expanded in the retail caller.
FABLE_FORCEINLINE void CRenderManagerCore::FlushRender()
{
    PD3DDevice->SetStreamSource(0, 0, 0, 0);
    PD3DDevice->SetIndices(0);
    for (long c_stage = 0; c_stage < TextureStagesInUse; ++c_stage) {
        if (CurrentlySelectedTextures[c_stage]) {
            CurrentlySelectedTextures[c_stage] = 0;
            PD3DDevice->SetTexture(c_stage, 0);
        }
    }
    TextureStagesInUse = 0;
}

void CEngine::Render(const CEngineInterpolationInfo& interpolation, unsigned long render_flags)
{
    if (EnableEngine) {
        UpdateOffGameTickProcessInfo((render_flags & 1) != 0);
        NEngineInternal::GPRenderManager->FlushRender();
        CScopedShadersEnable shaders_enable(true, true);
        if (NewSceneFlag || CutscenePreloadFlag || CameraAngleChanged)
            LoadNewScene(interpolation);
        RenderFrame(interpolation);
    }
}