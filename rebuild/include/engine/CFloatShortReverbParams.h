#ifndef FABLE_PASS22_CFLOATSHORTREVERBPARAMS_H
#define FABLE_PASS22_CFLOATSHORTREVERBPARAMS_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
#include "engine/CFloatShortReverbModuleParams.h"
struct CFloatShortReverbParams {
public:
 CFloatShortReverbModuleParams m_aShortRvb[4]; // +0x0
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CFloatShortReverbParamsSizePass22[(sizeof(CFloatShortReverbParams)==192)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
