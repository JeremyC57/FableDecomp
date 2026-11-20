// CFloatDiffuser::CFloatDiffuser() -- constructor of the reverb engine's diffuser (CFloatReverbDspEngine::m_diffuser, +0x25c): vptr, then both allpass sections through the `eh vector constructor iterator`
// Leaf: ??0CFloatDiffuser@@QAE@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C75410, 36 bytes, thiscall. Callee: `eh vector constructor iterator' 0x00F3FB96 (this+4, 0x10, 2,
//   CFloatAllpassSection ctor 0x00C74F60, dtor 0x00C75400).
// Name (NF109, q11607 family; was the descriptive CStereoDelayLine::CStereoDelayLine over a local model): GameCODA 2.10 CodeView public
//   ??0CFloatDiffuser@@QAE@XZ. Two sources: (1) the CodeView CFloatReverbDspEngine layout puts CFloatDiffuser m_diffuser at +0x25c, and the
//   retail engine ctor 0x00C6A650 calls this on this+0x25c; (2) it stores the vtable 0x012BF078 whose only slot is 0x00C754A0
//   (??_GCFloatDiffuser) and builds m_allpass[2] (+4, stride 0x10) with the byte-identical SDK ??0 / ??1CFloatAllpassSection 0x00C74F60 /
//   0x00C75400, the shape of the CODAApiDbg.lib ??0CFloatDiffuser (vector ctor iterator, empty body). The CODAApi.lib release member
//   (41 bytes) expands the section ctors instead: retail linked a build with out-of-line section ctor / dtor (header opt-in CTOR).
// Shape: an element type with an out-of-line constructor AND destructor under /EHsc gives the five-argument iterator.
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATALLPASSSECTION_CTOR
#define FABLE_RETAIL_CFLOATDIFFUSER_CTOR
#include "engine/CFloatDiffuser.h"

CFloatDiffuser::CFloatDiffuser()
{
}
