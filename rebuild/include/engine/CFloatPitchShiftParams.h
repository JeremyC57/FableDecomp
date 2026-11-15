#ifndef FABLE_PASS22_CFLOATPITCHSHIFTPARAMS_H
#define FABLE_PASS22_CFLOATPITCHSHIFTPARAMS_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
struct CFloatPitchShiftParams {
public:
 float m_fModulationPeriodSamples; // +0x0
 float m_fModulationAmplitudeSamples; // +0x4
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CFloatPitchShiftParamsSizePass22[(sizeof(CFloatPitchShiftParams)==8)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
