#ifndef FABLE_PASS22_CFLOATDIFFUSER_H
#define FABLE_PASS22_CFLOATDIFFUSER_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
#include "engine/CFloatAllpassSection.h"
class CFloatDiffuser {
public:
 virtual ~CFloatDiffuser();
private:
 CFloatAllpassSection m_allpass[2]; // +0x4
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CFloatDiffuserSizePass22[(sizeof(CFloatDiffuser)==36)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
