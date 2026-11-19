#ifndef FABLE_PASS22_CFLOATECHO_H
#define FABLE_PASS22_CFLOATECHO_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
#include "engine/CFloatCyclicBuffer.h"
#include "engine/CReverbDspProcessor.h"
class CFloatEcho : public CReverbDspProcessor {
public:
 virtual ~CFloatEcho();
#ifdef FABLE_RETAIL_CFLOATECHO_CLEAR
 // Retail opt-in (NF105, family of q11545): SDK 2.10 CodeView public inline ?Clear@CFloatEcho@@QAEXXZ (CODAApi.lib COMDAT: clears
 // its cyclic buffer), expanded by the release CFloatReverbDspEngine::Clear 0x00C69B60.
 void Clear() { m_buffer.Clear(); }
#endif
#ifdef FABLE_RETAIL_CFLOATECHO_CREATE
 // Retail opt-in (NF105, family of q11546): SDK 2.10 CodeView public ?Create@CFloatEcho@@QAEXIAAPAXPAHH@Z (native 0x00C76AB0, not landed;
 // the release CFloatReverbDspEngine::CreateComponents 0x00C6A460 calls it at REL32 +0x115).
 void Create(unsigned int,void*&,int*,int);
#endif
#ifdef FABLE_RETAIL_CFLOATECHO_PROCESS
 // Retail opt-in (NF105, q11557): SDK 2.10 CodeView public ?Process@CFloatEcho@@QAEXPBUCFloatEchoParams@@PBMPAMI@Z (native 0x00C76B60) and
 // the fading overload ?Process@CFloatEcho@@QAEXPAUCFloatEchoParams@@PBU2@PBMPAMII@Z (native 0x00C76C20); the release
 // CFloatReverbDspEngine::ProcessShortReverbInputBuffer 0x00C6A080 calls them at REL32 +0xaf / +0x9e.
 void Process(const struct CFloatEchoParams*,const float*,float*,unsigned int);
 void Process(struct CFloatEchoParams*,const struct CFloatEchoParams*,const float*,float*,unsigned int,unsigned int);
#endif
#ifdef FABLE_RETAIL_CFLOATECHO_TOKEN
 // Retail opt-in (NF105, family of q11521): SDK 2.10 CodeView public inline ?Token@CFloatEcho@@QBEHXZ (the Dbg
 // CFloatReverbDspEngine::ProcessShortReverbInputBuffer calls it; the release 0x00C6A080 expands the m_iMyFaderToken load).
 int Token() const { return m_iMyFaderToken; }
#endif
private:
 CFloatCyclicBuffer m_buffer; // +0x4
 int m_iMyFaderToken; // +0x10
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CFloatEchoSizePass22[(sizeof(CFloatEcho)==20)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
