// CFloatReverbDspEngine::Clear() -- virtual: once the engine's delay memory exists (m_pvData, +0x280), clear both input filters, zero the reflections input buffer, the reverb buffer, the 128-float long-reverb crossfeed block, the echo, the pitch shifter and the four reflections, then clear the short reverb, the long reverb and the diffuser
// Leaf: ?Clear@CFloatReverbDspEngine@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C69B60, 245 bytes, thiscall, plain `ret` (the diffuser clear is a tail jump). The zeroing
//   is the memset intrinsic (`shl ecx,2 ; rep stosd ; rep stosb` per counted float buffer, `rep stosd` 0x80 for the scratch block).
// Name (NF105, q11545; was the descriptive CReverbProcessor::Reset over local models): GameCODA 2.10 CodeView public virtual
//   ?Clear@CFloatReverbDspEngine@@UAEXXZ. Two sources: (1) this body is byte-identical (245 bytes, relocations masked) to that
//   CODAApi.lib FloatReverbDspEngine.obj member, whose relocations name the retail callees (+0x18 / +0x23 CFloatFilter::Clear
//   0x00C744E0, +0xd5 CFloatShortReverb::Clear 0x00C75CA0, +0xe0 CFloatLongReverb::Clear 0x00C75670, +0xef CFloatDiffuser::Clear
//   0x00C75010); (2) it is slot 3 of the engine vtable 0x012B9594, the CodeView CReverbDspEngine::Clear slot (engine/CReverbDspEngine.h:
//   ~dtor, Commit, Process, Clear, ProcessReverb). Layout: the CodeView CFloatReverbDspEngine (engine/CFloatReverbDspEngine.h).
// Inlines: CFloatCyclicBuffer::Clear and CFloatEcho / CFloatPitch / CFloatReflection::Clear are the SDK header inlines (release
//   COMDATs in CODAApi.lib: the counted memset of each cyclic buffer), expanded here as in the SDK body.
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATFILTER_APPLY_PASS23
#define FABLE_RETAIL_CFLOATCYCLICBUFFER_CLEAR
#define FABLE_RETAIL_CFLOATECHO_CLEAR
#define FABLE_RETAIL_CFLOATPITCH_CLEAR
#define FABLE_RETAIL_CFLOATREFLECTION_CLEAR
#define FABLE_RETAIL_CFLOATSHORTREVERB_CLEAR
#define FABLE_RETAIL_CFLOATLONGREVERB_CLEAR
#define FABLE_RETAIL_CFLOATDIFFUSER_CLEAR
#include "engine/CFloatReverbDspEngine.h"

void CFloatReverbDspEngine::Clear()
{
    if (!m_pvData)
        return;
    m_inputFilterLowPass.Clear();
    m_inputFilterHighPass.Clear();
    m_reflectionsInputBuffer.Clear();
    m_reverbBuffer.Clear();
    memset(m_pfLongRvbXfeed, 0, 128 * sizeof(float));
    m_echo.Clear();
    m_Pitch.Clear();
    for (int i = 0; i < 4; ++i)
        m_reflection[i].Clear();
    m_ShortReverb.Clear();
    m_LongReverb.Clear();
    m_diffuser.Clear();
}
