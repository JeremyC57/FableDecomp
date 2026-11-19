// CCoda3DSource::Commit(const CCodaObjectList& Listeners, const CCodaObjectList& Environments, unsigned int ffGlobalCommit) -- GameCODA 3D source: merge the global commit bits into the source's update word; a head-relative source copies its position / velocity / normalised cone direction, any other source finds the nearest listener and expresses its position, velocity and cone direction in that listener's frame (right axis scaled by the context's Z-axis sign); then the direction and distance, the distance gain (or the user callback), Doppler, cone gain, direct levels, the environment's obstruction / occlusion / exclusion levels (clamped at -10000 mB), and finally publish the update bits to the source parameters
// Leaf: ?Commit@CCoda3DSource@@QAEXABVCCodaObjectList@@0I@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C41FB0, 1570 bytes, thiscall `ret 0Ch`. Constants 0.5f (0x0122F59C), 1.0f (0x0122DED8).
// Identity: the same function as GameCODA 2.10 CODAApi.lib member .\Release\Coda3DSource.obj ?Commit@CCoda3DSource@@QAEXABVCCodaObjectList@@0I@Z
//   (cl 13.10.3077, @comp.id 0x00600C05), not byte-identical (1622 bytes: the 2.10 build calls the virtual IsListener / IsEnvironment
//   checks inside Listener() / Environment(); retail's native fork casts directly, and its members sit 4 bytes lower than the Dbg build).
//   Authored as C++; no library grade is possible.
// Shape: the CODAApiDbg.lib object's Commit (.\Coda3DSource.cpp): CodeView params Listeners / Environments / ffGlobalCommit, locals pObject,
//   pListener, f, v; block {vDirection, vRight, pNearestListener, fDistance}; block {pEnvironment, fGain, fLevel, env}; statement order and
//   the stepwise fLevel accumulations as the Dbg body. The Dbg CODA_ASSERTs (pListener, pListener->IsCreated(), pObject->GetNext(),
//   pEnvironment) are compiled out of the release and not written.
// Retail-asm: 0x00C423ED..0x00C423F5 `fsub [0.5f] ; fstp [esp+28h] ; fld [esp+28h] ; fistp dword ptr [esp+2Ch]` (and 0x00C42466,
//   0x00C424CA, 0x00C42573) -- the SDK's SalMath.h sal_ftoi: a bare dword fistp through inline asm (the Dbg /Od body emits `fld value ;
//   fistp result`; VC7.1 `(int)x` calls __ftol2). Reached through engine/SalMath.h -> the shared libpk/FtoL.h helper (FABLE_PORTABLE twin).
// Inferred: the listener / environment objects are reached with static_cast (the landed CCodaContext::Process 0x00C3D470 spelling): retail
//   0x00C42095 uses the list head as the listener and 0x00C423B3..0x00C423BE reads the environment parameters at object + 0x0C with no
//   IsListener / IsEnvironment call, where 2.10's Listener() / Environment() test the type.
// Callees: CCodaVector::Normalize 0x00C402E0 (x2), CalcDirectionAndDistance 0x00C418F0, CalcDistanceGain 0x00C41990, CalcDoppler 0x00C41A20,
//   CalcConeGain 0x00C41A70, CalcEnvFxGain 0x00C41BB0, sal_gainToLevel 0x00C40F50. Globals: CODA (m_uiHFReference 0x0143BBE0, m_fZAxis 0x0143BBE4).
// Header opt-ins: FABLE_RETAIL_CCODA3DSOURCE_COMMIT (CCodaObject GetNext, CCodaObjectList GetObjects, CCodaContext GetZAxis, CCodaListener
//   accessors, CCodaVector MagnitudeSquare / operator-, CCoda3DSource IsHeadRelative + the Calc* declarations), FABLE_RETAIL_CCODAVECTOR_ORTHOGONALRIGHT
//   (Normalize, Dot), new engine/SalMath.h.
#define FABLE_RETAIL_PASS22_CCODA3D_CREATE
#define FABLE_RETAIL_CCODAVECTOR_ORTHOGONALRIGHT
#define FABLE_RETAIL_CCODA3DSOURCE_COMMIT
#include "engine/CCoda3DSource.h"
#include "engine/CCodaContext.h"
#include "engine/CCodaListener.h"
#include "engine/SalMath.h"

void CCoda3DSource::Commit(const CCodaObjectList& Listeners, const CCodaObjectList& Environments, unsigned int ffGlobalCommit)
{
    CCodaObject* pObject;
    CCodaListener* pListener;
    float f;
    CCodaVector v;

    m_ffCommit |= ffGlobalCommit;
    if (0 == m_ffCommit)
        return;

    if (IsHeadRelative()) {
        pListener = 0;
        if (m_ffCommit & 0x8000)
            m_3D.vPosition = m_vPosition;
        if (m_ffCommit & 0x10000)
            m_3D.vVelocity = m_vVelocity;
        if (m_ffCommit & 0x80000) {
            m_vConeDirection.Normalize();
            m_3D.vConeDirection = m_vConeDirection;
        }
    } else {
        CCodaVector vDirection;
        CCodaVector vRight;
        CCodaListener* pNearestListener;
        float fDistance;

        if (m_ffCommit & 0x20000)
            m_ffCommit |= 0x8000;
        pObject = Listeners.GetObjects();
        pListener = static_cast<CCodaListener*>(pObject);
        pNearestListener = pListener;
        vDirection = m_vPosition - pListener->GetPosition();
        fDistance = vDirection.MagnitudeSquare();
        for (pObject = pObject->GetNext(); 0 != pObject; pObject = pObject->GetNext()) {
            pListener = static_cast<CCodaListener*>(pObject);
            v = m_vPosition - pListener->GetPosition();
            f = v.MagnitudeSquare();
            if (f < fDistance) {
                fDistance = f;
                vDirection = v;
                pNearestListener = pListener;
            }
        }
        pListener = pNearestListener;
        if (m_ffCommit & 0x88000) {
            vRight = pListener->GetRight();
            vRight.x *= CODA.GetZAxis();
        }
        if (m_ffCommit & 0x8000) {
            m_3D.vPosition.x = vDirection.Dot(vRight);
            m_3D.vPosition.y = vDirection.Dot(pListener->GetTop());
            m_3D.vPosition.z = vDirection.Dot(pListener->GetFront());
        }
        if (m_ffCommit & 0x10000)
            m_3D.vVelocity = m_vVelocity - pListener->GetVelocity();
        if (m_ffCommit & 0x80000) {
            m_vConeDirection.Normalize();
            m_3D.vConeDirection.x = m_vConeDirection.Dot(vRight);
            m_3D.vConeDirection.y = m_vConeDirection.Dot(pListener->GetTop());
            m_3D.vConeDirection.z = m_vConeDirection.Dot(pListener->GetFront());
        }
    }

    if (m_ffCommit & 0x8000) {
        m_3D.fDistance = CalcDirectionAndDistance(m_3D.vDirection, m_3D.vPosition);
        m_ffCommit |= 0xd0000;
    }
    if (m_ffCommit & 0x40000) {
        if (0 == m_pfnDistanceCallback)
            m_3D.fDistanceGain = CalcDistanceGain();
        else
            m_3D.fDistanceGain = m_pfnDistanceCallback(this, pListener, m_3D.fDistance, m_DistanceCallbackData);
        m_ffCommit |= 0x800000;
        m_ffCommit |= 0x1800000;
    }
    if (m_ffCommit & 0x10000)
        m_3D.fDoppler = CalcDoppler();
    if (m_ffCommit & 0x380000) {
        m_3D.fConeGain = CalcConeGain();
        m_ffCommit |= 0x200000;
    }
    if (m_ffCommit & 0x1400000) {
        m_3D.iDirect = m_iDirect;
        m_3D.iDirectHF = m_iDirectHF;
        m_3D.uiDirectHFReference = CODA.GetHFReference();
        m_ffCommit |= 0x1400000;
    }
    if (!(m_ffCaps & 0x1000))
        m_ffCommit &= ~0x400000;
    if (!(m_ffCaps & 0x80))
        m_ffCommit &= ~0x1000000;
    if ((m_ffCommit & 0x1000000) && 0 != (pObject = Environments.GetObjects())) {
        CCodaEnvironment* pEnvironment;
        float fGain;
        float fLevel;

        pEnvironment = static_cast<CCodaEnvironment*>(pObject);
        const CENVIRONMENT& env = pEnvironment->GetParams();
        fGain = CalcEnvFxGain(env.fRolloff);

        fLevel = m_EnvFx.iObstruction * m_EnvFx.fObstructionLFRatio;
        fLevel += m_EnvFx.iOcclusion * m_EnvFx.fOcclusionLFRatio;
        m_3D.iDirect += sal_ftoi(fLevel);
        if (m_3D.iDirect < -10000)
            m_3D.iDirect = -10000;

        fLevel = m_EnvFx.iObstruction * (1.0f - m_EnvFx.fObstructionLFRatio);
        fLevel += m_EnvFx.iOcclusion * (1.0f - m_EnvFx.fOcclusionLFRatio);
        fLevel += env.iAirAbsorptionHF * m_3D.fDistance * m_EnvFx.fAirAbsorptionFactor;
        m_3D.iDirectHF += sal_ftoi(fLevel);
        if (m_3D.iDirectHF < -10000)
            m_3D.iDirectHF = -10000;
        m_3D.uiDirectHFReference = env.uiHFReference;

        m_3D.iEnvFx = m_EnvFx.iEnvironment;
        fLevel = m_EnvFx.iOcclusion * m_EnvFx.fOcclusionLFRatio;
        fLevel += m_EnvFx.iExclusion * m_EnvFx.fExclusionLFRatio;
        m_3D.iEnvFx += sal_ftoi(fLevel);
        if (fGain != 1.0f)
            m_3D.iEnvFx += sal_gainToLevel(fGain);
        if (m_3D.iEnvFx < -10000)
            m_3D.iEnvFx = -10000;
        else if (m_3D.iEnvFx > 0)
            m_3D.iEnvFx = 0;

        m_3D.iEnvFxHF = m_EnvFx.iEnvironmentHF;
        fLevel = m_EnvFx.iOcclusion * (1.0f - m_EnvFx.fOcclusionLFRatio);
        fLevel += m_EnvFx.iExclusion * (1.0f - m_EnvFx.fExclusionLFRatio);
        m_3D.iEnvFxHF += sal_ftoi(fLevel);
        if (m_3D.iEnvFxHF < -10000)
            m_3D.iEnvFxHF = -10000;
        m_3D.uiEnvFxHFReference = env.uiHFReference;
    }
    if (!(m_ffCaps & 0x800000))
        m_ffCommit &= ~0x800000;
    m_Params.ffUpdate |= m_ffCommit;
    m_ffCommit = 0;
}

// The private helpers of the same .\Coda3DSource.cpp, in the SDK source order (after Commit). The release object compiles them ahead of
// Commit (sections 149..166 v Commit 168), so their register use and side effects are known at Commit's call sites: retail keeps
// m_ffCommit in edi across CalcDoppler / CalcConeGain (0x00C42307..0x00C4233D, no reload). Each body is graded at its own address.
// Measured (audit 2026-11-19): only the CalcDoppler and CalcConeGain bodies are byte-required (without either: 1574v1570 / 1568v1570).
// Not byte-required by Commit (audit 2026-11-19: dropping it alone, or with the other two such bodies, still matches).
// Fidelity: same-TU strong definition of 0x00C418F0 (retail TU-mate)
float CCoda3DSource::CalcDirectionAndDistance(CCodaVector& vDirection, const CCodaVector& cvPosition)
{
    float fDistance;
    if (cvPosition.IsZero()) {
        fDistance = 0.0f;
        vDirection.Set(0.0f, 0.0f, 1.0f);
    } else {
        vDirection = cvPosition;
        fDistance = cvPosition.Magnitude();
        vDirection.Normalize();
    }
    return fDistance;
}

// Not byte-required by Commit (audit 2026-11-19: dropping it alone, or with the other two such bodies, still matches).
// Fidelity: same-TU strong definition of 0x00C41990 (retail TU-mate)
float CCoda3DSource::CalcDistanceGain() const
{
    float fGain;
    float fDistance;
    if (IsMaxDistanceMuted() && m_3D.fDistance >= m_fMaxDistance)
        return 0.0f;
    fDistance = (m_3D.fDistance < m_fMinDistance) ? m_fMinDistance : ((m_3D.fDistance > m_fMaxDistance) ? m_fMaxDistance : m_3D.fDistance);
    fGain = powf(m_fRefDistance / fDistance, CODA.GetRolloffFactor() * m_fRolloff);
    return fGain;
}

// Fidelity: same-TU strong definition of 0x00C41A20 (retail TU-mate)
float CCoda3DSource::CalcDoppler() const
{
    float f;
    float fDoppler;
    f = m_3D.vVelocity.Dot(m_3D.vDirection);
    fDoppler = 1.0f - f * m_fDoppler * CODA.GetDopplerSpeed();
    if (fDoppler < 0.0f)
        fDoppler = 0.0f;
    return fDoppler;
}

// Fidelity: same-TU strong definition of 0x00C41A70 (retail TU-mate)
float CCoda3DSource::CalcConeGain() const
{
    float fGain;
    float fOuter;
    float fMagnitudeMinus;
    CCodaVector vPlus;
    CCodaVector vMinus;
    float fMagnitudePlus;
    float fAngle;
    float fInner;

    vMinus = m_3D.vConeDirection - m_3D.vDirection;
    vPlus = m_3D.vConeDirection + m_3D.vDirection;
    fMagnitudeMinus = vMinus.Magnitude();
    fMagnitudePlus = vPlus.Magnitude();
    if (fMagnitudePlus < fMagnitudeMinus)
        fAngle = atan2f(fMagnitudePlus, fMagnitudeMinus) * 229.18311f;
    else
        fAngle = 360.0f - atan2f(fMagnitudeMinus, fMagnitudePlus) * 229.18311f;
    fInner = (float)m_uiConeInner;
    fOuter = (float)m_uiConeOuter;
    if (fAngle <= fInner)
        fGain = 1.0f;
    else if (fAngle >= fOuter)
        fGain = m_fConeOuterGain;
    else
        fGain = (m_fConeOuterGain - 1.0f) * (fAngle - fInner) / (fOuter - fInner) + 1.0f;
    return fGain;
}

// Not byte-required by Commit (audit 2026-11-19: dropping it alone, or with the other two such bodies, still matches).
// Fidelity: same-TU strong definition of 0x00C41BB0 (retail TU-mate)
float CCoda3DSource::CalcEnvFxGain(float fRolloff) const
{
    float fGain;
    float fDistance;
    if (IsMaxDistanceMuted() && m_3D.fDistance >= m_fMaxDistance)
        return 0.0f;
    fDistance = (m_3D.fDistance < m_fMinDistance) ? m_fMinDistance : ((m_3D.fDistance > m_fMaxDistance) ? m_fMaxDistance : m_3D.fDistance);
    fGain = powf(m_fRefDistance / fDistance, fRolloff * m_EnvFx.fRolloffFactor);
    return fGain;
}