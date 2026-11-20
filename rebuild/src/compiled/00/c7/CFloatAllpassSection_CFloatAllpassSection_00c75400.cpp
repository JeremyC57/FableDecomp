// CFloatAllpassSection::~CFloatAllpassSection() -- destructor of one allpass section of the reverb diffuser: vptr, then Release (buffer pointer and length zeroed; the memory belongs to the engine's pool)
// Leaf: ??1CFloatAllpassSection@@UAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C75400, 15 bytes, thiscall, plain `ret`, no callees.
// Name (NF109, q11607 family; was the descriptive CDelayLine::~CDelayLine over a local model): GameCODA 2.10 CodeView public
//   ??1CFloatAllpassSection@@UAE@XZ. Two sources: (1) this body is byte-identical (15 bytes, relocations masked) to that CODAApi.lib
//   release member (work/tools/sdkfind.py; relocation ??_7CFloatAllpassSection@@6B@ = retail 0x012BF074); (2) the retail CFloatDiffuser
//   ctor 0x00C75410 / dtor 0x00C75440 pass it to the `eh vector` iterators for m_allpass[2], as the CODAApiDbg.lib CFloatDiffuser
//   ctor / dtor do (??_M with ??1CFloatAllpassSection@@UAE@XZ).
// Body: the CODAApiDbg.lib dtor calls Release (header opt-in FABLE_RETAIL_CFLOATALLPASSSECTION_RELEASE: zero m_pBuffer / m_length).
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATALLPASSSECTION_RELEASE
#include "engine/CFloatAllpassSection.h"

CFloatAllpassSection::~CFloatAllpassSection()
{
    Release();
}
