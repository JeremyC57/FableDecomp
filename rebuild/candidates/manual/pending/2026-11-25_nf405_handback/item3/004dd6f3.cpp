// std::_Destroy<std::pair<const unsigned __int64, CGroupMemberData> >(p) -- STLport _construct.h value destroy (CTCCreatureGroup::Members
//   map nodes), the __fastcall copy (p in ecx): `push 0 ; call ??_G pair 0x004DBE39 ; ret`
// Leaf: ??$_Destroy@U?$pair@$$CB_KVCGroupMemberData@@@std@@@std@@YIXPAU?$pair@$$CB_KVCGroupMemberData@@@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DD6F3, 8 bytes, unreferenced; boundary by hand (NF398 evidence/destroy_home_004dd6f3.txt):
//   complete 5-byte jmp at 0x004DD6EE before it, next function at 0x004DD6FB; the masked body occurs 49 times but only here does the
//   call resolve to 0x004DBE39.
// Identity: NF398 b1 (rows 0x004DFB8F / 0x00813460 emit this COMDAT under FABLE_RETAIL_STLP_DESTROY_FASTCALL, byte-identical to retail
//   here). STL instances have no PDB names: the spelling is the repo STLport's (descriptive-by-library).
// Fidelity: scaffolding, not retail (nf86 / nf88 pattern): the anonymous-namespace helper takes the template's address so this object
//   emits it pick-any. Size-optimised TU (0x004DBE39 / 0x004DFB8F set `#pragma optimize("s",on)`).
#pragma optimize("s",on)
#define FABLE_RETAIL_STLP_DESTROY_FASTCALL
#include "stlport/fable_map.h"
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CThingAICreature.h"
#include "engine/CGroupMemberData.h"

namespace {
typedef std::pair<const unsigned __int64, CGroupMemberData> CGroupMemberPair;
typedef void (__fastcall* DestroyGroupMemberPairFn)(CGroupMemberPair*);
DestroyGroupMemberPairFn DestroyGroupMemberPairAddress() { return &std::_Destroy<CGroupMemberPair>; }
}
