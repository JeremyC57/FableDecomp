// CFloatDiffuser::Clear() -- clear both allpass sections of the diffuser (zero each buffer when it exists, reset its index)
// Leaf: ?Clear@CFloatDiffuser@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C75010, 61 bytes, thiscall, plain `ret`, no calls (both section clears expanded).
// Name (NF105, family of q11545; was the descriptive CStereoDelayLine::Clear over a local model): GameCODA 2.10 CodeView public
//   ?Clear@CFloatDiffuser@@QAEXXZ. Two sources: (1) this body is byte-identical (61 bytes) to that CODAApi.lib FloatDiffuser.obj
//   member; (2) the byte-identical SDK CFloatReverbDspEngine::Clear 0x00C69B60 tail-jumps to it at REL32 +0xef on m_diffuser (+0x25c),
//   the retail call site. Layout: the CodeView CFloatDiffuser (m_allpass[2] +4, stride 0x10).
// Inline: CFloatAllpassSection::Clear is the SDK header inline (release COMDAT: memset when m_pBuffer, then m_index = 0).
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATALLPASSSECTION_CLEAR
#define FABLE_RETAIL_CFLOATDIFFUSER_CLEAR
#include "engine/CFloatDiffuser.h"

void CFloatDiffuser::Clear()
{
    for (int i = 0; i < 2; ++i)
        m_allpass[i].Clear();
}
