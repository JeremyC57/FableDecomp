// CCoda3DSource::CalcDoppler() const -- GameCODA 3D source: 1 - (velocity . direction) * Doppler factor * CODA Doppler speed, clamped at zero
// Leaf: ?CalcDoppler@CCoda3DSource@@ABEMXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C41A20, 80 bytes, thiscall `ret`, float in st(0); called once, from Commit 0x00C42317.
// Identity: GameCODA 2.10 (Sensaura SDK, archive.org game-coda-2.10-pc): byte-identical (relocations masked) to the CODAApi.lib .\Release\Coda3DSource.obj COMDAT ?CalcDoppler@CCoda3DSource@@ABEMXZ
//   (cl 13.10.3077); the CODAApiDbg.lib .\Debug\Coda3DSource.obj CodeView proc CCoda3DSource::CalcDoppler: locals f, fDoppler.
// No callees. Globals: CODA (m_fDopplerSpeed 0x0143BBF0 = CODA + 0x98). Constants 1.0f 0x0122DED8, 0.0f 0x0122DEDC.
// Text: verbatim the same-TU copy in CCoda3DSource::Commit 0x00C41FB0 (one text, GOTCHAS same-TU strong copy rule), on Commit's TU
//   prologue (FABLE_RETAIL_CCODA3DSOURCE_COMMIT declares the five Calc* private, as in the SDK class).
// Re-port (audit 2026-11-19): replaces the landed CCombatant::ComputeLightingAdjustment (descriptive stand-in class; global g_combatLightingScale = CODA.m_fDopplerSpeed).
#define FABLE_RETAIL_PASS22_CCODA3D_CREATE
#define FABLE_RETAIL_CCODAVECTOR_ORTHOGONALRIGHT
#define FABLE_RETAIL_CCODA3DSOURCE_COMMIT
#include "engine/CCoda3DSource.h"
#include "engine/CCodaContext.h"
#include "engine/CCodaListener.h"
#include "engine/SalMath.h"

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