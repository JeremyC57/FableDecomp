// CStateVariableFilter::CStateVariableFilter() -- an idle state-variable filter: zero target coefficients copied into the current ones, empty band/low state, ramp at its start
// Leaf: ??0CStateVariableFilter@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6B9A0, 37 bytes, __fastcall `ret`, returns `this`. No callees.
// Identity (label correction): the catalog's "pair<EHeroMorphType,CParticleMorphs::CEntry>" is propagated. libpk's
//   software-mixer DSP (no PDB/RTTI: descriptive names, after the staged models of SetCutoff 0x00C6BBD0 / SetParameters
//   0x00C6BC80): the only caller, the voice constructor 0x00C5B1A0 (vtable 0x012B5C60), runs it over the two 0x24-byte
//   filters at +0xDC; the next method 0x00C6B9D0 (unnamed; agent443 names 0x00C6BAE0 Filter and 0x00C6B8C0 FilterRamped)
//   compares the current coefficients (+0/+4/+8) with the targets (+0xC/+0x10/+0x14),
//   runs band (+0x18) / low (+0x1C) and commits `current = target` (`mov eax,esi`, an inlined struct assignment) when the
//   ramp position (+0x20) reaches its length.
// Shape: the targets are zeroed (f, gain, damping), then `m_current = m_target;` -- VC7.1 forwards the known zeros
//   through the inlined struct assignment, which is what gives retail's copied `this` (edx) and copied zero (esi) and
//   schedules the current triple first; a plain three-store reset of both triples has neither.
struct SFilterCoefficients {
    float m_f;                                          // +0x00 frequency coefficient
    float m_damping;                                    // +0x04 damping (1/Q)
    float m_gain;                                       // +0x08 gain coefficient
};

struct CStateVariableFilter {
    SFilterCoefficients m_current;                      // +0x00
    SFilterCoefficients m_target;                       // +0x0C
    float               m_band;                         // +0x18 filter state
    float               m_low;                          // +0x1C filter state
    unsigned long       m_rampPosition;                 // +0x20
    CStateVariableFilter();
};

CStateVariableFilter::CStateVariableFilter()
{
    m_target.m_f = 0.0f;
    m_target.m_gain = 0.0f;
    m_target.m_damping = 0.0f;
    m_current = m_target;
    m_band = 0.0f;
    m_low = 0.0f;
    m_rampPosition = 0;
}