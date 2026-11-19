// CSalSwFilter::SetFrequency(unsigned int uiFreq, float fScale) -- set the new frequency coefficient: the scaled frequency clamped to 200..8000 Hz, f = 2 sin(pi * freq / 44100) capped at 1; when the new Q makes f * q exceed 1 the damping is re-limited
// Leaf: ?SetFrequency@CSalSwFilter@@AAEXIM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6BBD0, thiscall `ret 8`.
// Name (NF99, q11410..q11412 family; was the descriptive CStateVariableFilter::SetCutoff over a local model): GameCODA 2.10
//   CODAApi.lib SalSwFilter.obj private ?SetFrequency@CSalSwFilter@@AAEXIM@Z (161 bytes; relocations 2^32, 200.0f x2, 8000.0f x2,
//   pi/44100 0x38956593, 1.0f x5) + the SDK release SetLP / SetEQ REL32 at +0x0D to this symbol, retail calls 0x00C6BC8C (SetLP
//   0x00C6BC80) and 0x00C6BD0C (SetEQ 0x00C6BD00). Declared by engine/CSalSwFilter.h (NF99 opt-in FABLE_RETAIL_CSALSWFILTER_SETFREQUENCY).
// Members: the header's CodeView names (m_NewParams.fF +0x0c, m_NewParams.fQ +0x10). Parameter names descriptive (no Dbg record read).
// Shape: the frequency is an unsigned conversion (`fild ; jge ; fadd 2^32`) times the scale; sin is the x87 intrinsic.
#include <math.h>
#pragma intrinsic(sin)
#define FABLE_RETAIL_CSALSWFILTER_SETFREQUENCY
#include "engine/CSalSwFilter.h"

void CSalSwFilter::SetFrequency(unsigned int uiFreq, float fScale)
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
