#ifndef FABLE_PASS22_CFLOATLONGREVERB_H
#define FABLE_PASS22_CFLOATLONGREVERB_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
#include "engine/CFloatFilter.h"
#include "engine/CFloatLongReverbModule.h"
#include "engine/CReverbDspProcessor.h"
class CReverbConstants;
class CFloatLongReverb : public CReverbDspProcessor {
private:
 CFloatFilter m_filter[3]; // +0x4
 CFloatLongReverbModule m_longReverbModule[2]; // +0x40
 CReverbConstants *m_pRvbConstants; // +0x90
 int m_iMyFaderTokenMin; // +0x94
 int m_iMyFaderTokenMax; // +0x98
public:
 virtual ~CFloatLongReverb();
#ifdef FABLE_RETAIL_CFLOATLONGREVERB_CLEAR
 // Retail opt-in (NF105, q11544): SDK 2.10 CodeView public ?Clear@CFloatLongReverb@@QAEXXZ (native 0x00C75670; the release
 // CFloatReverbDspEngine::Clear 0x00C69B60 calls it at REL32 +0xe0).
 void Clear();
#endif
#ifdef FABLE_RETAIL_CFLOATLONGREVERB_CREATE
 // Retail opt-in (NF105, family of q11546): SDK 2.10 CodeView public ?Create@CFloatLongReverb@@QAEXPAVCReverbConstants@@AAPAXPAHAAH@Z
 // (native 0x00C755B0, not landed; the release CFloatReverbDspEngine::CreateComponents 0x00C6A460 calls it at REL32 +0xf9).
 void Create(CReverbConstants*,void*&,int*,int&);
#endif
#ifdef FABLE_RETAIL_CFLOATLONGREVERB_PROCESSINPUTFILTER_FADING
private:
 // Retail opt-in (NF105, family of q11557): SDK 2.10 CodeView private
 // ?ProcessInputFilter@CFloatLongReverb@@AAEXIPAUCFloatLongReverbParams@@PBU2@PAVCFloatCyclicBuffer@@II@Z (the fading overload, native
 // 0x00C75820; the release CFloatLongReverb::Process fading overload 0x00C75AB0 calls it at REL32 +0x38).
 void ProcessInputFilter(unsigned int,struct CFloatLongReverbParams*,const struct CFloatLongReverbParams*,CFloatCyclicBuffer*,unsigned int,unsigned int);
public:
#endif
#ifdef FABLE_RETAIL_CFLOATLONGREVERB_PROCESS
 // Retail opt-in (NF105, family of q11522): SDK 2.10 CodeView public ?MatchToken@CFloatLongReverb@@QBE_NH@Z (the Dbg
 // CFloatReverbDspEngine::ProcessLongReverb calls it; the release 0x00C6A200 expands the two token compares) and the two Process
 // overloads ?Process@CFloatLongReverb@@QAEXPBUCFloatLongReverbParams@@PAVCFloatCyclicBuffer@@PAMPAPAMI@Z (native 0x00C75A20) /
 // ?Process@CFloatLongReverb@@QAEXPAUCFloatLongReverbParams@@PBU2@PAVCFloatCyclicBuffer@@PAMPAPAMHII@Z (native 0x00C75AB0); the release
 // 0x00C6A200 calls them at REL32 +0x76 / +0x4f.
 bool MatchToken(int iToken) const { return iToken >= m_iMyFaderTokenMin && iToken < m_iMyFaderTokenMax; }
 void Process(const struct CFloatLongReverbParams*,CFloatCyclicBuffer*,float*,float**,unsigned int);
 void Process(struct CFloatLongReverbParams*,const struct CFloatLongReverbParams*,CFloatCyclicBuffer*,float*,float**,int,unsigned int,unsigned int);
#endif
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CFloatLongReverbSizePass22[(sizeof(CFloatLongReverb)==156)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
