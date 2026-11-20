// CFloatDiffuser::~CFloatDiffuser() -- destructor of the reverb engine's diffuser: vptr, Release (both allpass sections detached from the engine's pooled memory), then both sections destroyed through the `eh vector destructor iterator`
// Leaf: ??1CFloatDiffuser@@UAE@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C75440, 39 bytes (+ int3 padding), thiscall, plain `ret`.
// Name (NF109, q11607 family; was the descriptive CStereoDelayLine::~CStereoDelayLine over a local model): GameCODA 2.10 CodeView public
//   ??1CFloatDiffuser@@UAE@XZ. Two sources: (1) the CODAApiDbg.lib ??1CFloatDiffuser body is `Release();` followed by ??_M over m_allpass
//   (this+4, 0x10, 2, ??1CFloatAllpassSection), the retail sequence here (Release expanded: +8 / +0xc / +0x18 / +0x1c zeroed; the
//   iterator gets the byte-identical SDK ~CFloatAllpassSection 0x00C75400); (2) the retail engine dtor 0x00C6A780 calls it on
//   this+0x25c, the CodeView CFloatReverbDspEngine::m_diffuser, and it stores the vtable 0x012BF078 whose only slot, 0x00C754A0, calls it.
//   The CODAApi.lib release member (51 bytes) also expands the section dtors (another SDK build, as for 0x00C75410).
// Shape: the expanded Release stores are interleaved with the `__ehvec_dtor` pushes; no EH frame (nothing after the call).
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATALLPASSSECTION_RELEASE
#include "engine/CFloatDiffuser.h"

CFloatDiffuser::~CFloatDiffuser()
{
    // Release() (0x00C75000, the TU-mate the release build expands here), written out as its body: m_allpass[i].Release().
    for (unsigned int i = 0; i < 2; ++i)
        m_allpass[i].Release();
}
