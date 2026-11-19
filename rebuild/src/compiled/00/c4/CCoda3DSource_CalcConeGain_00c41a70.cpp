// CCoda3DSource::CalcConeGain() const -- GameCODA 3D source: the cone angle from atan2 of |cone - direction| and |cone + direction|; 1 inside the inner angle, the outer gain beyond the outer angle, linear in between
// Leaf: ?CalcConeGain@CCoda3DSource@@ABEMXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C41A70, 308 bytes (320 with padding), thiscall `ret`, float in st(0); called once, from Commit 0x00C4232C.
// Identity: GameCODA 2.10 (Sensaura SDK, archive.org game-coda-2.10-pc): the same function as the CODAApi.lib .\Release\Coda3DSource.obj COMDAT ?CalcConeGain@CCoda3DSource@@ABEMXZ (not byte-identical: 125 non-relocation byte differences; the native fork)
//   (cl 13.10.3077); the CODAApiDbg.lib .\Debug\Coda3DSource.obj CodeView proc CCoda3DSource::CalcConeGain: locals fGain, fOuter, fMagnitudeMinus, vPlus, vMinus (CCodaVector), fMagnitudePlus, fAngle, fInner -- declared in the CodeView record order.
// No calls: atan2f expands to fpatan, CCodaVector operator- / operator+ / Magnitude are inline. Constants 229.18311f 0x012B7B7C, 360.0f 0x01238020, 1.0f 0x0122DED8, 2^32 0x0122DCB4.
// Text: verbatim the same-TU copy in CCoda3DSource::Commit 0x00C41FB0 (one text, GOTCHAS same-TU strong copy rule), on Commit's TU
//   prologue (FABLE_RETAIL_CCODA3DSOURCE_COMMIT declares the five Calc* private, as in the SDK class).
// Re-port (audit 2026-11-19): replaces the landed CCombatant::ComputeEffectiveRange (descriptive stand-in class).
#define FABLE_RETAIL_PASS22_CCODA3D_CREATE
#define FABLE_RETAIL_CCODAVECTOR_ORTHOGONALRIGHT
#define FABLE_RETAIL_CCODA3DSOURCE_COMMIT
#include "engine/CCoda3DSource.h"
#include "engine/CCodaContext.h"
#include "engine/CCodaListener.h"
#include "engine/SalMath.h"

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