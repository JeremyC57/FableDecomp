// CHLRvbParams::SetEchoDepth(float fEchoDepth) -- clamp the value into [0.0f, 1.0f], store it in m_fEchoDepth (+0x40) and set update flag 0x40000 in m_uiUpdateFlags
// Leaf: ?SetEchoDepth@CHLRvbParams@@QAEXM@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C53860, 95 bytes, thiscall, `ret 4`.
// Name (NF102, q11570; was a descriptive stand-in, old leaf in RENAMES.tsv): GameCODA 2.10 CodeView public ?SetEchoDepth@CHLRvbParams@@QAEXM@Z, a
//   header inline of the SDK (a CODAApi.lib SalSwReverb.obj COMDAT) of which retail calls this out-of-line copy. Two sources:
//   (1) this body is byte-identical (95 bytes, relocations masked) to that CODAApi.lib member, its two bound constants resolving
//   to the same retail words; (2) the SDK CSalSwReverb::Update (889 bytes, the length of retail 0x00C54190) calls it at REL32 +0x2ae,
//   where retail 0x00C54190 calls 0x00C53860; the store is CHLRvbParams+0x40 = m_fEchoDepth of the CodeView layout (engine/CHLRvbParams.h).
// Callers: 0x00C54190 (CDecodedStream::SetProperties, the SDK CSalSwReverb::Update) and 0x00C54510 (CDecodedStream::Initialise).
// Shape (the landed body, unchanged): one nested ternary, so each branch loads the chosen operand and VC7.1 duplicates the store
//   and the flag update (load / or / store interleaved with the FPU copy) into all three exits; the bounds are pooled float literals.
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CHLRVBPARAMS_SETTERS
#include "engine/CHLRvbParams.h"

void CHLRvbParams::SetEchoDepth(float fEchoDepth)
{
    m_fEchoDepth = fEchoDepth < 0.0f ? 0.0f : (fEchoDepth > 1.0f ? 1.0f : fEchoDepth);
    m_uiUpdateFlags |= 0x40000;
}
