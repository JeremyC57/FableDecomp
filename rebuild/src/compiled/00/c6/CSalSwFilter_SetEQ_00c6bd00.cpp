// CSalSwFilter::SetEQ(const _CEQFILTER* pFilter, float fScale) -- set the EQ target frequency (SetFrequency), gain ((gain - 1) * 1.1, or -1.001 below a gain of 0.09) and Q; while the filter history is clear the current parameters snap to the new ones
// Leaf: ?SetEQ@CSalSwFilter@@QAEXPBU_CEQFILTER@@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6BD00, 155 bytes, `ret 8`.
// Name (NF99, q11411; was the descriptive CStateVariableFilter::SetParametersWithGain over a local model): GameCODA 2.10 CODAApi.lib
//   SalSwFilter.obj public ?SetEQ@CSalSwFilter@@QAEXPBU_CEQFILTER@@M@Z (155 bytes; REL32 +0x0D SetFrequency, 0.09f, 1.0f, 1.1f,
//   0.0f x2) + the SDK SalSw3DVoice.obj CalcParams relocation 0x269, the retail call 0x00C54E78 the landed 0x00C54C10 already binds
//   under this spelling (engine/CSalSwFilter.h, opt-in FABLE_RETAIL_CSALSW3D_CALCPARAMS). The Dbg SalSwFilter.obj body gives the
//   statement order: SetFrequency(pFilter->uiFreq, fScale), SetGain(pFilter->fGain), SetQ(pFilter->fQ), `if (IsHistoryClear())
//   m_Params = m_NewParams`.
// Members / types: the CodeView names on engine/CSalSwFilter.h and engine/CodaSourceTypes.h (_CEQFILTER uiFreq / fGain / fQ).
// Callees: CSalSwFilter::SetFrequency 0x00C6BBD0 (same TU in retail: SalSwFilter.obj).
// Shape: as SetLP 0x00C6BC80 (same-TU noinline SetFrequency keeps ecx/edx); SetGain expands in place with its parameter in st0
//   across the `< 0.09f` test (`fcom` ; `fstp st0` on the constant arm, an immediate -1.001f store).
// Fidelity: same-TU strong definition of 0x00C6BBD0 (retail TU-mate), the owner's body verbatim (GOTCHAS nf96a).
#include <math.h>
#pragma intrinsic(sin)
#define FABLE_RETAIL_CSALSW3D_CALCPARAMS
#define FABLE_RETAIL_CSALSWFILTER_SETFREQUENCY
#define FABLE_RETAIL_CSALSWFILTER_SETQ_INLINE
#define FABLE_RETAIL_CSALSWFILTER_SETGAIN_INLINE
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

void CSalSwFilter::SetEQ(const _CEQFILTER* pFilter, float fScale)
{
    SetFrequency(pFilter->uiFreq, fScale);
    SetGain(pFilter->fGain);
    SetQ(pFilter->fQ);
    if (IsHistoryClear())
        m_Params = m_NewParams;
}
