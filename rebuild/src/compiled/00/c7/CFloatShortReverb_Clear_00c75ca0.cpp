// CFloatShortReverb::Clear() -- clear the four short-reverb modules (each zeroes its cyclic buffer and clears its filter)
// Leaf: ?Clear@CFloatShortReverb@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C75CA0, 58 bytes, thiscall, plain `ret`; CFloatFilter::Clear 0x00C744E0 called at +0x2c.
// Name (NF105, q11547; was the descriptive CReverbTapBank::Reset over a local model): GameCODA 2.10 CodeView public
//   ?Clear@CFloatShortReverb@@QAEXXZ. Two sources: (1) this body is byte-identical (58 bytes, relocations masked) to that CODAApi.lib
//   member (REL32 +0x2c ?Clear@CFloatFilter@@QAEXXZ = 0x00C744E0); (2) the byte-identical SDK CFloatReverbDspEngine::Clear 0x00C69B60
//   calls it at REL32 +0xd5 on m_ShortReverb (+0x94), the retail call site. Layout: the CodeView CFloatShortReverb
//   (engine/CFloatShortReverb.h: m_shortReverbModule[4] +4).
// Inline: CFloatShortReverbModule::Clear is the SDK header inline (release COMDAT: buffer memset, then CFloatFilter::Clear).
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATFILTER_APPLY_PASS23
#define FABLE_RETAIL_CFLOATCYCLICBUFFER_CLEAR
#define FABLE_RETAIL_CFLOATSHORTREVERBMODULE_CLEAR
#define FABLE_RETAIL_CFLOATSHORTREVERB_CLEAR
#include "engine/CFloatShortReverb.h"

void CFloatShortReverb::Clear()
{
    for (int i = 0; i < 4; ++i)
        m_shortReverbModule[i].Clear();
}
