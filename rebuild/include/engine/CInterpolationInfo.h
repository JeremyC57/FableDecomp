#ifndef FABLE_ENGINE_CINTERPOLATIONINFO_H
#define FABLE_ENGINE_CINTERPOLATIONINFO_H
// CInterpolationInfo (chunk 1022, row 0x00417747 CMainGameComponent::UpdateForcedTickSpeed): Ego_r.pdb layout (struct_layouts_egor.tsv,
// 0x34 bytes): six nested CInterpolationInfoSet pairs and the server frame (FableWin.pdb spells the nested type
// CInterpolationInfo::CInterpolationInfoSet, e.g. ?UpdateTimer@CPlayerGui@@ABEXABVCInterpolationInfoSet@CInterpolationInfo@@MMAAW4EPlayerGuiMode@@AAM@Z).
// The default constructor is out of line in retail (??0CInterpolationInfo@@QAE@XZ 0x00415A60, landed: zeroes the thirteen words);
// copies are the implicit member-wise ones (UpdateForcedTickSpeed: `rep movsd` of 13 dwords into CMainGameComponent::LastInterpolationInfo).
#include "rebuild_abi.h"
class CInterpolationInfo {
public:
    class CInterpolationInfoSet {
    public:
        float GTPredictedTimeSinceLastRenderFrame;           // +0x00
        float WFInterpolate;                                 // +0x04
    };
    CInterpolationInfo();                                    // 0x00415A60
    CInterpolationInfoSet Current;                           // +0x00
    CInterpolationInfoSet Last;                              // +0x08
    CInterpolationInfoSet PausedCurrent;                     // +0x10
    CInterpolationInfoSet PausedLast;                        // +0x18
    CInterpolationInfoSet BulletTimeCurrent;                 // +0x20
    CInterpolationInfoSet BulletTimeLast;                    // +0x28
    long                  WFServerCurrentTime;               // +0x30
};
FABLE_STATIC_ASSERT(sizeof(CInterpolationInfo) == 0x34);
#endif // FABLE_ENGINE_CINTERPOLATIONINFO_H
