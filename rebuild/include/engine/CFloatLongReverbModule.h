#ifndef FABLE_PASS22_CFLOATLONGREVERBMODULE_H
#define FABLE_PASS22_CFLOATLONGREVERBMODULE_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
#include "engine/CFloatCyclicBuffer.h"
#include "engine/CFloatFilter.h"
#include "engine/CReverbDspProcessor.h"
class CFloatLongReverbModule : public CReverbDspProcessor {
private:
 CFloatCyclicBuffer m_Buffer; // +0x4
 CFloatFilter m_Filter; // +0x10
 int m_iMyFaderToken; // +0x24
public:
 virtual ~CFloatLongReverbModule();
#ifdef FABLE_RETAIL_CFLOATLONGREVERBMODULE_PROCESS
 // Retail opt-in (NF102, q11558): SDK 2.10 CodeView public
 // ?Process@CFloatLongReverbModule@@QAEXPBUCFloatLongReverbModuleParams@@PBVCFloatCyclicBuffer@@PAM2I@Z (native 0x00C891A0).
 void Process(const struct CFloatLongReverbModuleParams*,const CFloatCyclicBuffer*,float*,float*,unsigned int);
#endif
#ifdef FABLE_RETAIL_CFLOATLONGREVERBMODULE_PROCESS_FADING
 // Retail opt-in (NF105, family of q11492): SDK 2.10 CodeView public fading overload
 // ?Process@CFloatLongReverbModule@@QAEXPAUCFloatLongReverbModuleParams@@PBU2@PBVCFloatCyclicBuffer@@PAM3II@Z (native 0x00C89350).
 void Process(struct CFloatLongReverbModuleParams*,const struct CFloatLongReverbModuleParams*,const CFloatCyclicBuffer*,float*,float*,unsigned int,unsigned int);
#endif
#ifdef FABLE_RETAIL_CFLOATLONGREVERBMODULE_CLEAR
 // Retail opt-in (NF105, family of q11545): SDK 2.10 CodeView public inline ?Clear@CFloatLongReverbModule@@QAEXXZ (CODAApi.lib COMDAT:
 // the cyclic buffer's memset, then a tail call of CFloatFilter::Clear), expanded by the release CFloatLongReverb::Clear.
 void Clear() { m_Buffer.Clear(); m_Filter.Clear(); }
#endif
#ifdef FABLE_RETAIL_CFLOATLONGREVERBMODULE_CREATE
 // Retail opt-in (NF105, q11548): SDK 2.10 CodeView public ?Create@CFloatLongReverbModule@@QAEXIAAPAXPAHH@Z (native 0x00C89110).
 void Create(unsigned int,void*&,int*,int);
#endif
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CFloatLongReverbModuleSizePass22[(sizeof(CFloatLongReverbModule)==40)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
