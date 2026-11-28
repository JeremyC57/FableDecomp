// 00b8c830 CEngineEnvironment::FrameStart
// Leaf: ?FrameStart@CEngineEnvironment@@UAEXABVCEngineComponentParams@@@Z
// Flags: (none)
#define FABLE_RETAIL_CENGINEENVIRONMENT
#define FABLE_RETAIL_CENGINEENVIRONMENT_WIND
#define FABLE_RETAIL_ENVIRONMENT_FRAMESTART_PASS30
#define FABLE_RETAIL_CENGINECOMPONENTPARAMS_INTERPOLATIONINFO
#define FABLE_RETAIL_CENGINEINTERPOLATIONINFO_NESTED_SET
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE
#include "engine/CEngineEnvironment.h"
#include "engine/CEngineComponentRenderParams.h"
#pragma intrinsic(sqrt)

void CEngineEnvironment::FrameStart(const CEngineComponentParams& params)
{
    BulletTimeInterpolation = params.InterpolationInfo.BulletTimeCurrent.WFInterpolate;
    PausedInterpolation = params.InterpolationInfo.PausedCurrent.WFInterpolate;
    NonPausedInterpolation = params.InterpolationInfo.Current.WFInterpolate;
    CurrentInterpolation = BulletTimeInterpolation;
    CurrentRenderFrame = params.FrameID;

    for (long n = 0; n < 63; ++n) {
        const C3DVector& skew_next = WindSkewHistory[n];
        const C3DVector& skew_last = WindSkewHistory[n + 1];
        InterpolatedWindSkewHistory[n] = skew_last + (skew_next - skew_last) * CurrentInterpolation;
    }
    CurrentWindSpeed = PreviousWindSpeed + (NextWindSpeed - PreviousWindSpeed) * CurrentInterpolation;
    CurrentWindVector = PreviousWindVector + (NextWindVector - PreviousWindVector) * CurrentInterpolation;
    if (CurrentWindVector.GetAccurateMagnitude() < 0.1f) {
        CurrentWindVector = PreviousWindVector;
    }
    CurrentWindVector.AccurateNormalise();
}