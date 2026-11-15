#ifndef FABLE_PASS22_CFLOATREFLECTIONPARAMS_H
#define FABLE_PASS22_CFLOATREFLECTIONPARAMS_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
struct CFloatReflectionParams {
public:
 unsigned int m_auiInputDelay[2]; // +0x0
 unsigned int m_uiFeedbackDelay; // +0x8
 float m_fFeedbackGain; // +0xc
 float m_fOutputGain; // +0x10
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CFloatReflectionParamsSizePass22[(sizeof(CFloatReflectionParams)==20)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
