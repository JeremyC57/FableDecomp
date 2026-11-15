#ifndef FABLE_PASS22_CFLOATFILTERPARAMS_H
#define FABLE_PASS22_CFLOATFILTERPARAMS_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
class CFloatFilterParams {
private:
 float m_fGain; // +0x0
 float m_fA[3]; // +0x4
 float m_fB[3]; // +0x10
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CFloatFilterParamsSizePass22[(sizeof(CFloatFilterParams)==28)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
