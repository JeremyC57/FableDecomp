#ifndef FABLE_PASS22_CFLOATREVERBCONSTANTS_H
#define FABLE_PASS22_CFLOATREVERBCONSTANTS_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
#include "engine/CFloatShortReverbConstants.h"
#include "engine/CReverbConstants.h"
class CFloatReverbConstants : public CReverbConstants {
public:
 CFloatShortReverbConstants m_ShortRvbConstants[4]; // +0x1c
 virtual ~CFloatReverbConstants();
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CFloatReverbConstantsSizePass22[(sizeof(CFloatReverbConstants)==44)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
