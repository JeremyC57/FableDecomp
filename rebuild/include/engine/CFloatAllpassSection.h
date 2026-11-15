#ifndef FABLE_PASS22_CFLOATALLPASSSECTION_H
#define FABLE_PASS22_CFLOATALLPASSSECTION_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
class CFloatAllpassSection {
public:
 virtual ~CFloatAllpassSection();
private:
 float * m_pBuffer; // +0x4
 unsigned int m_length; // +0x8
 unsigned int m_index; // +0xc
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CFloatAllpassSectionSizePass22[(sizeof(CFloatAllpassSection)==16)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
