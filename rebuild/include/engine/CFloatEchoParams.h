#ifndef FABLE_PASS22_CFLOATECHOPARAMS_H
#define FABLE_PASS22_CFLOATECHOPARAMS_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
struct CFloatEchoParams {
public:
 unsigned int m_uiFeedbackDelay; // +0x0
 float m_fFeedbackGain; // +0x4
 float m_fDepth; // +0x8
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CFloatEchoParamsSizePass22[(sizeof(CFloatEchoParams)==12)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
