#ifndef FABLE_PASS22_CFLOATSHORTREVERB_H
#define FABLE_PASS22_CFLOATSHORTREVERB_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
#include "engine/CFloatShortReverbModule.h"
#include "engine/CReverbDspProcessor.h"
class CFloatShortReverb : public CReverbDspProcessor {
private:
 CFloatShortReverbModule m_shortReverbModule[4]; // +0x4
 unsigned int m_uiNumOutputs; // +0xa4
 int m_iMyFaderTokenMin; // +0xa8
 int m_iMyFaderTokenMax; // +0xac
public:
 virtual ~CFloatShortReverb();
#ifdef FABLE_RETAIL_CFLOATSHORTREVERB_PROCESS
 // Retail opt-in (NF99, q11492): SDK 2.10 CodeView public
 // ?Process@CFloatShortReverb@@QAEXPAUCFloatShortReverbParams@@PBVCFloatCyclicBuffer@@PAMPAPAMI@Z (native 0x00C75CE0).
 void Process(struct CFloatShortReverbParams*,const CFloatCyclicBuffer*,float*,float**,unsigned int);
#endif
#ifdef FABLE_RETAIL_CFLOATSHORTREVERB_PROCESSFADE
 // Retail opt-in (NF99, q11493): SDK 2.10 CodeView public members the release CFloatReverbDspEngine::ProcessShortReverb 0x00C6A180 uses:
 // the fading overload ?Process@CFloatShortReverb@@QAEXPAUCFloatShortReverbParams@@PBU2@PBVCFloatCyclicBuffer@@PAMPAPAMHII@Z (REL32
 // +0x4f, native 0x00C75E90) and the const ?MatchToken@CFloatShortReverb@@QBE_NH@Z (the Dbg body calls it; the release expands the
 // two token compares `>= m_iMyFaderTokenMin` / `< m_iMyFaderTokenMax`).
 void Process(struct CFloatShortReverbParams*,const struct CFloatShortReverbParams*,const CFloatCyclicBuffer*,float*,float**,int,unsigned int,unsigned int);
 bool MatchToken(int iToken) const { return iToken >= m_iMyFaderTokenMin && iToken < m_iMyFaderTokenMax; }
#endif
#ifdef FABLE_RETAIL_CFLOATSHORTREVERB_CLEAR
 // Retail opt-in (NF105, q11547): SDK 2.10 CodeView public ?Clear@CFloatShortReverb@@QAEXXZ (native 0x00C75CA0; the release
 // CFloatReverbDspEngine::Clear 0x00C69B60 calls it at REL32 +0xd5).
 void Clear();
#endif
#ifdef FABLE_RETAIL_CFLOATSHORTREVERB_CREATE
 // Retail opt-in (NF105, q11549 family): SDK 2.10 CodeView public ?Create@CFloatShortReverb@@QAEXIPBUCFloatShortReverbConstants@@AAPAXPAHAAH@Z
 // (native 0x00C75C30; the release CFloatReverbDspEngine::CreateComponents 0x00C6A460 calls it at REL32 +0xe2).
 void Create(unsigned int,const struct CFloatShortReverbConstants*,void*&,int*,int&);
#endif
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CFloatShortReverbSizePass22[(sizeof(CFloatShortReverb)==176)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
