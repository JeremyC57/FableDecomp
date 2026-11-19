// CCoda3DSource::CalcEnvFxGain(float fRolloff) const -- GameCODA 3D source: 0 when muted beyond the maximum distance, else powf(ref / clamped distance, the environment rolloff * the source's EnvFx rolloff factor)
// Leaf: ?CalcEnvFxGain@CCoda3DSource@@ABEMM@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C41BB0, 134 bytes (144 with padding), thiscall `ret 4`, float in st(0); called once, from Commit 0x00C423C4 (env.fRolloff).
// Identity: GameCODA 2.10 (Sensaura SDK, archive.org game-coda-2.10-pc): byte-identical (relocations masked) to the CODAApi.lib .\Release\Coda3DSource.obj COMDAT ?CalcEnvFxGain@CCoda3DSource@@ABEMM@Z
//   (cl 13.10.3077); the CODAApiDbg.lib .\Debug\Coda3DSource.obj CodeView proc CCoda3DSource::CalcEnvFxGain: param fRolloff; locals fGain, fDistance.
// Callees: _CIpow 0x00BFEB40 (powf). Constant 0.0f 0x0122DEDC.
// Text: verbatim the same-TU copy in CCoda3DSource::Commit 0x00C41FB0 (one text, GOTCHAS same-TU strong copy rule), on Commit's TU
//   prologue (FABLE_RETAIL_CCODA3DSOURCE_COMMIT declares the five Calc* private, as in the SDK class).
// Re-port (audit 2026-11-19): replaces the landed CCombatant::ComputeSpellPower (descriptive stand-in class).
#define FABLE_RETAIL_PASS22_CCODA3D_CREATE
#define FABLE_RETAIL_CCODAVECTOR_ORTHOGONALRIGHT
#define FABLE_RETAIL_CCODA3DSOURCE_COMMIT
#include "engine/CCoda3DSource.h"
#include "engine/CCodaContext.h"
#include "engine/CCodaListener.h"
#include "engine/SalMath.h"

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