// CFloatLongReverb::Clear() -- clear the long reverb's three input filters, then both long-reverb modules (each zeroes its cyclic buffer and clears its feedback filter)
// Leaf: ?Clear@CFloatLongReverb@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C75670, 79 bytes, thiscall, plain `ret`; two counted loops, CFloatFilter::Clear 0x00C744E0
//   called at +0x13 and +0x41.
// Name (NF105, q11544; was the descriptive CReverbDiffuser::Reset over a local model): GameCODA 2.10 CodeView public
//   ?Clear@CFloatLongReverb@@QAEXXZ. Two sources: (1) this body is byte-identical (79 bytes, relocations masked) to that CODAApi.lib
//   member (REL32 +0x13 / +0x41 ?Clear@CFloatFilter@@QAEXXZ = 0x00C744E0); (2) the byte-identical SDK CFloatReverbDspEngine::Clear
//   0x00C69B60 calls it at REL32 +0xe0 on m_LongReverb (+0x144), the retail call site. Layout: the CodeView CFloatLongReverb
//   (engine/CFloatLongReverb.h: m_filter[3] +4, m_longReverbModule[2] +0x40).
// Inline: CFloatLongReverbModule::Clear is the SDK header inline (release COMDAT: buffer memset, then CFloatFilter::Clear).
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATFILTER_APPLY_PASS23
#define FABLE_RETAIL_CFLOATCYCLICBUFFER_CLEAR
#define FABLE_RETAIL_CFLOATLONGREVERBMODULE_CLEAR
#define FABLE_RETAIL_CFLOATLONGREVERB_CLEAR
#include "engine/CFloatLongReverb.h"

void CFloatLongReverb::Clear()
{
    for (int i = 0; i < 3; ++i)
        m_filter[i].Clear();
    for (int j = 0; j < 2; ++j)
        m_longReverbModule[j].Clear();
}
