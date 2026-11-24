// std::list<CThingAICreature*>::size() const -- STLport distance(begin(), end()): walk the ring from the sentinel's next node back to the sentinel, counting the nodes
// Leaf: ?size@?$list@PAVCThingAICreature@@V?$allocator@PAVCThingAICreature@@@std@@@std@@QBEIXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005143E0, 24 bytes, thiscall, plain `ret`; no callees.
// Identity (NF391, NF385 no-second-source row traced): retail CWorldMap::PopulateRegionWithVillagers 0x00502E90 (both PDB name sets; its
//   only callers are the RTTI slots CWorldMap 22 PostRegionLoad, CTCVillage 9 FrameUpdate and CGameScriptInterface 754 RepopulateVillage,
//   as in FableWin) calls 0x005143E0 at 0x00503495 / 0x0050351C on the list<CThingAICreature*>& that map<CThingBuilding*,
//   list<CThingAICreature*> >::operator[] 0x0051BF90 returns; FableWin's PopulateRegionWithVillagers calls
//   ?size@?$list@PAVCThingAICreature@@V?$allocator@PAVCThingAICreature@@@std@@@std@@QBEIXZ (world_map.obj) on that list. STLport and the PDB
//   agree on this spelling (list<T, allocator<T> >::size() const, size_t). The byte-identical 0x00615220 is list<CIntelligentPointer<CThing> >'s.
// NF391: was the `SortTreeRecursively(Tree*)` stand-in (catalog label "CGuiControlTreePane::SortTreeRecursively", propagated), shared with
//   0x00615220. Model: the shared stlport/fable_containers.h list with STLport's own distance(begin(), end()) (the 0x00ADB570 form, NF39);
//   explicit instantiation emits the member as its own pick-any COMDAT, as retail kept every used inline member.
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#include "stlport/fable_map.h"
class CThingAICreature;

template unsigned int std::list<CThingAICreature*, std::allocator<CThingAICreature*> >::size() const;
