// std::list<CIntelligentPointer<CThing> >::~list() -- STLport list destructor: _List_base::clear() out of line (0x004B8050), then free the header node
// Leaf: ??1?$list@V?$CIntelligentPointer@VCThing@@@@V?$allocator@V?$CIntelligentPointer@VCThing@@@@@std@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004B8940, thiscall, plain `ret`.
// Identity: ~CTCTargetingAI 0x004E0080 destroys ThingsToNotAttack (+0x48; Ego_r.pdb CTCTargetingAI member list<CIntelligentPointer<CThing> >,
//   engine/CTCTargetingAI.h) through it, and the landed binder 0x00699BF0 binds the clear callee 0x004B8050 as
//   ?clear@?$_List_base@V?$CIntelligentPointer@VCThing@@@@...@std@@QAEXXZ. The catalog _global::StdList_Dtor_004b8940 and the self leaf
//   OnReadFinished were stand-ins. Descriptive-by-library (STLport spelling; both PDBs are Dinkumware).
// NF72 (round 7, q10902): was a local CActiveFile stand-in; re-authored as the explicit instantiation on stlport/fable_containers.h, the
//   0x0042ABCA form (FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE: clear declared only; FABLE_RETAIL_STLP_LIST_DTOR_IN_LIST: clear + free in ~list).
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#define FABLE_RETAIL_STLP_LIST_DTOR_IN_LIST
#include "stlport/fable_containers.h"
class CThing;
template std::list<CIntelligentPointer<CThing>, std::allocator<CIntelligentPointer<CThing> > >::~list();
