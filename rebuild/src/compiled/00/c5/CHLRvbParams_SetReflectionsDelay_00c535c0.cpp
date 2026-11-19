// CHLRvbParams::SetReflectionsDelay(float fReflectionsDelay) -- clamp the value into [0.0f, 0.3f], store it in m_fReflectionsDelay (+0x28) and set update flag 0x2000 in m_uiUpdateFlags
// Leaf: ?SetReflectionsDelay@CHLRvbParams@@QAEXM@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C535C0, 95 bytes, thiscall, `ret 4`.
// Name (NF102, q11564; was a descriptive stand-in, old leaf in RENAMES.tsv): GameCODA 2.10 CodeView public ?SetReflectionsDelay@CHLRvbParams@@QAEXM@Z, a
//   header inline of the SDK (a CODAApi.lib SalSwReverb.obj COMDAT) of which retail calls this out-of-line copy. Two sources:
//   (1) this body is byte-identical (95 bytes, relocations masked) to that CODAApi.lib member, its two bound constants resolving
//   to the same retail words; (2) the SDK CSalSwReverb::Update (889 bytes, the length of retail 0x00C54190) calls it at REL32 +0x169,
//   where retail 0x00C54190 calls 0x00C535C0; the store is CHLRvbParams+0x28 = m_fReflectionsDelay of the CodeView layout (engine/CHLRvbParams.h).
// Callers: 0x00C54190 (CDecodedStream::SetProperties, the SDK CSalSwReverb::Update) and 0x00C54510 (CDecodedStream::Initialise).
// Shape (the landed body, unchanged): one nested ternary, so each branch loads the chosen operand and VC7.1 duplicates the store
//   and the flag update (load / or / store interleaved with the FPU copy) into all three exits; the bounds are pooled float literals.
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CHLRVBPARAMS_SETTERS
#include "engine/CHLRvbParams.h"

void CHLRvbParams::SetReflectionsDelay(float fReflectionsDelay)
{
    m_fReflectionsDelay = fReflectionsDelay<0.0f?0.0f:(fReflectionsDelay>0.3f?0.3f:fReflectionsDelay);
    m_uiUpdateFlags |= 0x2000;
}
