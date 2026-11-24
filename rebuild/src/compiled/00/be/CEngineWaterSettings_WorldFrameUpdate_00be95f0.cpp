// 00be95f0 CEngineWaterSettings::WorldFrameUpdate
// Leaf: ?WorldFrameUpdate@CEngineWaterSettings@@QAEX_N@Z
// Flags: /d2inlT105
// Inline-budget: native 00BE95F0 expands both next-wind queries but calls the four vector-history adjustments.
#define FABLE_RETAIL_CENGINEENVIRONMENT
#define FABLE_RETAIL_CENGINEENVIRONMENT_WIND
#define FABLE_RETAIL_CENGINEENVIRONMENT_NEXT_WIND_PASS26
#define FABLE_RETAIL_CENGINEWATERSETTINGS_TYPED
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_TO2D
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#include "engine/CEngineEnvironment.h"
#include "engine/CEngineWaterSettings.h"
namespace NEngineInternal { extern CEngineEnvironment* GPEngineEnvironment; }
void CEngineWaterSettings::KeepHistoryInRange(float* history,float range) const
{
    if (history[0] > range) {
        history[0] -= range;
        history[1] -= range;
    } else if (history[0] < -range) {
        history[0] += range;
        history[1] += range;
    }
}
void CEngineWaterSettings::KeepHistoryInRange(C2DVector* history, float range) const
{
    if (history[0].X > range) {
        history[0].X -= range;
        history[1].X -= range;
    } else if (-range > history[0].X) {
        history[0].X += range;
        history[1].X += range;
    }
    if (history[0].Y > range) {
        history[0].Y -= range;
        history[1].Y -= range;
    } else if (-range > history[0].Y) {
        history[0].Y += range;
        history[1].Y += range;
    }
}

void CEngineWaterSettings::WorldFrameUpdate(bool time_for_full_update)
{
    if (!time_for_full_update && !NEngineInternal::GPEngineEnvironment->PeekEnvironmentalEffectsAlwaysUpdate())
        return;
    IceBumpMapPCFrameOffset[1] = IceBumpMapPCFrameOffset[0];
    WaterBumpMapPCFrameOffset[1] = WaterBumpMapPCFrameOffset[0];
    SeaBumpMapPCFrameOffset[1] = SeaBumpMapPCFrameOffset[0];
    SwellTrigIncrimentHistory[1] = SwellTrigIncrimentHistory[0];
    IceBumpMapPCFrameOffset[0] += IceBumpMapPCFrameOffsetSpeed;
    WaterBumpMapPCFrameOffset[0] += WaterBumpMapPCFrameOffsetSpeed;
    SeaBumpMapPCFrameOffset[0] += SeaBumpMapPCFrameOffsetSpeed;
    SwellTrigIncrimentHistory[0] += SwellOscilationSpeed;
    BumpmapGenerationShiftHistory[1][0][1] = BumpmapGenerationShiftHistory[1][0][0];
    BumpmapGenerationShiftHistory[1][1][1] = BumpmapGenerationShiftHistory[1][1][0];
    BumpmapGenerationShiftHistory[0][0][1] = BumpmapGenerationShiftHistory[0][0][0];
    BumpmapGenerationShiftHistory[0][1][1] = BumpmapGenerationShiftHistory[0][1][0];

    C2DVector wind_dir_0 = NEngineInternal::GPEngineEnvironment->GetClampedScaledNextWindVector(BumpMapSpeed0 * FlowSpeedFactor, LakeMinimumFlowSpeed, LakeMaximumFlowSpeed).ToC2DVector();
    BumpmapGenerationShiftHistory[0][0][0] += wind_dir_0;
    BumpmapGenerationShiftHistory[1][0][0] += wind_dir_0;
    C2DVector wind_dir_1 = NEngineInternal::GPEngineEnvironment->GetClampedScaledNextWindVector(BumpMapSpeed1 * FlowSpeedFactor, LakeMinimumFlowSpeed, LakeMaximumFlowSpeed).ToC2DVector();
    BumpmapGenerationShiftHistory[1][1][0] += wind_dir_1;
    BumpmapGenerationShiftHistory[0][1][0] += wind_dir_1;
    EdgeWakeOffsetHistory[1] = EdgeWakeOffsetHistory[0];
    EdgeWakeOffsetHistory[0] += EdgeWakeSpeed;
    KeepHistoryInRange(SwellTrigIncrimentHistory,6.2831855f);
    KeepHistoryInRange(BumpmapGenerationShiftHistory[1][0],1.0f);
    KeepHistoryInRange(BumpmapGenerationShiftHistory[1][1],1.0f);
    KeepHistoryInRange(BumpmapGenerationShiftHistory[0][0],1.0f);
    KeepHistoryInRange(BumpmapGenerationShiftHistory[0][1],1.0f);
    KeepHistoryInRange(EdgeWakeOffsetHistory,1.0f);
}