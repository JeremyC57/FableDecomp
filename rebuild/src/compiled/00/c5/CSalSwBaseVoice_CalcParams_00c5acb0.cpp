// 00c5acb0 CSalSwBaseVoice::CalcParams
// CSalSwBaseVoice::CalcParams [0x00C5ACB0] -- calculate current source parameters, release envelopes, advance LFOs, then dispatch the 2D/3D update.
// Grade: RELOCATION_MATCH -- 662 bytes,20 exact relocations; full-body tests pass.
// Leaf: ?CalcParams@CSalSwBaseVoice@@AAEXPBVCCodaSource@@_J@Z
// Flags: /Oi
// Retail: Fable.exe TLC @0x00C5ACB0, 662 bytes, ret12.
#define FABLE_RETAIL_PASS22_CSALSW_CALCPARAMS
#include "engine/CSalSwBaseVoice.h"
#include "engine/CSalSw2DVoice.h"
#include "engine/CSalSw3DVoice.h"
#include "engine/CCoda3DSource.h"
#include "engine/CSalPlatform.h"
#include <math.h>

void CSalSwBaseVoice::CalcParams(const CCodaSource* pSource, CREFTIME rtTime)
{
    const CODASOURCE& params = pSource->GetParams();
    unsigned int ffUpdate;
    if (IsStarting())
        ffUpdate = InitParams(pSource,params);
    else
        ffUpdate = params.ffUpdate;
    if (m_PlayBuffer.IsCreated() && (ffUpdate & 1))
        UpdatePlayBuffer(pSource->IsLooping());
    if (IsReleasingEnvelopes() && m_rtRelease <= rtTime) {
        m_ffParams &= ~0x200;
        m_ffStatus &= ~0x7e7e;
        if ((params.pLevelEnv || params.pMultiEnv) && !(m_ffStatus & 0x20)) {
            m_ffStatus |= 0x20;
            if (params.pLevelEnv)
                m_fLevelEnvDelta = CalcExpDelta(1.0f,params.pLevelEnv->fRelease);
            if (params.pMultiEnv) {
                if (params.pMultiEnv->fPitchScale != 0.0f) {
                    m_fMultiEnvPitchDelta = CalcPitchDelta(params.pMultiEnv->fPitchScale,0.0f,params.pMultiEnv->fRelease);
                    m_nMultiEnvFrames = (int)(log10(m_fMultiEnvPitch) * (-1.0f / log10(m_fMultiEnvPitchDelta)));
                }
                if (params.pMultiEnv->fFilterCutOff != 0.0f) {
                    m_fMultiEnvCutoffDelta = CalcPitchDelta(params.pMultiEnv->fFilterCutOff,0.0f,params.pMultiEnv->fRelease);
                    if (params.pMultiEnv->fPitchScale == 0.0f)
                        m_nMultiEnvFrames = (int)(log10(m_fMultiEnvCutoff) * (-1.0f / log10(m_fMultiEnvCutoffDelta)));
                }
            }
        }
    }
    if (!IsLevelEnvReleased() && CalcLevelEnvelope(params.pLevelEnv,(ffUpdate & 8) != 0))
        ffUpdate |= 8;
    if (!IsMultiEnvReleased() && CalcMultiFuncEnvelope(params.pMultiEnv,(ffUpdate & 0x10) != 0))
        ffUpdate |= 0x10;
    if ((ffUpdate & 0x20) && !IsStarting())
        UpdatePitchLFO(params.pPitchLFO);
    if (params.pPitchLFO) {
        CalcPitchLFO(params.pPitchLFO);
        ffUpdate |= 0x20;
    }
    if ((ffUpdate & 0x40) && !IsStarting())
        UpdateMultiFuncLFO(params.pMultiLFO);
    if (params.pMultiLFO) {
        CalcMultiFuncLFO(params.pMultiLFO);
        ffUpdate |= 0x40;
    }
    if (ffUpdate || g_pSal->GetUpdate()) {
        if (Is3D()) {
            const CCoda3DSource* p3D = pSource->Get3D();
            ((CSalSw3DVoice*)this)->CalcParams(p3D,ffUpdate);
        } else {
            const CCoda2DSource* p2D = pSource->Get2D();
            ((CSalSw2DVoice*)this)->CalcParams(p2D,ffUpdate);
        }
    }
    m_ffStatus &= ~1;
}