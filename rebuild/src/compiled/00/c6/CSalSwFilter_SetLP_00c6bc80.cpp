// CSalSwFilter::SetLP(const _CLPFILTER* pFilter, float fScale) -- set the low-pass target frequency (SetFrequency) and Q (damping 1/Q, limited to 1/f for stability); while the filter history is clear the current parameters snap to the new ones
// Leaf: ?SetLP@CSalSwFilter@@QAEXPBU_CLPFILTER@@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6BC80, 113 bytes (+ int3 padding), `ret 8`.
// Name (NF99, q11410; was the descriptive CStateVariableFilter::SetParameters over a local model): GameCODA 2.10 CODAApi.lib
//   SalSwFilter.obj public ?SetLP@CSalSwFilter@@QAEXPBU_CLPFILTER@@M@Z (113 bytes; REL32 +0x0D SetFrequency, 1.0f x3, 0.0f x2) +
//   the SDK SalSw3DVoice.obj CalcParams relocation 0x247, the retail call 0x00C54E56 the landed 0x00C54C10 already binds under this
//   spelling (engine/CSalSwFilter.h, opt-in FABLE_RETAIL_CSALSW3D_CALCPARAMS). The Dbg SalSwFilter.obj body gives the statement order:
//   ASSERT(pFilter), SetFrequency(pFilter->uiFreq, fScale), SetQ(pFilter->fQ), `if (IsHistoryClear()) m_Params = m_NewParams`.
// Members / types: the CodeView names on engine/CSalSwFilter.h and engine/CodaSourceTypes.h (_CLPFILTER uiFreq / fQ).
// Callees: CSalSwFilter::SetFrequency 0x00C6BBD0 (same TU in retail: SalSwFilter.obj).
// Shape: retail keeps `this` (ecx) and `pFilter` (edx) live across the SetFrequency call, so its body is a same-TU
//   `__declspec(noinline)` copy under `#pragma auto_inline(off)`; SetQ and IsHistoryClear expand in place (header opt-in inlines:
//   `fld 1.0 ; fdiv ; fst`; `fld [ecx+18h] ; fld 0.0 ; fucompp`).
// Fidelity: same-TU strong definition of 0x00C6BBD0 (retail TU-mate), the owner's body verbatim (GOTCHAS nf96a).
#include <math.h>
#pragma intrinsic(sin)
#define FABLE_RETAIL_CSALSW3D_CALCPARAMS
#define FABLE_RETAIL_CSALSWFILTER_SETFREQUENCY
#define FABLE_RETAIL_CSALSWFILTER_SETQ_INLINE
#define FABLE_RETAIL_CSALSWFILTER_ISHISTORYCLEAR_INLINE
#include "engine/CodaSourceTypes.h"
#include "engine/CSalSwFilter.h"

#pragma auto_inline(off)
__declspec(noinline) void CSalSwFilter::SetFrequency(unsigned int uiFreq, float fScale)
{
    float fFreq = (float)uiFreq * fScale;
    if (fFreq < 200.0f)
        fFreq = 200.0f;
    else if (fFreq > 8000.0f)
        fFreq = 8000.0f;
    m_NewParams.fF = 2.0f * (float)sin(fFreq * 7.1237933e-05f);    // pi / 44100 Hz (float bits 0x38956593)
    if (m_NewParams.fF > 1.0f)
        m_NewParams.fF = 1.0f;
    if (m_NewParams.fF * m_NewParams.fQ > 1.0f) {
        m_NewParams.fQ = 1.0f / m_NewParams.fQ;
        if (m_NewParams.fF * m_NewParams.fQ > 1.0f)
            m_NewParams.fQ = 1.0f / m_NewParams.fF;
    }
}
#pragma auto_inline(on)

void CSalSwFilter::SetLP(const _CLPFILTER* pFilter, float fScale)
{
    SetFrequency(pFilter->uiFreq, fScale);
    SetQ(pFilter->fQ);
    if (IsHistoryClear())
        m_Params = m_NewParams;
}
