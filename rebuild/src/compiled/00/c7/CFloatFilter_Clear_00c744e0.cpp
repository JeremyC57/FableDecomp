// CFloatFilter::Clear() -- reset the filter's four history words (m_fHistoryData[0..3], +0x04..+0x10) to 1.0f
// Leaf: ?Clear@CFloatFilter@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C744E0, 18 bytes (+ int3 padding), thiscall, plain `ret`.
// Name (NF102, q11543; was the descriptive CDspGain::Reset over a local model): GameCODA 2.10 CodeView public
//   ?Clear@CFloatFilter@@QAEXXZ (engine/CFloatFilter.h, opt-in FABLE_RETAIL_CFLOATFILTER_APPLY_PASS23). Two sources: (1) this body is
//   byte-identical (18 bytes) to that CODAApi.lib FloatFilter.obj member; (2) the byte-identical SDK
//   CFloatReverbDspEngine::ProcessReflectionsInputBuffer 0x00C69C60 calls it at REL32 +0xfa / +0x105 and the byte-identical SDK
//   CFloatLongReverb::Clear 0x00C75670 at +0x13 / +0x41, the retail call sites of 0x00C744E0.
// Retail bytes:  mov eax,3F800000h ; mov [ecx+4],eax ; mov [ecx+8],eax ; mov [ecx+0Ch],eax ; mov [ecx+10h],eax ; ret
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATFILTER_APPLY_PASS23
#include "engine/CFloatFilter.h"

void CFloatFilter::Clear()
{
    m_fHistoryData[0] = 1.0f;
    m_fHistoryData[1] = 1.0f;
    m_fHistoryData[2] = 1.0f;
    m_fHistoryData[3] = 1.0f;
}
