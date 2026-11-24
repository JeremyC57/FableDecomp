// std::_Rb_tree<CThingBuilding*, std::pair<CThingBuilding* const, std::list<CThingAICreature*> >, ...>::~_Rb_tree() -- STLport tree destructor: clear() (the out-of-line recursive _M_erase from the root, header relinked, count zeroed), then _Rb_tree_base frees the header node
// Leaf: ??1?$_Rb_tree@PAVCThingBuilding@@U?$pair@QAVCThingBuilding@@V?$list@PAVCThingAICreature@@V?$allocator@PAVCThingAICreature@@@std@@@std@@@std@@U?$_Select1st@U?$pair@QAVCThingBuilding@@V?$list@PAVCThingAICreature@@V?$allocator@PAVCThingAICreature@@@std@@@std@@@std@@@3@U?$less@PAVCThingBuilding@@@3@V?$allocator@U?$pair@QAVCThingBuilding@@V?$list@PAVCThingAICreature@@V?$allocator@PAVCThingAICreature@@@std@@@std@@@std@@@3@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051AF20, 66 bytes, thiscall; no retail referrer (the linker kept the pick-any copy). Callees: _M_erase 0x00519970, free 0x00BFEA14.
// Identity (NF391, T3 nf389 0x0051B720; advisor decision, owner delegated): retail CWorldMap::PopulateRegionWithVillagers 0x00502E90 (both PDB
//   name sets; its only callers are the CWorldMap slot 22 PostRegionLoad, CTCVillage slot 9 FrameUpdate and CGameScriptInterface slot 754
//   RepopulateVillage, as in FableWin) calls 0x0051B720 at 0x005048C6 / 0x005048D2 on two map<CThingBuilding*, list<CThingAICreature*> >
//   locals, where FableWin's PopulateRegionWithVillagers destroys its two locals of that type with ??1?$map. A full-image E8/E9 scan of the
//   tree's _M_erase 0x00519970 finds exactly two standalone 66-byte destructor bodies, 0x0051AF20 and 0x0051B720; the TU that emits ~map
//   also emits the 66-byte ~_Rb_tree calling _M_erase directly, so the unreferenced twin 0x0051AF20 is the tree's own destructor.
// NF391: was the generated CRetailTree_erase00519970 destructor over a local tree model; now the repo STLport in the pick-any form
//   (the same TU shape as 0x0051B720: that object emits both destructors).
#include "fable_compiler.h"
#include "rebuild_abi.h"
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
class CThingBuilding;
class CThingAICreature;
namespace {
// Fidelity: scaffolding, not retail: a TU-internal holder whose out-of-line destructor destroys the map member, so VC7.1 emits the
// implicit map / tree destructors (and _M_erase) as pick-any COMDATs, as every retail TU that destroys this map does (GOTCHAS nf86 / nf88,
// nf387). Internal to this TU; never called.
struct BuildingCreatureMapHolder {
    std::map<CThingBuilding*, std::list<CThingAICreature*> > Buildings;
    ~BuildingCreatureMapHolder();
};
BuildingCreatureMapHolder::~BuildingCreatureMapHolder() {}
}
