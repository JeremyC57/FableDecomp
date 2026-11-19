// CFloatReflection::Create(unsigned int uiLength, void*& pvMemory, int* piBytes, int iToken) -- two-pass layout of one early reflection: when memory is supplied, carve `uiLength` floats for its cyclic buffer, advance the memory cursor, zero the buffer and take the fader token; always add the buffer's bytes to the running total
// Leaf: ?Create@CFloatReflection@@QAEXIAAPAXPAHH@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C75F80, 111 bytes, thiscall `ret 10h`, no calls.
// Name (NF105, family of q11546; was the descriptive CCombFilter::Allocate over a local model): GameCODA 2.10 CodeView public
//   ?Create@CFloatReflection@@QAEXIAAPAXPAHH@Z. Two sources: (1) this body is byte-identical (111 bytes, relocations masked) to that
//   CODAApi.lib FloatReflection.obj member; (2) the byte-identical SDK CFloatReverbDspEngine::CreateComponents 0x00C6A460 calls it at
//   REL32 +0xb1 for each of its four m_reflection[] (+0x1f8, stride 0x14), the retail call site. Layout: the CodeView CFloatReflection
//   (m_Buffer +4, m_iMyFaderToken +0x10).
// Shape: the landed order (memory cursor advanced before the buffer is set up); CFloatCyclicBuffer::Create / Clear are the SDK
//   header inlines.
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATCYCLICBUFFER_CLEAR
#define FABLE_RETAIL_CFLOATCYCLICBUFFER_CREATE
#define FABLE_RETAIL_CFLOATREFLECTION_CREATE
#include "engine/CFloatReflection.h"

void CFloatReflection::Create(unsigned int uiLength, void*& pvMemory, int* piBytes, int iToken)
{
    if (pvMemory) {
        float* pfBase = static_cast<float*>(pvMemory);
        pvMemory = pfBase + uiLength;
        m_Buffer.Create(pfBase, uiLength);
        m_Buffer.Clear();
        m_iMyFaderToken = iToken;
    }
    *piBytes += uiLength * sizeof(float);
}
