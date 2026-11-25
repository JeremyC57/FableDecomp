// std::map<unsigned __int64, CGroupMemberData>::~map() -- (CTCCreatureGroup::Members) the implicit STLport map destructor: a tail jump
//   to the tree destructor ~_Rb_tree 0x004E350F
// Leaf: ??1?$map@_KVCGroupMemberData@@U?$less@_K@std@@V?$allocator@U?$pair@$$CB_KVCGroupMemberData@@@std@@@3@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E409B, 5 bytes (`jmp 0x004E350F`), unreferenced (no E8 / E9 / abs32 reference anywhere); between
//   a function ending `leave ; ret` at 0x004E409A and the next function at 0x004E40A0. The only jump to 0x004E350F in the image.
// Identity (NF398 / NF405): the ~CTCCreatureGroup compiles (NF398 probe, row 0x004E5C71) emit exactly this ~map as a pick-any COMDAT,
//   `jmp ??1?$_Rb_tree`, beside the 23-byte tree destructor that is 0x004E350F. STL instances have no PDB names: the spelling is the repo
//   STLport's (descriptive-by-library); the type is Ego_r.pdb's CTCCreatureGroup::Members.
// Fidelity: scaffolding, not retail (nf86 / nf88 pattern, the 0x009AC310 form): the anonymous-namespace helper calls the implicit
//   destructor so this object emits it pick-any. Size-optimised TU (the define set of 0x004E350F: clear and _M_erase declared only).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_MERASE_DECL_ONLY
#include "stlport/fable_map.h"
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CThingAICreature.h"
#include "engine/CGroupMemberData.h"

namespace {
typedef std::map<unsigned __int64, CGroupMemberData> CGroupMemberMap;
void DestroyGroupMemberMap(CGroupMemberMap* members) { members->CGroupMemberMap::~CGroupMemberMap(); }
}
