// std::pair<const unsigned __int64, CGroupMemberData>::`scalar deleting destructor'(unsigned int flags) -- the value type of
//   CTCCreatureGroup::Members (map<unsigned __int64, CGroupMemberData>): ~pair (CGroupMemberData::PMember, a CIntelligentPointer at +8,
//   by ~CBaseIntelligentPointer 0x00A01C10), then operator delete when bit 0 is set; returns this
// Leaf: ??_G?$pair@$$CB_KVCGroupMemberData@@@std@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DBE39, 31 bytes, thiscall, `ret 4`; size-optimised TU. Two references, both `push 0 ; call`:
//   the tree's _M_erase 0x004DFB8F (at 0x004DFBAE, the per-node destroy) and the unreferenced __fastcall _Destroy copy 0x004DD6F3 (at
//   0x004DD6F5). Compiler-generated: STLport _construct.h `_Destroy(p) { p->~T(); }` is emitted by VC7.1 as a call of this ??_G with 0.
// Name: the binding table already maps this spelling here (NF398 b1, witness 0x004DFB8F, whose object emits it byte-identical); the
//   0x00813460 object emits the same 31-byte copy under FABLE_RETAIL_STLP_GROUP_MEMBER_DESTROY_SIZE_OWNER. STL instances have no PDB
//   names: the spelling is the repo STLport's (descriptive-by-library); the type is Ego_r.pdb's CTCCreatureGroup::Members value type.
// NF405: was the strong free stand-in ?vector_deleting_destructor@@YIPAXPAVCLandscapeBackgroundPatch@@HI@Z (catalog
//   ScalarDeletingDtor_004dbe39 / manifest CLandscapeBackgroundPatch::`vector_deleting_destructor', propagated) on a row-local model;
//   four other rows (0x004DA6A9, 0x004E41A1, 0x005C1390, 0x00656EAD) still define that stand-in for their own bodies.
// Fidelity: scaffolding, not retail (nf86 / nf88 pattern): the deleting destructor is a compiler-emitted COMDAT; the anonymous-namespace
//   helper below destroys a pair through a typedef so this object emits it pick-any, as 0x004DFB8F's does (same define set). Internal to
//   this TU; never called. `#pragma optimize("s",on)` is the row's own mode (cited by the size-pragma opt-ins of engine/CGroupMemberData.h).
#pragma optimize("s",on)
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
void EmitGroupMemberPairDeletingDestructor(CGroupMemberPair* member)
{
    member->~CGroupMemberPair();
}
}
