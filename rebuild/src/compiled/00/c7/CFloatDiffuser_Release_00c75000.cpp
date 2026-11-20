// CFloatDiffuser::Release() -- detach both allpass sections of the diffuser from the engine's pooled sample memory: each section's buffer pointer and length zeroed (the index is kept)
// Leaf: ?Release@CFloatDiffuser@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C75000, 15 bytes (+ int3 padding), thiscall, plain `ret`, no calls (both section releases expanded).
// Name (NF109, q11607; was the descriptive CStereoDelayLine::Detach over a local model): GameCODA 2.10 CodeView public
//   ?Release@CFloatDiffuser@@QAEXXZ. Two sources: (1) this body is byte-identical (15 bytes) to that CODAApi.lib FloatDiffuser.obj
//   release member (work/tools/sdkfind.py); (2) the byte-identical SDK CFloatReverbDspEngine::Destroy 0x00C6A3D0 tail-jumps to it at
//   REL32 +0x36 and the byte-identical SDK CFloatReverbDspEngine::Create 0x00C6A8B0 calls it at REL32 +0x56, both on m_diffuser (+0x25c),
//   the retail call sites. Layout: the CodeView CFloatDiffuser (m_allpass[2] +4, stride 0x10) and CFloatAllpassSection (m_pBuffer +4,
//   m_length +8, m_index +0xc).
// Shape: the CODAApiDbg.lib body loops `i < 2` (unsigned compare) calling CFloatAllpassSection::Release (stores 0 to m_pBuffer and
//   m_length); the release expands both calls (header opt-in FABLE_RETAIL_CFLOATALLPASSSECTION_RELEASE, body from the release member).
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATALLPASSSECTION_RELEASE
#define FABLE_RETAIL_CFLOATDIFFUSER_RELEASE
#include "engine/CFloatDiffuser.h"

void CFloatDiffuser::Release()
{
    for (unsigned int i = 0; i < 2; ++i)
        m_allpass[i].Release();
}
