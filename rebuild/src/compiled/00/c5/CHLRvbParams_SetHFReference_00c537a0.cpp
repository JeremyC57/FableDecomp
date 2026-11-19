// CHLRvbParams::SetHFReference(float fHFReference) -- clamp the value into [100.0f, 20000.0f], store it in m_fRefFrequencyHF (+0x20) and set update flag 0x10 in m_uiUpdateFlags
// Leaf: ?SetHFReference@CHLRvbParams@@QAEXM@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C537A0, 89 bytes, thiscall, `ret 4`.
// Name (NF102, q11568; was a descriptive stand-in, old leaf in RENAMES.tsv): GameCODA 2.10 CodeView public ?SetHFReference@CHLRvbParams@@QAEXM@Z, a
//   header inline of the SDK (a CODAApi.lib SalSwReverb.obj COMDAT) of which retail calls this out-of-line copy. Two sources:
//   (1) this body is byte-identical (89 bytes, relocations masked) to that CODAApi.lib member, its two bound constants resolving
//   to the same retail words; (2) the SDK CSalSwReverb::Update (889 bytes, the length of retail 0x00C54190) calls it at REL32 +0x361,
//   where retail 0x00C54190 calls 0x00C537A0; the store is CHLRvbParams+0x20 = m_fRefFrequencyHF of the CodeView layout (engine/CHLRvbParams.h).
// Callers: 0x00C54190 (CDecodedStream::SetProperties, the SDK CSalSwReverb::Update) and 0x00C54510 (CDecodedStream::Initialise).
// Shape (the landed body, unchanged): an inline Clamp returning by value (descriptive helper, no retail symbol); VC7.1 duplicates
//   the store and the flag update into each of the three exits (`v < lo` -> `test ah,5 ; jp`, `v > hi` -> `test ah,41h ; jne`).
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CHLRVBPARAMS_SETTERS
#include "engine/CHLRvbParams.h"

inline float Clamp(float value, float lo, float hi)
{
    if (value < lo)
        return lo;
    if (value > hi)
        return hi;
    return value;
}

void CHLRvbParams::SetHFReference(float fHFReference)
{
    m_fRefFrequencyHF = Clamp(fHFReference, 100.0f, 20000.0f);
    m_uiUpdateFlags |= 0x10;
}
