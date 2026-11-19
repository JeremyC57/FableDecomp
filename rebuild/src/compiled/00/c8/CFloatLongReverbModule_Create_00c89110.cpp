// CFloatLongReverbModule::Create(unsigned int uiLength, void*& pvMemory, int* piBytes, int iToken) -- two-pass layout of one long-reverb module: round its length up (plus one block, multiple of 4) and, when memory is supplied, take the fader token, carve and zero the cyclic buffer, advance the memory cursor and clear the feedback filter; always add the buffer's bytes to the running total
// Leaf: ?Create@CFloatLongReverbModule@@QAEXIAAPAXPAHH@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C89110, 142 bytes, thiscall `ret 10h`; CFloatFilter::Clear 0x00C744E0 called at +0x5b.
// Name (NF105, q11548; was the descriptive CReverbDiffuserStage::Allocate over a local model): GameCODA 2.10 CodeView public
//   ?Create@CFloatLongReverbModule@@QAEXIAAPAXPAHH@Z. Two sources: (1) this body is byte-identical (142 bytes, relocations masked) to
//   that CODAApi.lib member (REL32 +0x5c ?Clear@CFloatFilter@@QAEXXZ = 0x00C744E0); (2) the CODAApiDbg.lib body has the same statement
//   order (fader token, CFloatCyclicBuffer::Create + Clear, memory advance, filter, byte total) and the SDK CFloatLongReverb::Create
//   (0x00C755B0, not landed) is its caller. Layout: the CodeView CFloatLongReverbModule (m_Buffer +4, m_Filter +0x10,
//   m_iMyFaderToken +0x24).
// Shape: the Dbg build calls the header inline CFloatFilter::Create (release COMDAT `jmp Clear`); the release relocation at this call
//   names ?Clear@CFloatFilter@@QAEXXZ, so the row calls Clear. CFloatCyclicBuffer::Create / Clear are the SDK header inlines.
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATFILTER_APPLY_PASS23
#define FABLE_RETAIL_CFLOATCYCLICBUFFER_CLEAR
#define FABLE_RETAIL_CFLOATCYCLICBUFFER_CREATE
#define FABLE_RETAIL_CFLOATLONGREVERBMODULE_CREATE
#include "engine/CFloatLongReverbModule.h"

void CFloatLongReverbModule::Create(unsigned int uiLength, void*& pvMemory, int* piBytes, int iToken)
{
    unsigned int uiSize = (uiLength + 131) & ~3u;
    if (pvMemory) {
        m_iMyFaderToken = iToken;
        m_Buffer.Create(static_cast<float*>(pvMemory), uiSize);
        m_Buffer.Clear();
        pvMemory = static_cast<float*>(pvMemory) + uiSize;
        m_Filter.Clear();
    }
    *piBytes += uiSize * sizeof(float);
}
