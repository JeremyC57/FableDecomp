// std::list<CIntelligentPointer<CThingAICreature> >::size() const -- STLport 4.6 list size: count the nodes from the head's next back to the head
// Leaf: ?size@?$list@V?$CIntelligentPointer@VCThingAICreature@@@@V?$allocator@V?$CIntelligentPointer@VCThingAICreature@@@@@std@@@std@@QBEIXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065F17B, 16 bytes: `mov edx,[ecx] ; mov ecx,[edx] ; xor eax,eax ; jmp T ; L: mov ecx,[ecx] ;
//   inc eax ; T: cmp ecx,edx ; jne L ; ret` (size TU, `#pragma optimize("s",on)` as the landed row had it).
// Identity: FableWin.pdb 0x0216B9BB ?size@?$list@V?$CIntelligentPointer@VCThingAICreature@@@@...@std@@QBEIXZ. Second source: its callers
//   read the PDB list<CIntelligentPointer<CThingAICreature> > members -- CTCFollowed::FrameUpdate 0x006D53C0 on AIFollowers (Ego_r.pdb
//   CTCFollowed +24), CTCTrophy::GlobalUpdate 0x006DEEE0 and CGameScriptInterface::AddCrimeCommitted 0x0089FEB0 on CTCVillage::VillagePeople
//   (Ego_r.pdb list<CIntelligentPointer<CThingAICreature> >); the two landed callers already bind this spelling (reloc_bindings).
//   STLport instance found in no PDB in this exact form (both PDBs are Dinkumware builds): descriptive-by-library, the template
//   arguments are the callers' PDB member types.
// Name fix (agentNF55, 2026-11-17): re-authored as the explicit instantiation on stlport/fable_containers.h (FABLE_RETAIL_STLP_LIST);
//   it was the stand-in List::Count() (manifest label CBasicString<char>::CalcStringLength).
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "stlport/fable_containers.h"
#include "engine/CIntelligentPointer.h"
#pragma optimize("s",on)
class CThingAICreature;

template size_t std::list<CIntelligentPointer<CThingAICreature> >::size() const;
