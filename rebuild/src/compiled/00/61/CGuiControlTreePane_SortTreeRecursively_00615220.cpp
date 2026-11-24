// std::list<CIntelligentPointer<CThing> >::size() const -- STLport distance(begin(), end()): walk the ring from the sentinel's next node back to the sentinel, counting the nodes
// Leaf: ?size@?$list@V?$CIntelligentPointer@VCThing@@@@V?$allocator@V?$CIntelligentPointer@VCThing@@@@@std@@@std@@QBEIXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00615220, 24 bytes, thiscall, plain `ret`; no callees.
// Identity (NF391, NF385 no-second-source row traced): retail RTTI CAIStateGroup_BanditSitRoundCampfire vftable 0x0128B78C slot 3
//   (= FableWin ?InitCreaturePosition@CAIStateGroup_BanditSitRoundCampfire@@UAE_NXZ) is 0x0093AA40; it counts its local list once inline
//   (0x0093AAC0 loop) and calls 0x00615220 on it at 0x0093AADF (`lea ecx,[esp+10h]`). FableWin's InitCreaturePosition calls
//   ?size@?$list@V?$CIntelligentPointer@VCThing@@@@V?$allocator@V?$CIntelligentPointer@VCThing@@@@@std@@@std@@QBEIXZ twice on its local
//   list<CIntelligentPointer<CThing> > (0x02B7D3E7 / 0x02B7D411). The byte-identical 0x005143E0 is list<CThingAICreature*>'s.
// NF391: was the `SortTreeRecursively(Tree*)` stand-in (catalog label "CGuiControlTreePane::SortTreeRecursively", propagated), shared with
//   0x005143E0. Model: the shared stlport/fable_containers.h list with STLport's own distance(begin(), end()) (the 0x00ADB570 form, NF39);
//   explicit instantiation emits the member as its own pick-any COMDAT.
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#include "stlport/fable_map.h"
#include "engine/CIntelligentPointer.h"
class CThing;

template unsigned int std::list<CIntelligentPointer<CThing>, std::allocator<CIntelligentPointer<CThing> > >::size() const;
