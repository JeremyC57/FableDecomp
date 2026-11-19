#ifndef FABLE_PASS22_CFLOATPITCH_H
#define FABLE_PASS22_CFLOATPITCH_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
#include "engine/CFloatCyclicBuffer.h"
#include "engine/CReverbDspProcessor.h"
class CFloatPitch : public CReverbDspProcessor {
public:
 virtual ~CFloatPitch();
#ifdef FABLE_RETAIL_CFLOATPITCH_CLEAR
 // Retail opt-in (NF105, family of q11545): SDK 2.10 CodeView public inline ?Clear@CFloatPitch@@QAEXXZ (CODAApi.lib COMDAT: clears
 // its cyclic buffer), expanded by the release CFloatReverbDspEngine::Clear 0x00C69B60.
 void Clear() { m_Buffer.Clear(); }
#endif
#ifdef FABLE_RETAIL_CFLOATPITCH_CREATE
 // Retail opt-in (NF105, family of q11546): SDK 2.10 CodeView public ?Create@CFloatPitch@@QAEXIAAPAXPAHH@Z (native 0x00C76DD0, not landed;
 // the release CFloatReverbDspEngine::CreateComponents 0x00C6A460 calls it at REL32 +0x136).
 void Create(unsigned int,void*&,int*,int);
#endif
#ifdef FABLE_RETAIL_CFLOATPITCH_PROCESS
 // Retail opt-in (NF105, family of q11521): SDK 2.10 CodeView public inline ?Token@CFloatPitch@@QBEHXZ and the two Process overloads
 // ?Process@CFloatPitch@@QAEXPBUCFloatPitchShiftParams@@PBMPAMI@Z (native 0x00C76E90) / ?Process@CFloatPitch@@QAEXPAUCFloatPitchShiftParams@@PBU2@PBMPAMII@Z
 // (native 0x00C770B0); the release CFloatReverbDspEngine::ProcessShortReverbInputBuffer 0x00C6A080 calls them at REL32 +0x67 / +0x4d.
 int Token() const { return m_iMyFaderToken; }
 void Process(const struct CFloatPitchShiftParams*,const float*,float*,unsigned int);
 void Process(struct CFloatPitchShiftParams*,const struct CFloatPitchShiftParams*,const float*,float*,unsigned int,unsigned int);
#endif
private:
 CFloatCyclicBuffer m_Buffer; // +0x4
 float m_fControlPoint; // +0x10
 int m_iMyFaderToken; // +0x14
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CFloatPitchSizePass22[(sizeof(CFloatPitch)==24)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
