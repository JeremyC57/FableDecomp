// std::vector<CIntelligentPointer<CThing> >::~vector() -- STLport 4.6 destructor: destroy every element (virtual ~CIntelligentPointer, slot 0, stride 8), then free the block
// Leaf: ??1?$vector@V?$CIntelligentPointer@VCThing@@@@V?$allocator@V?$CIntelligentPointer@VCThing@@@@@std@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004406D0, thiscall, plain `ret`.
// Identity: the CTCTargetingSpirit scalar deleting destructor 0x004E1023 destroys Targets (+0x3C; Ego_r.pdb CTCTargetingSpirit::Targets is
//   vector<CIntelligentPointer<CThing> >, engine/CTCTargetingSpirit.h) through it, and the landed binder 0x00903383 (CAIStateGroup_HoverHornetQueen,
//   Things / CircleMarkers, Ego_r.pdb vector<CIntelligentPointer<CThing> >) already calls this spelling (reloc_bindings.tsv). The catalog's
//   vector<CConversation> label was propagated across a byte-identical body. Descriptive-by-library (STLport spelling; both PDBs are Dinkumware).
// NF72 (round 7, q10901): was a generic local {first,last} stand-in; re-authored as the explicit instantiation on stlport/fable_containers.h with the
//   shared engine/CIntelligentPointer.h (retail branch: vptr +0x0, PData +0x4; virtual destructor = the `call [vtbl]` with flag 0 per element).
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#include "stlport/fable_containers.h"
class CThing;
template std::vector<CIntelligentPointer<CThing> >::~vector();
