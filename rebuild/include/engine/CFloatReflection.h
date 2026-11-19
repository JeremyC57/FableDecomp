#ifndef FABLE_PASS22_CFLOATREFLECTION_H
#define FABLE_PASS22_CFLOATREFLECTION_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
#include "engine/CFloatCyclicBuffer.h"
#include "engine/CReverbDspProcessor.h"
class CFloatReflection : public CReverbDspProcessor {
private:
 CFloatCyclicBuffer m_Buffer; // +0x4
 int m_iMyFaderToken; // +0x10
public:
 virtual ~CFloatReflection();
#ifdef FABLE_RETAIL_CFLOATREFLECTION_PROCESS
 // Retail opt-in (NF102, q11550): SDK 2.10 CodeView public
 // ?Process@CFloatReflection@@QAEXPBUCFloatReflectionParams@@PAVCFloatCyclicBuffer@@PAM2I@Z (native 0x00C75FF0).
 void Process(const struct CFloatReflectionParams*,CFloatCyclicBuffer*,float*,float*,unsigned int);
#endif
#ifdef FABLE_RETAIL_CFLOATREFLECTION_CLEAR
 // Retail opt-in (NF105, family of q11545): SDK 2.10 CodeView public inline ?Clear@CFloatReflection@@QAEXXZ (CODAApi.lib COMDAT: clears
 // its cyclic buffer), expanded by the release CFloatReverbDspEngine::Clear 0x00C69B60.
 void Clear() { m_Buffer.Clear(); }
#endif
#ifdef FABLE_RETAIL_CFLOATREFLECTION_CREATE
 // Retail opt-in (NF105, family of q11546): SDK 2.10 CodeView public ?Create@CFloatReflection@@QAEXIAAPAXPAHH@Z (native 0x00C75F80; the
 // release CFloatReverbDspEngine::CreateComponents 0x00C6A460 calls it at REL32 +0xb1).
 void Create(unsigned int,void*&,int*,int);
#endif
#ifdef FABLE_RETAIL_CFLOATREFLECTION_PROCESS_FADING
 // Retail opt-in (NF105, family of q11520): SDK 2.10 CodeView public inline ?Token@CFloatReflection@@QBEHXZ and the fading overload
 // ?Process@CFloatReflection@@QAEXPAUCFloatReflectionParams@@PBU2@PAVCFloatCyclicBuffer@@PAM3II@Z (native 0x00C76120, not landed; the
 // release CFloatReverbDspEngine::ProcessReflections 0x00C69FB0 calls it at REL32 +0x80).
 int Token() const { return m_iMyFaderToken; }
 void Process(struct CFloatReflectionParams*,const struct CFloatReflectionParams*,CFloatCyclicBuffer*,float*,float*,unsigned int,unsigned int);
#endif
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CFloatReflectionSizePass22[(sizeof(CFloatReflection)==20)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
