#ifndef FABLE_PASS22_CFLOATDIFFUSER_H
#define FABLE_PASS22_CFLOATDIFFUSER_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
#include "engine/CFloatAllpassSection.h"
class CFloatDiffuser {
public:
 virtual ~CFloatDiffuser();
#ifdef FABLE_RETAIL_CFLOATDIFFUSER_PROCESS
 // Retail opt-in (NF102, q11551): SDK 2.10 CodeView public ?Process@CFloatDiffuser@@QAEXPAMI@Z (native 0x00C75140).
 void Process(float* pfBuffer, unsigned int uiSamples);
#endif
#ifdef FABLE_RETAIL_CFLOATDIFFUSER_CLEAR
 // Retail opt-in (NF105, family of q11545): SDK 2.10 CodeView public ?Clear@CFloatDiffuser@@QAEXXZ (native 0x00C75010; the release
 // CFloatReverbDspEngine::Clear 0x00C69B60 tail-jumps to it at REL32 +0xef).
 void Clear();
#endif
#ifdef FABLE_RETAIL_CFLOATDIFFUSER_CREATE
 // Retail opt-in (NF105, family of q11546): SDK 2.10 CodeView public ?Create@CFloatDiffuser@@QAEXIAAPAXPAH@Z (native 0x00C75050, not
 // landed; the release CFloatReverbDspEngine::CreateComponents 0x00C6A460 calls it at REL32 +0x154).
 void Create(unsigned int,void*&,int*);
#endif
private:
 CFloatAllpassSection m_allpass[2]; // +0x4
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CFloatDiffuserSizePass22[(sizeof(CFloatDiffuser)==36)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
