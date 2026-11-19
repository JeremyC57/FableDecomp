#ifndef FABLE_PASS22_CFLOATSHORTREVERBMODULE_H
#define FABLE_PASS22_CFLOATSHORTREVERBMODULE_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
#include "engine/CFloatCyclicBuffer.h"
#include "engine/CFloatFilter.h"
#include "engine/CReverbDspProcessor.h"
class CFloatShortReverbModule : public CReverbDspProcessor {
private:
 CFloatFilter m_Filter; // +0x4
 CFloatCyclicBuffer m_Buffer; // +0x18
 int m_iMyFaderToken; // +0x24
public:
 virtual ~CFloatShortReverbModule();
#ifdef FABLE_RETAIL_CFLOATSHORTREVERBMODULE_PROCESSINPUT
 // Retail opt-in (NF99, q11492): SDK 2.10 CodeView public members the release CFloatShortReverb::Process 0x00C75CE0 uses (CODAApi.lib
 // FloatShortReverb.obj REL32: +0x5e GetFeedbackData 0x00C89B40, +0xe5 MixInputs 0x00C89B60, +0x14d ProcessInput 0x00C89750,
 // +0x192 ProcessCrossfeed 0x00C89AA0). GetFilterHistory (public ?GetFilterHistory@CFloatShortReverbModule@@QAEPAMXZ) is expanded
 // by the release (`lea` of module +8 = m_Filter.m_fHistoryData); the Dbg body calls it out of line.
 float* GetFilterHistory(){return m_Filter.GetFilterHistory();}
 void ProcessInput_GetFeedbackData_SseOptimisation(float*,unsigned int);
 void ProcessInput_MixInputs_SseOptimisation(const struct CFloatShortReverbModuleParams*,const CFloatCyclicBuffer*,float*,float*,unsigned int);
 void ProcessInput(const struct CFloatShortReverbModuleParams*,const CFloatCyclicBuffer*,float*,float*,unsigned int);
 void ProcessCrossfeed(const float*,const float*,const float*,unsigned int);
#endif
#ifdef FABLE_RETAIL_CFLOATSHORTREVERBMODULE_CLEAR
 // Retail opt-in (NF105, family of q11545): SDK 2.10 CodeView public inline ?Clear@CFloatShortReverbModule@@QAEXXZ (CODAApi.lib COMDAT:
 // the cyclic buffer's memset, then a tail call of CFloatFilter::Clear), expanded by the release CFloatShortReverb::Clear.
 void Clear() { m_Buffer.Clear(); m_Filter.Clear(); }
#endif
#ifdef FABLE_RETAIL_CFLOATSHORTREVERBMODULE_CREATE
 // Retail opt-in (NF105, q11549): SDK 2.10 CodeView public ?Create@CFloatShortReverbModule@@QAEXIAAPAXPAHH@Z (native 0x00C896C0).
 void Create(unsigned int,void*&,int*,int);
#endif
#ifdef FABLE_RETAIL_CFLOATSHORTREVERBMODULE_PROCESSINPUT_FADING
 // Retail opt-in (NF105, family of q11518): SDK 2.10 CodeView public inline ?Token@CFloatShortReverbModule@@QBEHXZ (the Dbg fading
 // CFloatShortReverb::Process calls it out of line; the release 0x00C75E90 expands the m_iMyFaderToken load) and the fading overload
 // ?ProcessInput@CFloatShortReverbModule@@QAEXPAUCFloatShortReverbModuleParams@@PBU2@PBVCFloatCyclicBuffer@@PAM3II@Z (native 0x00C89850,
 // not landed; the release 0x00C75E90 calls it at REL32 +0x73).
 int Token() const { return m_iMyFaderToken; }
 void ProcessInput(struct CFloatShortReverbModuleParams*,const struct CFloatShortReverbModuleParams*,const CFloatCyclicBuffer*,float*,float*,unsigned int,unsigned int);
#endif
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CFloatShortReverbModuleSizePass22[(sizeof(CFloatShortReverbModule)==40)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
