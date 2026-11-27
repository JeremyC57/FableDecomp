// 00b62440 CEngineSkyRenderer::WorldFrameUpdate
// Leaf: ?WorldFrameUpdate@CEngineSkyRenderer@@UAEX_N@Z
// Flags: (none)
#define FABLE_RETAIL_SKY_WORLDFRAME_PASS29
#define FABLE_RETAIL_CENGINESKYRENDERER
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_GAMEWORLD
#define FABLE_RETAIL_CENGINEENVIRONMENT
#define FABLE_RETAIL_CENGINEENVIRONMENT_WIND
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C3DVECTOR
#include "engine/CEngineSkyRenderer.h"
#include "engine/CEngineWorldMap.h"
#include "engine/CEngineEnvironment.h"
#include "engine/CIDrawWorld.h"
#include "engine/CIDrawEnvironment.h"

namespace NEngineInternal {
    extern CEngineWorldMap* GPEngineWorldMap;
    extern CEngineEnvironment* GPEngineEnvironment;
}

void CEngineSkyRenderer::KeepHistoryInRange(C2DVector* history, const float range) const
{
    if (history[0].X > range) {
        history[0].X -= range;
        history[1].X -= range;
    } else if (history[0].X < -range) {
        history[0].X += range;
        history[1].X += range;
    }
    if (history[0].Y > range) {
        history[0].Y -= range;
        history[1].Y -= range;
    } else if (history[0].Y < -range) {
        history[0].Y += range;
        history[1].Y += range;
    }
}

void CEngineSkyRenderer::WorldFrameUpdate(bool time_for_full_update)
{
    if (time_for_full_update) {
        const CIDrawEnvironmentParams& params = NEngineInternal::GPEngineWorldMap->PeekGameWorld()->DrawGetEnvironment()->PeekInterpolatedParams();
        float speed = CloudSpeedMultiplier * params.ThemeChannels[0].Scalers[1];
        C3DVector wind_vel = NEngineInternal::GPEngineEnvironment->GetClampedScaledCurrentWindVector(speed, CloudMinSpeed, CloudMaxSpeed);
        CloudVectorHistory[1][1] = CloudVectorHistory[1][0];
        CloudVectorHistory[1][0] += C2DVector(wind_vel);
        speed = CloudSpeedMultiplier * params.ThemeChannels[0].Scalers[0];
        wind_vel = NEngineInternal::GPEngineEnvironment->GetClampedScaledCurrentWindVector(speed, CloudMinSpeed, CloudMaxSpeed);
        CloudVectorHistory[0][1] = CloudVectorHistory[0][0];
        CloudVectorHistory[0][0] += C2DVector(wind_vel);
        KeepHistoryInRange(CloudVectorHistory[1], 1.0f);
        KeepHistoryInRange(CloudVectorHistory[0], 1.0f);
    }
}