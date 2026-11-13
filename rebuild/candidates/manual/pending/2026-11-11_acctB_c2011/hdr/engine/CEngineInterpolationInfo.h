#ifndef FABLE_ENGINE_CENGINEINTERPOLATIONINFO_H
#define FABLE_ENGINE_CENGINEINTERPOLATIONINFO_H
// Overlay header (chunk 2011, row 0x00BF18B0 CEnginePrimitiveRenderUtil::AddGeneratedObjects): Ego_r.pdb CEngineInterpolationInfo
// (112 bytes) and its nested CEngineInterpolationInfoSet (12 bytes; FableWin.pdb ??0CEngineInterpolationInfoSet@CEngineInterpolationInfo@@QAE@XZ),
// the layout the landed 0x01224900 / 0x00BCEB40 rows model locally: six sets, WFServerCurrentTime +0x48, InterpolationSetList[3]
// +0x4C (WFInterpolate at +8: `[set*12 + 0x0143B934]` for the static at 0x0143B8E0). The default constructor is out of line
// (??0CEngineInterpolationInfo@@QAE@XZ 0x0042FA30).
#include "rebuild_abi.h"
class CEngineInterpolationInfo {
public:
    class CEngineInterpolationInfoSet {
    public:
        float GTPredictedRenderAt;                      // +0x00
        float GTPredictedTimeSinceLastRenderFrame;      // +0x04
        float WFInterpolate;                            // +0x08
    };
    CEngineInterpolationInfo();                         // 0x0042FA30
    CEngineInterpolationInfoSet Current;                // +0x00
    CEngineInterpolationInfoSet Last;                   // +0x0c
    CEngineInterpolationInfoSet PausedCurrent;          // +0x18
    CEngineInterpolationInfoSet PausedLast;             // +0x24
    CEngineInterpolationInfoSet BulletTimeCurrent;      // +0x30
    CEngineInterpolationInfoSet BulletTimeLast;         // +0x3c
    long WFServerCurrentTime;                           // +0x48
    CEngineInterpolationInfoSet InterpolationSetList[3];   // +0x4c
};
FABLE_STATIC_ASSERT(sizeof(CEngineInterpolationInfo) == 0x70);
#endif
