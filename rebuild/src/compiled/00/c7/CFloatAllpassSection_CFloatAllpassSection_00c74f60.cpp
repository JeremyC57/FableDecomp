// CFloatAllpassSection::CFloatAllpassSection() -- constructor of one allpass section of the reverb diffuser: vptr, buffer pointer and length zeroed (the index is left as is)
// Leaf: ??0CFloatAllpassSection@@QAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C74F60, 17 bytes, thiscall, no callees.
// Name (NF109, q11607 family; was the descriptive CDelayLine::CDelayLine over a local model): GameCODA 2.10 CodeView public
//   ??0CFloatAllpassSection@@QAE@XZ. Two sources: (1) this body is byte-identical (17 bytes, relocations masked) to that CODAApi.lib
//   release member (work/tools/sdkfind.py; its one relocation is ??_7CFloatAllpassSection@@6B@ = retail 0x012BF074; the identical
//   CCodaObject ctor stores another vtable); (2) the retail CFloatDiffuser ctor 0x00C75410 passes it with ~CFloatAllpassSection
//   0x00C75400 to the `eh vector constructor iterator` for m_allpass[2] (+4, stride 0x10), as the CODAApiDbg.lib ??0CFloatDiffuser does.
// Body: the CODAApiDbg.lib ctor stores 0 to m_pBuffer (+4) and m_length (+8); the landed initialiser-list shape is kept.
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATALLPASSSECTION_CTOR
#include "engine/CFloatAllpassSection.h"

CFloatAllpassSection::CFloatAllpassSection()
    : m_pBuffer(0), m_length(0)
{
}
