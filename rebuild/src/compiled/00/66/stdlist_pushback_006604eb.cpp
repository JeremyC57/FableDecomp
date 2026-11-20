// std::list<CIntelligentPointer<CThingAICreature> >::push_back(const value_type& x) -- STLport 4.6 _list.h `insert(end(), x)` with insert called out of line
// Leaf: ?push_back@?$list@V?$CIntelligentPointer@VCThingAICreature@@@@V?$allocator@V?$CIntelligentPointer@VCThingAICreature@@@@@std@@@std@@QAEXABV?$CIntelligentPointer@VCThingAICreature@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006604EB, 26 bytes, thiscall `ret 4`, size-optimised TU with a frame: the end() iterator is copied into
//   its reserved argument slot (`push ecx ; mov eax,esp ; mov [eax],edx`), the returned iterator goes to the dead argument word
//   (`lea eax,[ebp+8] ; push eax`), then list<CIntelligentPointer<CThingAICreature> >::insert(iterator, const T&) 0x005151D0.
// Names (NF115, PRIORITY q4491; was the stand-in ListWrapper_006604eb::ListPushFrontWrapper_006604eb): FableWin.pdb STLport-equivalent
//   list<CIntelligentPointer<CThingAICreature> >::push_back; CTCVillage::VillagePeople is that list (Ego_r layout) and
//   CTCVillage::AddThingToVillage 0x0065B854 (case 1) calls this address under this spelling (RELOCATION_MATCH), as do 0x006DE8C0 and
//   0x006DFA50. Descriptive-by-library (STLport 4.6; both PDBs are Dinkumware builds).
// Shape: explicit instantiation of stlport/fable_containers.h list::push_back with FABLE_RETAIL_STLP_LIST_INSERT_OUTOFLINE (the
//   0x005A887D opt-in: insert visible but kept out of line, as retail calls 0x005151D0).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_INSERT_OUTOFLINE
#include "rebuild_abi.h"
#include "engine/CIntelligentPointer.h"
#include "stlport/fable_containers.h"
class CThingAICreature;
template void std::list<CIntelligentPointer<CThingAICreature> >::push_back(const CIntelligentPointer<CThingAICreature>&);
