#ifndef FABLE_PASS22_CFLOATFILTERPARAMS_H
#define FABLE_PASS22_CFLOATFILTERPARAMS_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
class CFloatFilterParams {
#ifdef FABLE_RETAIL_CREVERBFNSSSE_APPLYFILTERORDER2_QUAD
 friend class CReverbFnsSSE;
#endif
#ifdef FABLE_RETAIL_CFLOATFILTERPARAMS_FRIEND_CFLOATFILTER
 // Retail opt-in (NF109, q11600 / q11601): CFloatFilter::Process (both overloads, native 0x00C74CE0 / 0x00C74D30) reads m_fGain / m_fA /
 // m_fB directly: the CODAApiDbg.lib /Od bodies load params+0 / +4 / +0x10 with no accessor call (GetGain / GetA0 / GetB0 return a
 // float, not the arrays ApplyFilter takes). C++ needs this access grant; the CodeView field list has no friend record.
 friend class CFloatFilter;
#endif
#ifdef FABLE_RETAIL_CFLOATREVERBPARAMETERTRANSLATOR_UPDATEDECAYTIMES
public:
 void Set_ShelfLfHf(float fLFRatio,float fFreqLF,float fHFRatio,float fFreqHF,unsigned int uiSamplesPerSecond);
 void SetGain(float fGain){m_fGain=fGain;}
#endif
#ifdef FABLE_RETAIL_CFLOATFILTERPARAMS_SET_HIGHPASS
public:
 // Retail opt-in (NF99, q11477): SDK 2.10 CodeView public ?Set_HighPass@CFloatFilterParams@@QAEXHMI@Z (native 0x00C74BB0; the release
 // Translate calls it at +0x73).
 void Set_HighPass(int,float,unsigned int);
#endif
#ifdef FABLE_RETAIL_CFLOATFILTERPARAMS_SET_LPBUTTERWORTH2
public:
 // Retail opt-in (NF99, q11477): SDK 2.10 CodeView public ?Set_LpButterworth2@CFloatFilterParams@@QAEXHMI@Z (native 0x00C74AB0; the
 // release Translate calls it at +0x8e).
 void Set_LpButterworth2(int,float,unsigned int);
#endif
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
