#ifndef FABLE_ENGINE_CENGINEINTERPOLATIONINFO_H
#define FABLE_ENGINE_CENGINEINTERPOLATIONINFO_H
// Chunk 2051 (row 0x0042EC7C CNewFrontendGameComponent::Run). Ego_r.pdb CEngineInterpolationInfo (0x70 bytes; the landed ctor
// 0x0042FA30 zeroes all of it), CEngineInterpolationInfoSet 0x0C. Default ctor out of line (??0CEngineInterpolationInfo@@QAE@XZ).
#include "rebuild_abi.h"
class CEngineInterpolationInfoSet {
public:
#ifdef FABLE_RETAIL_CENGINEINTERPOLATIONINFOSET_CTOR
    // NF233 opt-in (row 0x0042EA8F CNewFrontendGameComponent::CNewFrontendGameComponent, m_LastInterp +0xCC): the user default
    // constructor, Ego_d.pdb and FableWin.pdb ??0CEngineInterpolationInfoSet@CEngineInterpolationInfo@@QAE@XZ (no Ego_r.pdb copy:
    // inline in release); retail expands it as three float zero stores (0x0042EB41..0x0042EB58 `fldz ; fstp dword ptr [eax]` / +4 / +8).
    // The PDBs nest the class in CEngineInterpolationInfo; this header keeps the existing top-level spelling (a header item).
    CEngineInterpolationInfoSet() : GTPredictedRenderAt(0.0f), GTPredictedTimeSinceLastRenderFrame(0.0f), WFInterpolate(0.0f) {}
#endif
    float GTPredictedRenderAt;                                                // +0x00
    float GTPredictedTimeSinceLastRenderFrame;                                // +0x04
    float WFInterpolate;                                                      // +0x08
};
class CEngineInterpolationInfo {
public:
    CEngineInterpolationInfo();                                               // 0x0042FA30
    CEngineInterpolationInfoSet Current;                                      // +0x00
    CEngineInterpolationInfoSet Last;                                         // +0x0C
    CEngineInterpolationInfoSet PausedCurrent;                                // +0x18
    CEngineInterpolationInfoSet PausedLast;                                   // +0x24
    CEngineInterpolationInfoSet BulletTimeCurrent;                            // +0x30
    CEngineInterpolationInfoSet BulletTimeLast;                               // +0x3C
    long WFServerCurrentTime;                                                 // +0x48
    CEngineInterpolationInfoSet InterpolationSetList[3];                      // +0x4C
};
FABLE_STATIC_ASSERT_AT(24, sizeof(CEngineInterpolationInfo) == 0x70);
#endif
