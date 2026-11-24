// std::_Rb_tree<CThingBuilding*, std::pair<CThingBuilding* const, std::list<CThingAICreature*> >, ...>::_M_erase(_Rb_tree_node* x) -- STLport recursive subtree release: erase the right subtree, destroy the value (the list's nodes and sentinel freed), free the node, continue left
// Leaf: ?_M_erase@?$_Rb_tree@PAVCThingBuilding@@U?$pair@QAVCThingBuilding@@V?$list@PAVCThingAICreature@@V?$allocator@PAVCThingAICreature@@@std@@@std@@@std@@U?$_Select1st@U?$pair@QAVCThingBuilding@@V?$list@PAVCThingAICreature@@V?$allocator@PAVCThingAICreature@@@std@@@std@@@std@@@3@U?$less@PAVCThingBuilding@@@3@V?$allocator@U?$pair@QAVCThingBuilding@@V?$list@PAVCThingAICreature@@V?$allocator@PAVCThingAICreature@@@std@@@std@@@std@@@3@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@QAVCThingBuilding@@V?$list@PAVCThingAICreature@@V?$allocator@PAVCThingAICreature@@@std@@@std@@@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00519970, 108 bytes (+ int3 padding to 112), thiscall `ret 4`. Callees: itself (right subtree), free
//   0x00BFEA14 (list nodes, the list sentinel, the tree node).
// Identity: the tree of map<CThingBuilding*, list<CThingAICreature*> > (FableWin world_map.obj holds only this map over it, next to
//   CWorldMap::PopulateRegionWithVillagers, which expands its destructors at 0x005062FB / 0x00506354; the landed operator[] 0x0051BF90 carries
//   the Ego_r name of the map). The repo STLport's _M_erase for this tree is EQ_MASKED to retail (NF389 evidence/cmp_live_51af20.txt).
//   Private STLport member (AAE). NAME_FIXES nf389.
// NF391: was ?_M_erase@CBuildingCreatureMapTree@@ over a local model; now the repo STLport (stlport/fable_map.h) as the explicit
//   instantiation of the private member (the 0x0051AA00 _M_insert pattern): a pick-any COMDAT, as retail's TUs emit it.
#include "fable_compiler.h"
#include "rebuild_abi.h"
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
class CThingBuilding;
class CThingAICreature;
typedef std::pair<CThingBuilding* const, std::list<CThingAICreature*> > TBuildingCreatureValue;
template void std::_Rb_tree<CThingBuilding*, TBuildingCreatureValue, std::_Select1st<TBuildingCreatureValue>, std::less<CThingBuilding*>,
    std::allocator<TBuildingCreatureValue> >::_M_erase(std::_Rb_tree_node<TBuildingCreatureValue>*);
